#include "syscall.h"
#define NB_THREADS 251

void* fun()
{

  int i;
  for(i=0; i<2;i ++)
  {
      PutInt(i);
  }

  for (i= 0; i < 10000; i++);
  return 0;
}

int main()
{
	int id[NB_THREADS];

    int i;
    for(i=0;i<NB_THREADS;i++)
    {
        id[i] = UserThreadCreate(&fun, 0);
        //Some check for the threads creation
        if(id[i] < 0)
        {
            PutString("Error while creating thread\n");
            PutInt(i);
            PutChar('\n');
            Halt();
        }
	}

	for(i=0;i<NB_THREADS;i++)
	{
        //Joining all threads
		if(UserThreadJoin(id[i], 0) == -1)
		{
			PutString(" joined failed! ");
		}
		else
		{
			PutString(" joined successfull! ");
		}
	}

	PutString("Parent finish\n");
	return 0;
}
