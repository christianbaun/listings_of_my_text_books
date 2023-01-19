// title:        Listing_8_2_fork.c
// description:  This c program demonstrates that parent and child processes are
//               independent of each other
// author:       Dr. Christian Baun
// url:          http://www.christianbaun.de
// license:      GPLv2
// date:         January 17th 2023
// version:      1.0
// gcc_version:  gcc 10.2.1 (Debian 10.2.1-6)
// compile with: gcc Listing_8_2_fork.c -o Listing_8_2_fork
// nodes:        This program demonstrates that parent and child processes are always
//               independent of each other and have different memory areas. When the 
//               program is executed, it creates an identical copy as a child process 
//               using the standard library function for the system call fork.
//               Parent process and child process each use a for loop to increment a
//               counter variable i, starting from 0 to value 5,000,000.
//               command line when executing the compiled program on a single CPU core:
//               taskset --cpu-list 1 ./Listing_8_2_fork  

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void main() {
  int i;
  // Einen Kindprozess erzeugen
  if (fork()) {
    // Parent process source code
    for (i = 0; i < 5000000; i++) {
      printf("\n Parent: %i", i);
    }
  } else {
    // Child process source code
    for (i = 0; i < 5000000; i++) {
      printf("\n Child : %i", i);
    }
  }
}