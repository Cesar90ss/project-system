// post.cc
// 	Routines to deliver incoming network messages to the correct
//	"address" -- a mailbox, or a holding area for incoming messages.
//	This module operates just like the US postal service (in other
//	words, it works, but it's slow, and you can't really be sure if
//	your mail really got through!).
//
//	Note that once we prepend the MailHdr to the outgoing message data,
//	the combination (MailHdr plus data) looks like "data" to the Network
//	device.
//
// 	The implementation synchronizes incoming messages with threads
//	waiting for those messages.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "post.h"
#include "thread.h"
//class Thread;

#include <strings.h> /* for bzero */


#define NB_INTERNAL_PORTS 256
#define MAX_NB_PORT (NB_BOX * NB_INTERNAL_PORTS) 		//number of mailbox times number of internal available port used to communicate
//----------------------------------------------------------------------
// Mail::Mail
//      Initialize a single mail message, by concatenating the headers to
//	the data.
//
//	"pktH" -- source, destination machine ID's
//	"mailH" -- source, destination mailbox ID's
//	"data" -- payload data
//----------------------------------------------------------------------

Mail::Mail(PacketHeader pktH, MailHeader mailH, char *msgData)
{
    ASSERT(mailH.length <= MaxMailSize);

    pktHdr = pktH;
    mailHdr = mailH;
    bcopy(msgData, data, mailHdr.length);
}

//----------------------------------------------------------------------
// MailBox::MailBox
//      Initialize a single mail box within the post office, so that it
//	can receive incoming messages.
//
//	Just initialize a list of messages, representing the mailbox.
//----------------------------------------------------------------------


MailBox::MailBox()
{
    int i;
	messages = new SynchList*[MAX_NB_PORT];
	for(i=0;i<MAX_NB_PORT;i++)
	{
		messages[i] = new SynchList();
	}
}

//----------------------------------------------------------------------
// MailBox::~MailBox
//      De-allocate a single mail box within the post office.
//
//	Just delete the mailbox, and throw away all the queued messages
//	in the mailbox.
//----------------------------------------------------------------------

MailBox::~MailBox()
{
    delete messages;
}

//----------------------------------------------------------------------
// PrintHeader
// 	Print the message header -- the destination machine ID and mailbox
//	#, source machine ID and mailbox #, and message length.
//
//	"pktHdr" -- source, destination machine ID's
//	"mailHdr" -- source, destination mailbox ID's
//----------------------------------------------------------------------

static void
PrintHeader(PacketHeader pktHdr, MailHeader mailHdr)
{
    printf("From (%d, %d) to (%d, %d) bytes %d\n",
    	    pktHdr.from, mailHdr.from, pktHdr.to, mailHdr.to, mailHdr.length);
}

//----------------------------------------------------------------------
// MailBox::Put
// 	Add a message to the mailbox.  If anyone is waiting for message
//	arrival, wake them up!
//
//	We need to reconstruct the Mail message (by concatenating the headers
//	to the data), to simplify queueing the message on the SynchList.
//
//	"pktHdr" -- source, destination machine ID's
//	"mailHdr" -- source, destination mailbox ID's
//	"data" -- payload message data
//----------------------------------------------------------------------

void
MailBox::Put(PacketHeader pktHdr, MailHeader mailHdr, char *data)
{
    Mail *mail = new Mail(pktHdr, mailHdr, data);
	int sid = mailHdr.sid;
    messages[sid]->Append((void *)mail);	// put on the end of the list of
					// arrived messages, and wake up
					// any waiters
}

//----------------------------------------------------------------------
// MailBox::Get
// 	Get a message from a mailbox, parsing it into the packet header,
//	mailbox header, and data.
//
//	The calling thread waits if there are no messages in the mailbox.
//
//	"pktHdr" -- address to put: source, destination machine ID's
//	"mailHdr" -- address to put: source, destination mailbox ID's
//	"data" -- address to put: payload message data
//----------------------------------------------------------------------

void
MailBox::Get(PacketHeader *pktHdr, MailHeader *mailHdr, char *data, int sid)
{
    DEBUG('n', "Waiting for mail in mailbox\n");
    Mail *mail = (Mail *) messages[sid]->Remove();	// remove message from list;
						// will wait if list is empty
						//change message in array to make sure right message will be taken in the right port
    *pktHdr = mail->pktHdr;
    *mailHdr = mail->mailHdr;
    if (DebugIsEnabled('n')) {
	printf("Got mail from mailbox: ");
	PrintHeader(*pktHdr, *mailHdr);
    }
    bcopy(mail->data, data, mail->mailHdr.length);
					// copy the message data into
					// the caller's buffer
    delete mail;			// we've copied out the stuff we
					// need, we can now discard the message
}

//----------------------------------------------------------------------
// PostalHelper, ReadAvail, WriteDone
// 	Dummy functions because C++ can't indirectly invoke member functions
//	The first is forked as part of the "postal worker thread; the
//	later two are called by the network interrupt handler.
//
//	"arg" -- pointer to the Post Office managing the Network
//----------------------------------------------------------------------

static void PostalHelper(int arg)
{ PostOffice* po = (PostOffice *) arg; po->PostalDelivery(); }
static void ReadAvail(int arg)
{ PostOffice* po = (PostOffice *) arg; po->IncomingPacket(); }
static void WriteDone(int arg)
{ PostOffice* po = (PostOffice *) arg; po->PacketSent(); }

//----------------------------------------------------------------------
// PostOffice::PostOffice
// 	Initialize a post office as a collection of mailboxes.
//	Also initialize the network device, to allow post offices
//	on different machines to deliver messages to one another.
//
//      We use a separate thread "the postal worker" to wait for messages
//	to arrive, and deliver them to the correct mailbox.  Note that
//	delivering messages to the mailboxes can't be done directly
//	by the interrupt handlers, because it requires a Lock.
//
//	"addr" is this machine's network ID
//	"reliability" is the probability that a network packet will
//	  be delivered (e.g., reliability = 1 means the network never
//	  drops any packets; reliability = 0 means the network never
//	  delivers any packets)
//	"nBoxes" is the number of mail boxes in this Post Office
//----------------------------------------------------------------------

PostOffice::PostOffice(NetworkAddress addr, double reliability)
{
// First, initialize the synchronization with the interrupt handlers
    messageAvailable = new Semaphore("message available", 0);
    messageSent = new Semaphore("message sent", 0);
    sendLock = new Lock("message send lock");

// Second, initialize the mailboxes
    netAddr = addr;
    numBoxes = NB_BOX;
    boxes = new MailBox[NB_BOX];

// Third, initialize the network; tell it which interrupt handlers to call
    network = new Network(addr, reliability, ReadAvail, WriteDone, (int) this);


// Finally, create a thread whose sole job is to wait for incoming messages,
//   and put them in the right mailbox.
    NetworkDeamon = new Thread("postal worker");

    NetworkDeamon->Fork_No_User_Space(PostalHelper, (int) this);
}

//----------------------------------------------------------------------
// PostOffice::~PostOffice
// 	De-allocate the post office data structures.
//----------------------------------------------------------------------

PostOffice::~PostOffice()
{
    delete network;
    delete [] boxes;
    delete messageAvailable;
    delete messageSent;
    delete sendLock;
    delete NetworkDeamon;
}

//----------------------------------------------------------------------
// PostOffice::PostalDelivery
// 	Wait for incoming messages, and put them in the right mailbox.
//
//      Incoming messages have had the PacketHeader stripped off,
//	but the MailHeader is still tacked on the front of the data.
//----------------------------------------------------------------------

void
PostOffice::PostalDelivery()
{
    PacketHeader pktHdr;
    MailHeader mailHdr;
    char m_buffer[MaxPacketSize];	//we want to avoid memory leak, so alloc on
    char *buffer = m_buffer;		//stack and then point on allocated memory, the function will not exit so
					// there is no risk of data loss
    for (;;) {
        // first, wait for a message
        messageAvailable->P();
        pktHdr = network->Receive(buffer);

        mailHdr = *(MailHeader *)buffer;
        if (DebugIsEnabled('n')) {
	    printf("Putting mail into mailbox: ");
	    PrintHeader(pktHdr, mailHdr);
        }

	// check that arriving message is legal!
	ASSERT(0 <= mailHdr.to && mailHdr.to < numBoxes);
	ASSERT(mailHdr.length <= MaxMailSize);

	// put into mailbox
        boxes[mailHdr.to].Put(pktHdr, mailHdr, buffer + sizeof(MailHeader));
    }
}

//----------------------------------------------------------------------
// PostOffice::Send
// 	Concatenate the MailHeader to the front of the data, and pass
//	the result to the Network for delivery to the destination machine.
//
//	Note that the MailHeader + data looks just like normal payload
//	data to the Network.
//
//	"pktHdr" -- source, destination machine ID's
//	"mailHdr" -- source, destination mailbox ID's
//	"data" -- payload message data
//----------------------------------------------------------------------

void
PostOffice::Send(PacketHeader pktHdr, MailHeader mailHdr, const char* data)
{
    char* buffer = new char[MaxPacketSize]();	// space to hold concatenated
						// mailHdr + data

    if (DebugIsEnabled('n')) {
	printf("Post send: ");
	PrintHeader(pktHdr, mailHdr);
    }
    ASSERT(mailHdr.length <= MaxMailSize);
    ASSERT(0 <= mailHdr.to && mailHdr.to < numBoxes);

    // fill in pktHdr, for the Network layer
    pktHdr.from = netAddr;
    pktHdr.length = mailHdr.length + sizeof(MailHeader);

    // concatenate MailHeader and data
    bcopy(&mailHdr, buffer, sizeof(MailHeader));
    bcopy(data, buffer + sizeof(MailHeader), mailHdr.length);

    sendLock->Acquire();   		// only one message can be sent
					// to the network at any one time
    network->Send(pktHdr, buffer);
    messageSent->P();			// wait for interrupt to tell us
					// ok to send the next message
    sendLock->Release();

    delete [] buffer;			// we've sent the message, so
					// we can delete our buffer
}

//----------------------------------------------------------------------
// PostOffice::Send
// 	Retrieve a message from a specific box if one is available,
//	otherwise wait for a message to arrive in the box.
//
//	Note that the MailHeader + data looks just like normal payload
//	data to the Network.
//
//
//	"box" -- mailbox ID in which to look for message
//	"pktHdr" -- address to put: source, destination machine ID's
//	"mailHdr" -- address to put: source, destination mailbox ID's
//	"data" -- address to put: payload message data
//----------------------------------------------------------------------

void
PostOffice::Receive(int box, PacketHeader *pktHdr,
				MailHeader *mailHdr, char* data)
{
    ASSERT((box >= 0) && (box < numBoxes));

    boxes[box].Get(pktHdr, mailHdr, data,0);
    ASSERT(mailHdr->length <= MaxMailSize);
}

//----------------------------------------------------------------------
// PostOffice::IncomingPacket
// 	Interrupt handler, called when a packet arrives from the network.
//
//	Signal the PostalDelivery routine that it is time to get to work!
//----------------------------------------------------------------------

void
PostOffice::IncomingPacket()
{
    messageAvailable->V();
}

//----------------------------------------------------------------------
// PostOffice::PacketSent
// 	Interrupt handler, called when the next packet can be put onto the
//	network.
//
//	The name of this routine is a misnomer; if "reliability < 1",
//	the packet could have been dropped by the network, so it won't get
//	through.
//----------------------------------------------------------------------

void
PostOffice::PacketSent()
{
    messageSent->V();
}


// enable listening on a "port"
int PostOffice::EnableListening(int i_local_port)
{
	return 0;
}

NachosSocket** PostOffice::FreeConnectionPlace(int i_local_port)
{
	return NULL;
}
	
//Retrieve connection from a port
int PostOffice::searchConnection( int i_local_port,int i_remote_machine, int i_remote_port)
{
	return 0;
}

bool PostOffice::IsListening(int i_local_port)
{
	return false;
}

//----------------------------------------------------------------------
// NachosSocket::NachosSocket
// 
//----------------------------------------------------------------------
NachosSocket::NachosSocket(SocketStatusEnum i_status, int i_remote_machine, int i_remote_port, int i_local_port)
{
	status = i_status;
	
	local_port = i_local_port;
	remote_port = i_remote_port;
	remote_machine = i_remote_machine;
	
	messages = NULL;
}

NachosSocket::~NachosSocket()
{
	delete messages;
}

/**
 * Receive : read a message of at most size bytes from the socket 
 * and write it inside the buffer given as argument. If it can
 * not read full size, the actual readen size will be returned.
 * Return 0 if there is nothing to read, -1 if the socket is closed
 * -2 if the socket is waiting for connection
 */
Mail *NachosSocket::Receive(char *buffer, size_t size)
{
	/*if(status != SOCKET_CONNECTED)
	{
		return -1;
	}*/
	
	//get size bytes in the buffer or less if we can't and return the number of read bytes
	return NULL;
}


/**
 * Send : Write a message read from buffer of at most size bytes 
 * in the socket  . If it can not read full size, the actual written size 
 * will be returned.
 * Return 0 if transmission is a success, -1 if the socket is not 
 * connected, -2 if a transmission problem.
 */
int NachosSocket::Send(char *buffer, size_t size)
{
	if(status != SOCKET_CONNECTED)
	{
		return -1;
	}
	
	//TODO Sending, return -2 if fail many times
	
	return 0;
}


/**
 * Disconnect the socket(update its status).
 * Return -1 if socket is already disconnected, 0 if no error.
 */
int NachosSocket::Disconnect()
{
	if(status != SOCKET_CONNECTED)
	{
		return -1;
	}

	//TODO send the close message
	status = SOCKET_CLOSED;
	return 0;
}

int NachosSocket::LocalPort()
{
	return local_port;
}

bool NachosSocket::IsListening()
{
	return (status == SOCKET_LISTENING);
}
