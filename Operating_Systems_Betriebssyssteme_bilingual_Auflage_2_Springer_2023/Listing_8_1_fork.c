// title:        Listing_8_1_fork.c
// description:  This c program demonstrates how child and parent processes are identified
//               in the sorce code
// author:       Dr. Christian Baun
// url:          http://www.christianbaun.de
// license:      GPLv2
// date:         January 17th 2023
// version:      1.0
// gcc_version:  gcc 10.2.1 (Debian 10.2.1-6)
// compile with: gcc Listing_8_1_fork.c -o Listing_8_1_fork
// nodes:        A process calls the standard library function for the system call fork. 
//               This results in the creation of an exact copy of the calling process. 
//               In the source code, the two processes can only be identified using the 
//               return value of fork. If the return value is negative, an error occurred 
//               when attempting to create the process. If the return value is a positive 
//               integer, it is the parent process, and the return value is the process ID 
//               (PID) of the child process. If the return value has the 0, it is the 
//               child process.

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void main() {
  // Create a child process
  int return_value = fork();

  if (return_value < 0) {
    // Return value of fork = negative number --> error
    // Memory or processes table might be full
    printf("An error occurred.\n");
  }
  
  if (return_value > 0) {
    // Return value of fork = positive number --> parent process
    // Return value = PID of the ne child process
    printf("Parent: Here is the parent process.\n");
    printf("Parent: PID of the child: %i\n", return_value);
  }
  
  if (return_value == 0) {
    // Return value of fork = 0 --> child process
    printf("Child: Here is the child process.\n");
  }
}