#include "syscall.h"
#define NB_THREADS 21

void* fun()
{
  return 0;
}

int main()
{
    //Used to store treads id at creation
	int id[NB_THREADS];

    int j=0;
    int i;
    for(i=0 ; i < NB_THREADS ; i++)
    {
        id[i] = UserThreadCreate(&fun, 0);
        //Some check for the threads creation
        if(id[i] <= 0)
        {
            PutString("-2");
        }
        else{
            j++;
            PutString("0");
        }
	    UserThreadJoin(id[i], 0);
    }

    PutString("\n");
    PutInt(j);
    PutString("\n");
	return 0;
}
