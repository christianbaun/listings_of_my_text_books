#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void main() {
  int rueckgabewert = fork();

  if (rueckgabewert < 0) {
    // Rückgabewert von fork = negativ --> Fehler!
    // Speicher oder Prozesstabelle sind evtl. voll
    printf("Es kam zu einem Fehler.\n");
  }
  
  if (rueckgabewert > 0) {
    // Rückgabewert von fork = positiv --> Elternprozess
    // Rückgabewert = PID des neuen Kindprozesses
    printf("Hier ist der Elternprozess. Der Kindprozess hat die PID: %i\n", rueckgabewert);
  }
  
  if (rueckgabewert == 0) {
    // Rückgabewert von fork = 0 --> Kindprozess
    printf("Hier ist der Kindprozess.\n");
  }
}
