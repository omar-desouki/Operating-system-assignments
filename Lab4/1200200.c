#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int searchvalue;
int child1, child2;

void myHandler(int sign_num) 
{
    int state;
    int terminatedpid = wait(&state);    

    if (terminatedpid == child1) 
    {
        printf("Child 1 :Value %d found at position %d", searchvalue, WEXITSTATUS(state));
    }
    else
    {
        printf("Child 2 :Value %d found at position %d \n", searchvalue, WEXITSTATUS(state));
    }

    printf("\n");
    killpg(getpgrp(), SIGKILL); // terminate el 3
}
 

int main(int argc, char *argv[])
{
    int state1 = 0;

    signal(SIGUSR1, myHandler);

    searchvalue = atoi(argv[1]);
    int arr[argc];
    int j = 0;

    for (int i = 2; i < argc; i++)
    {
        arr[j] = atoi(argv[i]);
        j++;
    }

    int middleindex = (argc - 2) / 2;
    //printf("%d \n", middleindex); test

    child1 = fork();

    if (child1 == -1)
    {
        perror("Fork failed");
        return 1;
    }
    else if (child1 == 0)
    { // first child process //mn 0 le el middle index
        printf("I am the first child, PID = %d, PPID = %d \n", getpid(), getppid());
        for (int i = 0; i < middleindex; i++)
        {
            if (arr[i] == searchvalue)
            {
                kill(getppid(), SIGUSR1);
                exit(i); 
            }
        }
        sleep(3);
        printf("child 1 terminates\n");
        exit(0);
    }
    else
    { // parent
        printf("I am the parent, PID = %d.\n", getpid());

        child2 = fork();
        if (child2 == -1)
        {
            perror("Fork failed");
            return 1;
        }
        else if (child2 == 0)
        { // second child process // mn el middle index le el el end (size of array)
            printf("I am the second child, PID = %d, PPID = %d \n", getpid(), getppid());

            for (int i = middleindex; i < argc - 2; i++)
            {
                if (arr[i] == searchvalue)
                {
                    kill(getppid(), SIGUSR1);
                    exit((i)); // eftrad bnbd2 mn index 0   
                }
            }
            sleep(3);
            printf("child 2 terminates \n");
            exit(0);
        }
        else
        { // parent
            sleep(5);
            // wait(&state1); //lazem?
            // wait(&state1); 
            printf("value not found\n");
        }
    }

    return 0;
}

//answer to the first non code question
//the parent is the one who executes the code in the handler. 
//because, the child that finds the value first sends the signal to the parent which executes the signal

//answer to the second non code question is because 
//the child sends the signal then exits 
//so, it will definitely recieve an exit code because when the parent recives the signal
//the child hasn't exited yet