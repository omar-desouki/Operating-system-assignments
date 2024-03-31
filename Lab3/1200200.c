#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h> 


int main(int argc, char* argv[])
{
    FILE *file;
    char *filename = argv[1];
    int numberofta = atoi(argv[2]);
    int mingrade = atoi(argv[3]);
    file = fopen(filename,"r");
    char line[1000];
    

    if (file == NULL)
    {
       printf("file is empty\n");
        return -1;
    }

    int numberofstudents;
    int num1;
    int num2;
    
    
    fscanf(file,"%d",&numberofstudents); //saves the first line in numbersofstudents
    //printf("%d \n",numberofstudents);

    int grades[numberofstudents];
    int i=0;
    while (fscanf(file, "%d %d", &num1, &num2) == 2)//basave el sum bta3 el grades le kol student gwa el array
    {
        grades[i] = num1 + num2;
        //printf("%d \n",grades[i]);
        i++;
        
    }
    
    fclose(file);

    int n = floor(numberofstudents / numberofta); //each TA take n studetnts

    int numberofstudents_perta[numberofta]; //arr bindex el ta 3shan a3rf lw feh zyada akher ta yakhdhom
    int temp =numberofstudents;

    for (int i =0; i < numberofta ; i++) 
    {
        if(i == numberofta-1)
        {
            numberofstudents_perta[i] = temp;
        }
        else
        {
            numberofstudents_perta[i] = n;
            temp = temp - n;
        }
    } //b3d el loop b2a numberofstudents_perta stored gwa kol index bta3ha kol ta mafrod yakhod kam student

        // for (int i =0; i < numberofta ; i++) //test
        // {
        //     printf("%d \n",numberofstudents_perta[i]);
        // }

    int child_pid;

    for (int i = 0; i < numberofta; i++) 
    {
        child_pid = fork();

        if (child_pid == -1)
        {
            perror("Fork failed");
            return 1;
        }
        
        int loopsbefore = 0;
        int count=0;

        if (child_pid == 0) //ana gwa el child process
        {
            //printf("Child %d (PID: %d)\n", i, getpid());
            int numberofstudentsforthista = numberofstudents_perta[i]; //kam student 3andy

            //ezbot el loopsbefore 3shan fe el grades my3dsh mn awel we gded
            for(int k=0 ; k < i ; k++)
            {
                loopsbefore +=  numberofstudents_perta[k];
            }

            //eh el grades bta3thom
            for(int j=0 ; j < numberofstudentsforthista ; j++)
            {
                //printf("%d \n",grades[j+loopsbefore]); //test
                if(grades[j+loopsbefore] > mingrade)
                {
                    count++;
                }
            }
            printf("%d ",count);

            return 0; // Child process exits
        }
    }

    return 0;
}