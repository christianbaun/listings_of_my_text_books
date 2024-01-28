#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main () {
  int pid;
  // Einen Kindprozess erzeugen
  pid = fork();

  // Es kam zu einem Fehler --> Programmabbruch
  if (pid < 0) {
    printf("Es kam beim fork zu einem Fehler!\n");
    exit(1);
  }
  
  // Wenn die PID!=0 --> Elternprozess
  if (pid > 0) {
    printf("[Eltern] Eigene PID:     %d\n", getpid());
    printf("[Eltern] PID des Kindes: %d\n", pid);
  }
  
  // Wenn die PID=0 --> Kindprozess
  if (pid == 0) {
    printf("[Kind]   Eigene PID:     %d\n", getpid());
    printf("[Kind]   PID des Vaters: %d\n", getppid());

    // Aktuelles Programm durch "date" ersetzen
    // "date" wird der Prozessname in der Prozesstabelle
    execl("/bin/date", "date", "-u", NULL);
  }
  
  printf("[Eltern] Programmende.\n");
  return 0;
}
