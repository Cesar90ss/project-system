#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

//declare static to use under C function
static Semaphore *SynchConsole_readAvail;
static Semaphore *SynchConsole_writeDone;

static void SynchConsole_ReadAvail(int arg) { SynchConsole_readAvail->V(); }
static void SynchConsole_WriteDone(int arg) { SynchConsole_writeDone->V(); }

//init semaphore and bind SynchConsole_ReadAvail and SynchConsole_WriteDone to console object
SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
    SynchConsole_readAvail = new Semaphore("SynchConsole_read avail", 0);
    SynchConsole_writeDone = new Semaphore("SynchConsole_write done", 0);
    console = new Console(readFile, writeFile, SynchConsole_ReadAvail, SynchConsole_WriteDone, 0);
}

SynchConsole::~SynchConsole()
{
    delete console;
    delete SynchConsole_writeDone;
    delete SynchConsole_readAvail;
}

void SynchConsole::SynchPutChar(const char ch)
{
    console->PutChar(ch);
    //wait for the end of the writing
    SynchConsole_writeDone->P();
}

char SynchConsole::SynchGetChar()
{
    //wait for avalaible character
    SynchConsole_readAvail->P();
    return console->GetChar();
}

void SynchConsole::SynchPutString(const char s[])
{
    int i = 0;
    //simulate strlen
    while (s[i] != '\0')
        SynchPutChar(s[i++]);
}

void SynchConsole::SynchGetString(char *s, int n)
{
    int i;
    //try to get n-1 characters
    for(i = 0; i < n - 1; i++)
    {
        s[i] = SynchGetChar();
        if (s[i] == EOF)
            break;
    }

    //add end of string
    s[i] = '\0';
}
