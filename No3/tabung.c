
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE sizeof(double)

int main()
{
  int shmid;
  key_t key = 1234;
  double *luas;
  const double pi = 3.14159;
  const int r = 7;
  const int t = 10;

  // Locate the shared memory segment
  shmid = shmget(key, SHM_SIZE, 0666);
  if (shmid < 0)
  {
    perror("shmget");
    exit(1);
  }

  // Attach to the shared memory segment
  luas = (double *)shmat(shmid, NULL, 0);
  if (luas == (double *)-1)
  {
    perror("shmat");
    exit(1);
  }

  // second shared memory variable
  int shmid2 = shmget(key + 1, SHM_SIZE, IPC_CREAT | 0666);
  if (shmid2 < 0)
  {
    perror("shmget");
    exit(1);
  }

  double *luas_permukaan;
  luas_permukaan = (double *)shmat(shmid2, NULL, 0);
  if (luas_permukaan == (double *)-1)
  {
    perror("shmat");
    exit(1);
  }

  // menghitung luas permukaan tabung
  *luas_permukaan = (2 * pi * r * t) + (2 * *luas);
  printf("Luas permukaan tabung dengan jari-jari %d dan tinggi %d adalah %.2f\n", r, t, *luas_permukaan);

  if (shmdt(luas) < 0)
  {
    perror("shmdt");
    exit(1);
  }

  if (shmdt(luas_permukaan) < 0)
  {
    perror("shmdt");
    exit(1);
  }

  return 0;
}