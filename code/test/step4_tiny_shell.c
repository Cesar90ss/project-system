#include "libc.c"
#define MAX_NB_JOBS 10

typedef struct{
    int p;
    char name[30];
    int pid;
} job_t;

job_t Job_Array[MAX_NB_JOBS];

void fg(int id)
{
    if(Job_Array[id].p && ( !CheckEnd(Job_Array[id].pid) ) )
    {
        Waitpid(Job_Array[id].pid, NULL);
        Job_Array[id].p=0;
    }
    else
    {
        PutString("fg : no such job\n");
    }
}

int first_free()
{
    int i = 0;
    while( Job_Array[i].p && ((i++) < MAX_NB_JOBS) );
    if (i < MAX_NB_JOBS)
    {
        return i;
    }

    return -1;
}

void jobs()
{
    int i;
    for(i=0;i<MAX_NB_JOBS; i++)
    {
        if(Job_Array[i].p == 1)
        {
            PutInt(i);
            PutString(" : ");
            PutString(Job_Array[i].name);
            PutChar('\n');
        }
    }
}

void clean_zombies()
{
    int i = 0;
    for(i=0;i<MAX_NB_JOBS;i++)
    {
        if( (Job_Array[i].p) && CheckEnd(Job_Array[i].pid))
        {
            Job_Array[i].p = 0;
        }
    }
}

int
main ()
{
    int newProc;
    char prompt[3], buffer[70];
    int i;
    int num;
    for(i = 0; i<  MAX_NB_JOBS; i++)
    {
        Job_Array[i].p = 0;
    }

    prompt[0] = '-';
    prompt[1] = '-';
    prompt[2] =	'\0';

    while (1)
    {
        PutString(prompt);

        i = 0;

        do
        {
            int c;
            c = GetChar();

            if (c == -1)
            {
                // Wait for PutString to end
                for (c = 0; c < 10000; c++);

                Exit(0);
            }

            buffer[i] = (char)c;
        }
        while (buffer[i++] != '\n');

        buffer[--i] = '\0';

        if (i > 0)
        {
            if (strncmp(buffer,"fg",2)) //fg command
            {
                if(i>3)
                {
                    fg( Int_Of_Char(buffer[3]) );
                }
                else
                {
                    PutString("fg without a job number, error\n");
                }
            }
            else
            {
                if (strncmp(buffer,"jobs",4)) //jobs command
                {
                    jobs();
                }
                else
                {
                    if(buffer[i-1] == '&') //background launch
                    {
                        num = first_free();
                        if(num >= 0)
                        {
                            buffer[i-2] = '\0';
                            memcpy(Job_Array[num].name,buffer,i);
                            Job_Array[num].p = 1;
                            newProc = ForkExec(buffer);
                            Job_Array[num].pid = newProc;
                            PutString("The Job was launched in background with id : ");
                            PutInt(num);
                            PutChar('\n');
                        }
                        else
                        {
                            PutString("Can not launch any more background job\n");
                        }
                    }
                    else	//foreground launch
                    {
                        newProc = ForkExec(buffer);
                        if( newProc > 0)
                        {
                            Waitpid(newProc, 0);
                        }
                        else
                            PutString("The job could not be launched\n");
                    }
                }
            }
        }
    }
}
