#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *SynchConsole_readAvail;
static Semaphore *SynchConsole_writeDone;

static void SynchConsole_ReadAvail(int arg) { SynchConsole_readAvail->V(); }
static void SynchConsole_WriteDone(int arg) { SynchConsole_writeDone->V(); }

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
    SynchConsole_writeDone->P(); //wait for the end of the writing
}


char SynchConsole::SynchGetChar()
{
    SynchConsole_readAvail->P();
    return console->GetChar();
}

void SynchConsole::SynchPutString(const char s[])
{
    int i = 0;

    while (s[i] != '\0')
        SynchPutChar(s[i++]);
}

void SynchConsole::SynchGetString(char *s, int n)
{
    int i;
    for(i = 0; i < n - 1; i++)
    {
        s[i] = SynchGetChar();
        if (s[i] == EOF)
            break;

    }

    s[i] = '\0';
}
