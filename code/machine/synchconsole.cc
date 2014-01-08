#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

//static Semaphore *readAvail;
//static Semaphore *writeDone;

//static void ReadAvail(int arg) { readAvail->V(); }
//static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
  //readAvail = new Semaphore("read avail", 0);
  //writeDone = new Semaphore("write done", 0);
  console(readFile, writeFile, readAvail, writeDone, 0)
}

SynchConsole::~SynchConsole()
{
//delete console;
//delete writeDone;
//delete readAvail;

}

void SynchConsole::SynchPutChar(const char ch)
{
  console->PutChar(ch);
  writeDone->P(); //wait for the end of the writing
}


char SynchConsole::SynchGetChar()
{
  readAvail->P();
  return console->GetChar();
}

void SynchConsole::SynchPutString(const char s[])
{
// ...
}

void SynchConsole::SynchGetString(char *s, int n)
{
// ...
}
