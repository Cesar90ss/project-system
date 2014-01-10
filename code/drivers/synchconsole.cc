#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "utility.h"
#include <cmath>
#include <cctype>

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
    monitor = new Semaphore("SynchConsole internal monitor", 1);
}

SynchConsole::~SynchConsole()
{
    delete console;
    delete SynchConsole_writeDone;
    delete SynchConsole_readAvail;
    delete monitor;
}


void SynchConsole::__PutChar(const char ch)
{
    console->PutChar(ch);
    //wait for the end of the writing
    SynchConsole_writeDone->P();
}

void SynchConsole::SynchPutChar(const char ch)
{
    monitor->P();
    __PutChar(ch);
    monitor->V();
}

int SynchConsole::__GetChar()
{
    //wait for avalaible character
    SynchConsole_readAvail->P();
    return console->GetChar();
}

int SynchConsole::SynchGetChar()
{
    monitor->P();
    int res = __GetChar();
    monitor->V();
    return res;
}

void SynchConsole::SynchPutString(const char s[])
{
    monitor->P();

    int i = 0;
    //simulate strlen
    while (s[i] != '\0')
        __PutChar(s[i++]);

    monitor->V();
}

char *SynchConsole::SynchGetString(char *s, int n)
{
    monitor->P();

    int i;
    
    //try to get n-1 characters
    for(i = 0; i < n - 1; i++)
    {
        s[i] = __GetChar();
        if (console->isEOF())
        {
            s[i] = '\0';
            monitor->V();
            return i == 0 ? NULL : s;
        }
    }

    //add end of string
    s[i] = '\0';
    monitor->V();
    return s;
}

#define MAX_INT_NUM 11

void SynchConsole::SynchPutInt(int i)
{
    char *ToBeWritten = new char[MAX_INT_NUM + 1];
    sprintf(ToBeWritten, "%i", i);
    SynchPutString(ToBeWritten);
}

int SynchConsole::SynchGetInt(int* p)
{
    monitor->P();

    int c = __GetChar();
    char buf[MAX_STRING_SIZE + 1];
    int count = 0;
    if(c==EOF){
        monitor->V();
        return EOF;
    }

    // get string up to space
    while (count < MAX_STRING_SIZE && c!=EOF && !isspace((char)c))
    {
        buf[count++] = (char) c;
        c = __GetChar();
    }

    int ret = sscanf(buf, "%d", p);
    monitor->V();
    return ret;
}
