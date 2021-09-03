#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void main() {
  int pid_des_Kindes;

  pid_des_Kindes = fork();

  // Es kam zu einem Fehler --> Programmabbruch
  if (pid_des_Kindes < 0) {
    printf("\n Es kam beim fork zu einem Fehler!");
    exit(1);
  }

  // Elternprozess
  if (pid_des_Kindes > 0) {
    printf("\n Elternprozess: PID: %i", getpid());
    printf("\n Elternprozess: PPID: %i \n", getppid());
  }

  // Kindprozess
  if (pid_des_Kindes == 0) {
    printf("\n Kindprozess: PID: %i", getpid());
    printf("\n Kindprozess: PPID: %i \n", getppid());
  }
}
