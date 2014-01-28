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
#include "stats.h"
//class Thread;

#include <strings.h> /* for bzero */
class Thread;

extern PostOffice* postOffice;
extern Thread *currentThread;
extern Statistics *stats;

#define NB_REEMISSION 500
#define SEND_TIMEOUT 10000
#define TIMEOUT SEND_TIMEOUT*(NB_REEMISSION+1)

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
	Sockets = new NachosSocket*[NB_CONNECTION_PER_PORT];
	for(i=0;i<NB_CONNECTION_PER_PORT;i++)
	{
		Sockets[i] = NULL;
	}
	Listener = NULL;
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
    int i;
	for(i=0;i<NB_CONNECTION_PER_PORT;i++)
	{
		if(Sockets[i] != NULL)
		{
			delete Sockets[i];
		}
	}
	delete [] Sockets;
	if(Listener != NULL)
	{
		delete Listener;
	}
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
MailBox::Put(int sid,PacketHeader pktHdr, MailHeader mailHdr, char *data)
{
    Mail *mail = new Mail(pktHdr, mailHdr, data);
    Sockets[sid]->messages->Append((void *)mail);	// put on the end of the list of
													// arrived messages, and wake up
													// any waiter on the socket
}

void
MailBox::PutRequest(PacketHeader pktHdr, MailHeader mailHdr, char *data)
{
    Mail *mail = new Mail(pktHdr, mailHdr, data);
	Listener->messages->Append((void *)mail);	// put on the end of the list of
													// arrived messages, and wake up
													// any waiter on the socket
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
	Mail *mail;
	if(Sockets[sid] != NULL)
    	mail = (Mail *) Sockets[sid]->messages->Remove();	// remove message from list;
						// will wait if list is empty
						//change message in array to make sure right message will be taken in the right port
	else
	{
		DEBUG('n', "There is no such socket using this mailbox\n");
		return;
	}
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

int
MailBox::SearchByRemote(int machine_from, int mailBox_from)
{
	int i;
	for(i = 0; i<NB_CONNECTION_PER_PORT; i++)
	{
		if(Sockets[i] != NULL && (Sockets[i]->RemoteMachine() == machine_from) && (Sockets[i]->RemotePort() == mailBox_from) )
			return i;
	}
	return -1;
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


int PostOffice::NumBoxes()
{
	return numBoxes;
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
    PacketHeader pktHdr, conf_pktHdr ;
    MailHeader mailHdr, conf_mailHdr ;
	NachosSocket* socket;
    char m_buffer[MaxPacketSize];	//we want to avoid memory leak, so alloc on
    char *buffer = m_buffer;		//stack and then point on allocated memory, the function will not exit so
					// there is no risk of data loss
    for (;;) 
	{
        // first, wait for a message
        messageAvailable->P();
        pktHdr = network->Receive(buffer);

        mailHdr = *(MailHeader *)buffer;
        if (DebugIsEnabled('n'))
		{
		    printf("Putting mail into mailbox: ");
		    PrintHeader(pktHdr, mailHdr);
        }

		// check that arriving message is legal!
		ASSERT(0 <= mailHdr.to && mailHdr.to < numBoxes);
		ASSERT(mailHdr.length <= MaxMailSize);

		// put into mailbox
		switch(mailHdr.mailType)
		{
			case CONFIRMATION:
			{
				boxes[mailHdr.to].Sockets[boxes[mailHdr.to].SearchByRemote(pktHdr.from,mailHdr.from)]->confirm = true;
				boxes[mailHdr.to].Sockets[boxes[mailHdr.to].SearchByRemote(pktHdr.from,mailHdr.from)]->confirm_id = mailHdr.id;
				break;
			}
			case ACK:
			{
				socket = boxes[mailHdr.to].Sockets[boxes[mailHdr.to].SearchByRemote(pktHdr.from,mailHdr.from)];

				if(mailHdr.id > socket->received_id)
				{
					socket->ack = true;
					socket->received_id ++;
				}
				break;
			}
			case REQUEST:
			{
				if(IsListening(mailHdr.to))
				{
					boxes[mailHdr.to].PutRequest(pktHdr, mailHdr, buffer + sizeof(MailHeader)); // we send the request to the listener
				}
				break;
			}
			case MESSAGE:
			{
				socket = boxes[mailHdr.to].Sockets[boxes[mailHdr.to].SearchByRemote(pktHdr.from,mailHdr.from)];
				if(mailHdr.id > socket->received_id)
				{
					boxes[mailHdr.to].Put(boxes[mailHdr.to].SearchByRemote(pktHdr.from,mailHdr.from),
					pktHdr, mailHdr, buffer + sizeof(MailHeader));
					socket->received_id ++;
				}
				break;
			}
			default:
			{
				//unknown message type
				ASSERT(FALSE);
				break;
			}
		}

		if(mailHdr.mailType != CONFIRMATION)
		{
			conf_pktHdr.to = pktHdr.from;
			conf_pktHdr.from = pktHdr.to;
			conf_pktHdr.length = sizeof(MailHeader);

			conf_mailHdr.to = mailHdr.from;
			conf_mailHdr.from = mailHdr.to;
			conf_mailHdr.mailType = CONFIRMATION;
			conf_mailHdr.length = 0;
			conf_mailHdr.id = mailHdr.id;
			Send(conf_pktHdr,conf_mailHdr,NULL);
		}
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

int
PostOffice::Send(PacketHeader pktHdr, MailHeader mailHdr, const char* data)
{
    char* buffer = new char[MaxPacketSize]();	// space to hold concatenated
						// mailHdr + data

    if (DebugIsEnabled('n'))
	{
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
    int return_value = network->Send(pktHdr, buffer);
    messageSent->P();			// wait for interrupt to tell us
					// ok to send the next message
    sendLock->Release();

    delete [] buffer;			// we've sent the message, so
					// we can delete our buffer
	return return_value;
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
int PostOffice::EnableListening(int i_local_port, NachosSocket *socket)
{
	if(i_local_port<0 || i_local_port>=numBoxes)
	{
		return -1;
	}

	if(socket == NULL)
	{
		return -2;
	}

	boxes[i_local_port].Listener = socket;
	return 0;
}

int PostOffice::ReserveSlot(NachosSocket ***slot, int mailbox, int remote_machine, int remote_port) //reserve a socket slot in the mailbox
{
	MailBox *box = &boxes[mailbox];
	NachosSocket** free_slot = NULL;
	int i;
	
	if(mailbox >= numBoxes)
	{
			return -3;
	}
	
	for(i=0;i<NB_BOX;i++)
	{
		if(box->Sockets[i] != NULL)
		{		
			if((box->Sockets[i]->RemoteMachine() == remote_machine) && (box->Sockets[i]->RemotePort() == remote_port))
			{
				return -2;	//already present
			}
		}
		else
		{
			free_slot = &(box->Sockets[i]);
		}
	}
	if(free_slot != NULL)
	{
		*slot = free_slot;
		//*free_slot = **slot;
		return 0;
	}

	return -1; // Not enough space left
}
// assume the port number is OK
bool PostOffice::IsListening(int i_local_port)
{
	ASSERT( (i_local_port >= 0) && (i_local_port < numBoxes) );

	return (boxes[i_local_port].Listener != NULL); // listener != NULL
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
	confirm = false;
	ack = false;
	messages = new SynchList();

	mail_id_counter = 1;
	received_id = 0;

	reception_buffer = new char[MaxMailSize];
	buffer_length = 0;
}

NachosSocket::~NachosSocket()
{
	delete messages;
	delete reception_buffer;
}

/**
 * Receive : read a message of at most size bytes from the socket 
 * and write it inside the buffer given as argument. If it can
 * not read full size, the actual readen size will be returned.
 * Return 0 if there is nothing to read, -1 if the socket is closed
 * -2 if the socket is waiting for connection
 */
int NachosSocket::Receive(char *buffer, unsigned int size)
{	
	if(status != SOCKET_CONNECTED)
	{
		return -1;
	}

	unsigned int received_size = 0;

	//firstly pick bytes in the buffer if it is not empty
	if(buffer_length > 0)
	{
		//we take all the buffer
		if(buffer_length <= size)
		{
			bcopy(reception_buffer, buffer, buffer_length);
			received_size += buffer_length;
			buffer_length = 0;
		}
		//we only take a part of the buffer
		else
		{
			bcopy(reception_buffer, buffer, size);
			received_size += size;
			buffer_length -= size;
			//replace the buffer content to the beggining of the buffer
			memmove(reception_buffer, reception_buffer + size, MaxMailSize - size);
		}
	}

	if(received_size == size)
	{
		return received_size;
	}

	//then if we still need bytes, pick a new mail
	Mail* mail;
	/*if(mail_counter != 0)
	{*/
		mail = PickAMail();
	/*}
	else
	{
		return 	
	}*/

	while(mail != NULL && received_size < size)
	{
		//take all the mail
		if(mail->mailHdr.length <= size-received_size)
		{
			//TODO just see if the arithmetic pointer is ok here when testing
			bcopy(mail->data, (void*)(buffer+received_size), mail->mailHdr.length);
			received_size += mail->mailHdr.length;
		}
		//take a part of the mail and put the rest in the socket buffer
		else
		{
			bcopy(mail->data, (void*)(buffer+received_size), size-received_size);
			
			//copy the rest in the buffer (we know it is empty if we are here)
			bcopy((mail->data)+(size-received_size), reception_buffer, mail->mailHdr.length-(size-received_size));
			buffer_length = mail->mailHdr.length-(size-received_size);
			received_size = size;
		}
		delete mail;

		//take a new mail if more bytes needed
		if(received_size < size)
		{
			/*if(mail_counter != 0)
			{*/
				mail = PickAMail();
			/*}
			else
			{
				mail = NULL;
			}*/
		}
	}

	return received_size;
}
//------------------------------------------------------------------------//
int NachosSocket::SendRequest()
{
	MailHeader mailHdr; //= new MailHeader();
	mailHdr.mailType = REQUEST;
    mailHdr.to = remote_port;
    mailHdr.from = local_port;
    mailHdr.length = 0;

	PacketHeader packetHdr;
	packetHdr.to = remote_machine;

	return Send(packetHdr, mailHdr, NULL);
}
//------------------------------------------------------------------------//
int NachosSocket::SendAck()
{
	MailHeader mailHdr; //= new MailHeader();
	mailHdr.mailType = ACK;
    mailHdr.to = remote_port;
    mailHdr.from = local_port;
    mailHdr.length = 0;

	PacketHeader packetHdr;
	packetHdr.to = remote_machine;
	
	return Send(packetHdr, mailHdr, NULL);
}
//------------------------------------------------------------------------//
int NachosSocket::SendMail(char* buffer,unsigned int size)
{
	 
	MailHeader mailHdr;
	mailHdr.mailType = MESSAGE;
    mailHdr.to = remote_port;
    mailHdr.from = local_port;
    

	PacketHeader packetHdr;
	packetHdr.to = remote_machine;

	mailHdr.length = MaxMailSize;
	while(size > MaxMailSize)
	{
		if(Send(packetHdr, mailHdr, buffer)<0)
		{
			return -1;
		}
			
		buffer+=MaxMailSize;
		size-=MaxMailSize;
	}
	if(size > 0)
	{
		mailHdr.length = size;
		if(Send(packetHdr, mailHdr, buffer)<0)
		{
			return -1;
		}
	}
	return 0;
}
//------------------------------------------------------------------------//
int NachosSocket::Send(PacketHeader packetHdr,MailHeader mailHdr, char* data)
{
	long long int startTick;
	int i=0;

	//TODO use the id counter in a method with lock
	if(mailHdr.mailType != REQUEST)
	{
		mailHdr.id = mail_id_counter;
		mail_id_counter++;
	}
	else
	{
		mailHdr.id = 0;
	}

	while (!confirm && i<NB_REEMISSION)
	{
		startTick = stats->totalTicks;
		if(postOffice->Send(packetHdr, mailHdr, data) < 0)
		{
			//socket closed
			return -2;
		}
		while((!confirm || confirm_id!=mailHdr.id) && stats->totalTicks < startTick+SEND_TIMEOUT)
		{
			currentThread->Yield();			
		}
		i++;
	}
	if(!confirm || confirm_id!=mailHdr.id)
	{
		//timeout reached
		return -1;
	}
	confirm = false;
	return 0;							// Success Sending Mail
	
}
//------------------------------------------------------------------------//
int NachosSocket::WaitTimeoutAck()
{
	long long int startTick = stats->totalTicks;

	while(!ack && stats->totalTicks<startTick+TIMEOUT)
	{
		currentThread->Yield();			
	}
	if(!ack)
	{
		return -1;						//Send Timeout
	}
	return 0;
}
//------------------------------------------------------------------------//
Mail* NachosSocket::PickAMail()
{
	return ((Mail*)messages->Remove());
}
//------------------------------------------------------------------------//
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
//------------------------------------------------------------------------//
void NachosSocket::SetStatus(SocketStatusEnum new_status)
{
	status = new_status;
}
//------------------------------------------------------------------------//
int NachosSocket::LocalPort()
{
	return local_port;
}
//------------------------------------------------------------------------//
bool NachosSocket::IsListening()
{
	return (status == SOCKET_LISTENING);
}
//------------------------------------------------------------------------//
int NachosSocket::RemotePort()
{
	return remote_port;
}
//------------------------------------------------------------------------//
int NachosSocket::RemoteMachine()
{
	return remote_machine;
}
//------------------------------------------------------------------------//
