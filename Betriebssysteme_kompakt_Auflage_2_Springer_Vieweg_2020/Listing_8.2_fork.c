#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void main() {
  int i;
  if (fork()) {
    // Hier arbeitet der Vaterprozess
    for (i = 0; i < 5000000; i++) {
      printf("\n Vater: %i", i);
    }
  } else {
    // Hier arbeitet der Kindprozess
    for (i = 0; i < 5000000; i++) {
      printf("\n Kind : %i", i);
    }
  }
}

