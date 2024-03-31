#include <stdio.h>
#include <string.h>
#include <ctype.h>


int main(int argc , char*argv[])
{

    if(argc == 3) //check enhom 3 arguments y3ny 2 inputs
    {
    char firststr[21];
    char secondstr[21];

    strcpy(firststr,argv[1]); //bahothom fe char arr 3shan aloop 3alehom bra7ty
    strcpy(secondstr,argv[2]);

    int length1 = strlen(firststr); //store el lengthes
    int length2 = strlen(secondstr);
    
    //bakhly el etnen lowercase
    char* ch = firststr;
    while (*ch != '\0')
    {
        *ch = tolower(*ch);
        ch++;
    }
    ch = secondstr;
    while (*ch != '\0')
    {
        *ch = tolower(*ch);
        ch++;
    }
    ///////////


    int j=0;

    //part 3shan lw string el tany hwa el atwal we el mafrod el awel yb2a hwa ely goz2 mno
    char temp[21];
    int tempint;
    if (length2>length1)
    {
        strcpy(firststr,temp);
        strcpy(firststr,secondstr);
        strcpy(secondstr,temp);
        length1 = tempint;
        length1 = length2;
        length2 = tempint;

    }
    ////////////////////////////

    for (int i=0; i < length1 ; i++ ) //checkk (hna 3altol b3tber el tany hwa ely substring 3shan kda ba2lb fo2 lw el 3aks)
    {
        if(secondstr[j] == firststr[i])
        {
            j++;
        }


    }
    if(j==length2 && length1!=length2) // batcheck eno dakhl ad tol tany string we anhom msh nafs el string
    {
        printf("1\n");
    }
    else
    {
        printf("0\n");
    }

    }
    else
    {
        printf("inputs are wrong\n");
    }
   

    return 0;
}