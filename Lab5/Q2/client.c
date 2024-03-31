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

//fn btupdate el shared memory be el input
//3shan b3d kda el server yb2a y3mlo update
void updatememory(void *shmaddr , int shmid, char *input)
{
  shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (void *)-1)
    {
      perror("Error in attach in adding");
      exit(-1);
    }
    strcpy((char *)shmaddr, input);
}

//fn to read from memory b3d ma el server 3amlha update
//we tprint el value deh
void readmemory(void *shmaddr , int shmid)
{
    shmaddr = shmat(shmid, (void *)0, 0);
    if (shmaddr == (void *)-1)
    {
      perror("Error in attach in recieving");
      exit(-1);
    }
    printf("\nMessage received: %s\n", (char *)shmaddr);
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
  union Semun semun2;

  int sem1 = semget(key_id, 1, 0666 | IPC_CREAT);
  int sem2 = semget(key_id, 1, 0666 | IPC_CREAT);
  int sem3 = semget(key_id, 1, 0666 | IPC_CREAT);

  if (sem1 == -1 || sem2 == -1 || sem3 == -1) 
  {
    perror("Error in create sem");
    exit(-1);
  }

  semun.val = 0; /* initial value of the semaphore, Binary semaphore */
  semun2.val = 1; //3shan sem 3 hatb2a 1 msh zero
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
  if (semctl(sem3, 0, SETVAL, semun2) == -1)
  {
    perror("Error in semctl");
    exit(-1);
  }

  ///////////////////////////////////sending and recieving////////////////////////////////
  void *shmaddr;
  while (1)//loop to infinty
  {
    ///////sem///////
    down(sem3);

    ///////read input from user/////// 
    char input[257];
    fgets(input, 257, stdin); 

    ///////update memory///////
    updatememory(shmaddr,shmid,input);

    ///////sem///////
    up(sem2);
    down(sem1);

    ///////read memory///////
    readmemory(shmaddr,shmid);

    ///////sem///////
    up(sem3);
    
  }

  return 0;
}


/*          code flow

  client:                                       server:

  down(s3)                
  take input then
  update memory                                  down(s2) 

  up(s2)                                         read memory                          
  down(s1)                                       update memory

  read memory                                    up(s1)
  up(s3)

*/

//down (s2) 3shan msh hykhosh ela lma el client yb2a 3ml update
//down (s1) 3shan msh hykhosh el lma el server yb2a 3ml update  
//down (s3) 3shan el critical section msh hyb2a feh 2 clients y3mlo update fe wa2t wa7ed
//note en semohore 3 = 1 we el tanyen = 0
//el read we el update memory fe el server m3mloen fe fn wahda