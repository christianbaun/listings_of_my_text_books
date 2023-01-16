// title:        Listing_9_5_anonymous_pipe.c
// description:  This c program is an example about inter-process communication 
//               via a pipe for Linux
// author:       Dr. Christian Baun
// url:          http://www.christianbaun.de
// license:      GPLv2
// date:         January 15th 2023
// version:      1.4
// gcc_version:  gcc 10.2.1 (Debian 10.2.1-6)
// compile with: gcc Listing_9_5_anonymous_pipe.c -o Listing_9_5_anonymous_pipe
// nodes:        This program creates a child process and inside the parent 
//               process a pipe is created. The parent process writes a string
//               into the pipe and the child process reads the string out of
//               the pipe

#include <stdio.h>      // for printf
#include <stdlib.h>     // for exit
// This is required if you want to stop the program
// for some seconds e.g. with sleep(10);
#include <unistd.h>

void main() {
  int pid_of_child;
  // Create file descriptors (handles) for read and write
  int testpipe[2]; 
  
  // Create the pipe testpipe
  if (pipe(testpipe) < 0) {
    printf("Unable to create the Pipe.\n");
    // Terminate the program (failure)
    exit(1);
  } else {
    printf("The pipe testpipe has been created.\n");
  }
  
  // Create a child process
  pid_of_child = fork();
  
  // An error occured during fork
  if (pid_of_child < 0) {
    printf("An error occured during fork!\n");
    // Terminate the program (failure)
    exit(1);
  }

  // Parent process
  if (pid_of_child > 0) {    
    printf("Parent process. PID: %i\n", getpid());
    
    // Block the read channel of the pipe
    close(testpipe[0]);
        
    char message[] = "Test message";
    
    // Write data into the write channel of the pipe
    write(testpipe[1], &message, sizeof(message));
  }

  // Child process
  if (pid_of_child == 0) {
    printf("Child process. PID: %i\n", getpid());
    
    // Block the write channel of the pipe
    close(testpipe[1]);
    
    // Create a receive buffer
    char buffer[80];
    
    // Read data from the read channel of the pipe
    read(testpipe[0], buffer, sizeof(buffer));
    
    // Return received data to the command line
    printf("Data received: %s\n", buffer);
  }
}