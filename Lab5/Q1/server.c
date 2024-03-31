#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

/* convert upper case to lower case or vise versa */
void conv(char *msg, int size)
{
    int i;
    for (i = 0; i < size; ++i)
        if (islower(msg[i]))
            msg[i] = toupper(msg[i]);
        else if (isupper(msg[i]))
            msg[i] = tolower(msg[i]);
}

struct msgbuff
{
    long mtype;
    char mtext[257];
};

void myHandler(int sign_num)
{
    // btrg3 el msg queue id
    key_t key_id;
    key_id = ftok("key", 1);
    int msgq_id = msgget(key_id, 0666 | IPC_CREAT);

    if (msgq_id == -1) // lw mafesh msg queue
    {
        perror("Error in create");
        exit(-1);
    }
    else // delete el msg queue lw magesh error
    {
        msgctl(msgq_id, IPC_RMID, NULL);
        exit(0);
    }
}

int main()
{
    signal(SIGINT, myHandler);

    while (1) // loop to infinty
    {
        key_t key_id;
        int rec_val, msgq_id;

        key_id = ftok("key", 1);                    // create unique key
        msgq_id = msgget(key_id, 0666 | IPC_CREAT); // create message queue and return id

        if (msgq_id == -1)
        {
            perror("Error in create");
            exit(-1);
        }

        struct msgbuff message;

        /* receive all types of messages */
        // we b3d kda byb3tha lel client wahed bs 3la hasab el last 4 digits bta3 el client elly 3ml el msg
        rec_val = msgrcv(msgq_id, &message, sizeof(message.mtext), 0, !IPC_NOWAIT);

        if (rec_val == -1)
            perror("Error in receive");

        conv(message.mtext, sizeof(message.mtext)); // albt el message

        ///////////////////////////////////send it tany lel client////////////////////////////////////
        int client_last_four_digits = message.mtype; // get the last 4 digits of the client pid of the msg retrieved
        key_t key_id2;
        int msgq_id2, send_val2;

        key_id2 = ftok("key", 2);
        msgq_id2 = msgget(key_id2, 0666 | IPC_CREAT);

        if (msgq_id2 == -1)
        {
            perror("Error in create");
            exit(-1);
        }

        send_val2 = msgsnd(msgq_id2, &message, sizeof(message.mtext), !IPC_NOWAIT);

        if (send_val2 == -1)
            perror("Errror in send");
    }
    return 0;
}
