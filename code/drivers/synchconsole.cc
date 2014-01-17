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
    monitorRead = new Semaphore("SynchConsole read internal monitor", 1);
    monitorWrite = new Semaphore("SynchConsole write internal monitor", 1);
}

SynchConsole::~SynchConsole()
{
    delete console;
    delete SynchConsole_writeDone;
    delete SynchConsole_readAvail;
    delete monitorRead;
    delete monitorWrite;
}


void SynchConsole::__PutChar(const char ch)
{
    console->PutChar(ch);
    //wait for the end of the writing
    SynchConsole_writeDone->P();
}

void SynchConsole::SynchPutChar(const char ch)
{
    monitorWrite->P();
    __PutChar(ch);
    monitorWrite->V();
}

int SynchConsole::__GetChar()
{
    //wait for avalaible character
    SynchConsole_readAvail->P();
    return console->GetChar();
}

int SynchConsole::SynchGetChar()
{
    monitorRead->P();
    int res = __GetChar();
    monitorRead->V();
    return res;
}

void SynchConsole::SynchPutString(const char s[])
{
    monitorWrite->P();

    int i = 0;
    //simulate strlen
    while (s[i] != '\0')
        __PutChar(s[i++]);

    monitorWrite->V();
}

char *SynchConsole::SynchGetString(char *s, int n)
{
    monitorRead->P();

    int i;

    //try to get n-1 characters
    for(i = 0; i < n - 1; i++)
    {
        s[i] = __GetChar();
        if (console->isEOF())
        {
            s[i] = '\0';
            monitorRead->V();
            return i == 0 ? NULL : s;
        }
    }

    //add end of string
    s[i] = '\0';
    monitorRead->V();
    return s;
}

#define MAX_INT_NUM 11

void SynchConsole::SynchPutInt(int i)
{
    char ToBeWritten[MAX_INT_NUM + 1];
    sprintf(ToBeWritten, "%i", i);
    SynchPutString(ToBeWritten);
}

int SynchConsole::SynchGetInt(int* p)
{
    monitorRead->P();

    int res = __GetChar();
    char c;
    char buf[MAX_STRING_SIZE + 1];
    int count = 0;

    if(res == EOF)
    {
        monitorRead->V();
        return EOF;
    }

    c = (char)res;

    // get string up to space
    while (count < MAX_STRING_SIZE && res != EOF && !isspace(c))
    {
        buf[count++] = c;
        res = __GetChar();
        c = (char)res;
    }

    buf[count] = '\0';

    int ret = sscanf(buf, "%d", p);
    monitorRead->V();
    return ret;
}
