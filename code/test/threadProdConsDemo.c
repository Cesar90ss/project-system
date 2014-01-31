#include "syscall.h"
#define BUFFER_SIZE 3
#define NB_TRANSACTIONS 8
#define NB_PRODUCER 5
#define NB_CONSUMER 5

//---------------------//
int buffer[BUFFER_SIZE];
int n = 0;
int first_empty = 0;

//semaphores
int prod, cons, critical_section;
//---------------------//
void produce(int x, int tid)
{
	UserSemaphoreP(prod);
		UserSemaphoreP(critical_section);
			PutString("Producer ");
			PutInt(tid);
			PutString(" produce item ");
			PutInt(x);
			PutChar('\n');
			buffer[first_empty] = x;
			first_empty = (first_empty+1)%BUFFER_SIZE;
			n++;
		UserSemaphoreV(critical_section);
	UserSemaphoreV(cons);
}
//---------------------//
void consume(int tid)
{
	UserSemaphoreP(cons);
		UserSemaphoreP(critical_section);
			PutString("Consumer ");
			PutInt(tid);
			PutString(" consume item ");
			PutInt(buffer[(BUFFER_SIZE+first_empty-n) % BUFFER_SIZE]);
			PutChar('\n');
			n--;
		UserSemaphoreV(critical_section);
	UserSemaphoreV(prod);
}
//---------------------//
void *producer(void* arg)
{
	int tid = (int)arg;
	int i;
	for(i=0;i<NB_TRANSACTIONS;i++)
	{
		produce(i+10*(int)arg,tid);
	}

	return 0;
}
//---------------------//
void *consumer(void* arg)
{
	int tid = (int)arg;
	int i;
	for(i=0;i<NB_TRANSACTIONS;i++)
	{
		consume(tid);
	}
	return 0;
}
//---------------------//
int main()
{
	prod = UserSemaphoreCreate("prod",BUFFER_SIZE);
	cons = UserSemaphoreCreate("cons",0);
	critical_section = UserSemaphoreCreate("crit",1);

	int tid[NB_PRODUCER+NB_CONSUMER];

	int i,j;
	//create producers
	for(i=0;i<NB_PRODUCER;i++)
	{
		tid[i] = UserThreadCreate(&producer, (void*)i);
	}
	//create consumers
	for(j=0;j<NB_CONSUMER;j++)
	{
		tid[i+j] = UserThreadCreate(&consumer, 0);
	}

	//join producers and consumers
	for(i=0;i<NB_PRODUCER+NB_CONSUMER;i++)
	{
		UserThreadJoin(tid[i],0);
	}
	return 0;
}
