#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

struct msgbuff
{
    long mtype;
    char mtext[257];
};

int main(int argc, char *argv[])
{
    while (1)
    {
        //////////////////////////////sending el input////////////////////
        char input[257];
        // strcpy(input, argv[1]); //take el input
        fgets(input, 257, stdin); // read input from user
        int last_four_digits = getpid() % 10000;

        key_t key_id;
        int msgq_id, send_val;

        key_id = ftok("key", 1);
        msgq_id = msgget(key_id, 0666 | IPC_CREAT);

        if (msgq_id == -1)
        {
            perror("Error in create");
            exit(-1);
        }

        struct msgbuff message;

        message.mtype = last_four_digits;
        strcpy(message.mtext, input);

        send_val = msgsnd(msgq_id, &message, sizeof(message.mtext), !IPC_NOWAIT);

        if (send_val == -1)
            perror("Errror in send");

        //////////////////////////////reciving el msg////////////////////////////
        key_t key_id2;
        int rec_val2, msgq_id2;

        key_id2 = ftok("key", 2);                     // create unique key
        msgq_id2 = msgget(key_id2, 0666 | IPC_CREAT); // create message queue and return id

        if (msgq_id2 == -1)
        {
            perror("Error in create");
            exit(-1);
        }
        // mabyt3mlsh recieve lel msg ela lw el type bta3 nafs el four digits bta3ty
        rec_val2 = msgrcv(msgq_id2, &message, sizeof(message.mtext), last_four_digits, !IPC_NOWAIT);

        if (rec_val2 == -1)
            perror("Error in receive");
        else
            printf("\nMessage received: %s\n", message.mtext);
    }

    return 0;
}