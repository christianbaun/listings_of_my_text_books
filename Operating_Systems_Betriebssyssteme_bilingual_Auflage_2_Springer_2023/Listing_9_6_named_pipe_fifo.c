// title:        Listing_9_6_named_pipe_fifo.c
// description:  This c program is an example about inter-process communication 
//               via a named pipe (FIFO) for Linux
// author:       Dr. Christian Baun
// url:          http://www.christianbaun.de
// license:      GPLv2
// date:         January 15th 2023
// version:      1.5
// gcc_version:  gcc 10.2.1 (Debian 10.2.1-6)
// compile with: gcc Listing_9_6_named_pipe_fifo.c -o Listing_9_6_named_pipe_fifo
// nodes:        This program creates a child process and inside the parent 
//               process a named pipe (FIFO) is created. The parent process 
//               writes a string into the pipe and the child process reads the 
//               string out of the pipe

#include <stdio.h>      // for printf
#include <stdlib.h>     // for exit
// for O_RDONLY and O_WRONLY file access modes of open
#include <fcntl.h>
// This is required if you want to stop the program
// for some seconds e.g. with sleep(10);
#include <unistd.h>
#include <sys/stat.h>   // for mkfifo

void main() {
  int pid_of_child;
  
  // Create the pipe testfifo
  if (mkfifo("testfifo",0666) < 0) {
    printf("Unable to create the pipe.\n");
    // Terminate the program (failure)
    exit(1);
  } else {
    printf("The pipe testfifo has been created.\n");
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
    
    // Create the file descriptor (handle) for the pipe
    int fd;  
    char message[] = "Test message";
    
    // Open the pipe for writing
    fd = open("testfifo", O_WRONLY);
    
    // Write data into the pipe
    write(fd, &message, sizeof(message));
    
    // Close the pipe
    close(fd);
    
    exit(0);
  }

  // Child process
  if (pid_of_child == 0) {
    printf("Child process. PID: %i\n", getpid());

    // Create the file descriptor (handle) for the pipe
    int fd; 
    // Create a receive buffer 
    char buffer[80];

    // Open the pipe for reading
    fd = open("testfifo", O_RDONLY);
    
    // Read data from the pipe
    if (read(fd, buffer, sizeof(buffer)) > 0) {
      // Return received data to the command line
      printf("Data received: %s\n", buffer);
    } else {
      printf("Unable to read data from the pipe.\n");  
    }
    
    // Close the pipe
    close(fd);
    
    // Erase the pipe
    if (unlink("testfifo") < 0) {
      printf("Unable to erase the pipe.\n");
      // Terminate the program (failure)
      exit(1);
    } else {
      printf("The pipe has been erased.\n");
    }
  
    exit(0);
  }
}