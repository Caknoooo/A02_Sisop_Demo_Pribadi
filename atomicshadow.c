#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
  int fd1[2];
  int fd2[2];

  char shadow_msg[] = "I am shadow";
  char atomic_msg[] = "I am atomic";

  char read_buffer[100];

  pid_t pid;

  if (pipe(fd1) == -1)
  {
    fprintf(stderr, "Pipe Failed");
    return 1;
  }
  if (pipe(fd2) == -1)
  {
    fprintf(stderr, "Pipe Failed");
    return 1;
  }

  // Create child process
  pid = fork();

  if (pid < 0)
  {
    fprintf(stderr, "fork Failed");
    return 1;
  }

  // Child process
  if (pid == 0)
  {
    // Close reading end of pipe 1
    close(fd1[0]);

    // Write shadow message to pipe 1
    write(fd1[1], shadow_msg, strlen(shadow_msg) + 1);

    // Close writing end of pipe 1
    close(fd1[1]);

    // Read atomic message from pipe 2
    read(fd2[0], read_buffer, sizeof(read_buffer));

    // Print atomic message
    printf("%s\n", read_buffer);

    // Close reading end of pipe 2
    close(fd2[0]);

    exit(0);
  }

  // Parent process
  else
  {
    // Close writing end of pipe 1
    close(fd1[1]);

    // Read shadow message from pipe 1
    read(fd1[0], read_buffer, sizeof(read_buffer));

    // Print shadow message
    printf("%s\n", read_buffer);

    // Close reading end of pipe 1
    close(fd1[0]);

    // Close reading end of pipe 2
    close(fd2[0]);

    // Write atomic message to pipe 2
    write(fd2[1], atomic_msg, strlen(atomic_msg) + 1);

    // Close writing end of pipe 2
    close(fd2[1]);
  }

  return 0;
}