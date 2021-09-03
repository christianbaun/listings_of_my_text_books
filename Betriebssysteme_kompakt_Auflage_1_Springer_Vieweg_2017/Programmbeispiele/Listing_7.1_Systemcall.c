#include <syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main(void) {
  unsigned int ID1, ID2;

  // Systemaufruf
  ID1 = syscall(SYS_getpid);
  printf ("Das Ergebnis des Systemaufrufs ist: %d\n", ID1);

  // Von der glibc aufgerufener Systemaufruf
  ID2 = getpid();
  printf ("Das Ergebnis der Bibliotheksfunktion ist: %d\n", ID2);

  return(0);
}

