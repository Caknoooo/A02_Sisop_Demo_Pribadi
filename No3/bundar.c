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
  const double pi = 3.14;
  const int r = 7;

  // Create a shared memory segment
  shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
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

  // menghitung luas lingkaran
  *luas = pi * r * r;
  printf("Luas lingkaran dengan jari-jari %d adalah %.2f\n", r, *luas);
  sleep(5);

  // Second shared variable
  int shmid2 = shmget(key + 1, SHM_SIZE, 0666);
  if (shmid < 0)
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

  double total_harga;
  total_harga = *luas_permukaan * 25;
  printf("Total harga cat adalah %.2f\n", total_harga);

  // Detach from the shared memory segment
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