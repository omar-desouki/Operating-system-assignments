#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <ctype.h>

/* arg for semctl system calls. */
union Semun
{
  int val;               /* Value for SETVAL */
  struct semid_ds *buf;  /* Buffer for IPC_STAT, IPC_SET */
  unsigned short *array; /* Array for GETALL, SETALL */
  struct seminfo *__buf; /* Buffer for IPC_INFO (Linux-specific) */
};

void down(int sem)
{
  struct sembuf op;

  op.sem_num = 0;
  op.sem_op = -1;
  op.sem_flg = !IPC_NOWAIT;

  if (semop(sem, &op, 1) == -1)
  {
    perror("Error in down()");
    exit(-1);
  }
}

void up(int sem)
{
  struct sembuf op;

  op.sem_num = 0;
  op.sem_op = 1;
  op.sem_flg = !IPC_NOWAIT;

  if (semop(sem, &op, 1) == -1)
  {
    perror("Error in up()");
    exit(-1);
  }
}

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

//fn btread ely fe el memory btsaveha fe el recieved
//btakhod el recieved t3mlo conv
//we tupdate el memory be el recieved ely 3mltlo conv
void read_and_update_memory(void *shmaddr, int shmid) 
{
  shmaddr = shmat(shmid, (void *)0, 0);
  if (shmaddr == (void *)-1)
  {
    perror("Error in attach in recieving");
    exit(-1);
  }
  char recieved[257];
  strcpy(recieved, (char *)shmaddr); // hot el msg fe el recieved

  ///////change el msg///////
  conv(recieved, sizeof(recieved)); // conv el message

  shmaddr = shmat(shmid, NULL, 0);
  if (shmaddr == (void *)-1)
  {
    perror("Error in attach in adding");
    exit(-1);
  }
  //hot el recieved msg b3d ma etzabtet fe el memory
  strcpy((char *)shmaddr, recieved); // update el msg
}

int main()
{
  ///////////////////////////////////shared memory creation/////////////////////////////////

  int shmid;
  key_t key_id;
  key_id = ftok("key", 1);                        // create unique key
  shmid = shmget(key_id, 4096, IPC_CREAT | 0666); // create or return el shmid

  if (shmid == -1)
  {
    perror("Error in create");
    exit(-1);
  }
  //printf("\nShared memory ID = %d\n", shmid);

  ///////////////////////////////////semaphore creation////////////////////////////////

  union Semun semun;

  int sem1 = semget(key_id, 1, 0666 | IPC_CREAT);
  int sem2 = semget(key_id, 1, 0666 | IPC_CREAT);

  if (sem1 == -1 || sem2 == -1)
  {
    perror("Error in create sem");
    exit(-1);
  }

  semun.val = 0; /* initial value of the semaphore, Binary semaphore */
  if (semctl(sem1, 0, SETVAL, semun) == -1)
  {
    perror("Error in semctl");
    exit(-1);
  }
  if (semctl(sem2, 0, SETVAL, semun) == -1)
  {
    perror("Error in semctl");
    exit(-1);
  }

  ///////////////////////////////////sending and recieving////////////////////////////////
  void *shmaddr;
  while (1) //loop to infinty
  {
    ///////down s2///////
    down(sem2);

    ///////read memory and update it///////
    read_and_update_memory(shmaddr, shmid);

    ///////up s1///////
    up(sem1);
  }

  return 0;
}



/*          code flow

  client:                                       server:
                               
  take input then
  update memory                                  down(s2) 

  up(s2)                                         read memory                          
  down(s1)                                       update memory

  read memory                                    up(s2)

*/

//down (s2) 3shan msh hykhosh ela lma el client yb2a 3ml update
//down (s1) 3shan msh hykhosh el lma el server yb2a 3ml update  
//el read we el update memory fe el server m3mloen fe fn wahda