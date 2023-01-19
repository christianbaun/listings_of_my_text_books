// title:        Listing_8_3_fork.c
// description:  This c program returns the PID and PPID od child and parent processes
// author:       Dr. Christian Baun
// url:          http://www.christianbaun.de
// license:      GPLv2
// date:         January 17th 2023
// version:      1.0
// gcc_version:  gcc 10.2.1 (Debian 10.2.1-6)
// compile with: gcc Listing_8_3_fork.c -o Listing_8_3_fork
// nodes:        The program creates a child process by calling fork. Parent process and
//               child process use the library functions for the system calls getpid and 
//               getppid to request the own process ID (PID) and the parent process ID 
//               (PPID). In rate cases, the parent process terminated before the child 
//               process, and, as usual, the child process gets the init/systemd process
//               assigned as its new parent process. The init/systemd process is the 
//               first process in Linux and UNIX-like operating systems. It adopts
//               orphaned processes automatically. 

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void main() {
  int pid_of_child;

  pid_of_child = fork();

  // An error occured --> program abort
  if (pid_of_child < 0) {
    perror("\n fork() caused an error!");
    exit(1);
  }

  // Parent process
  if (pid_of_child > 0) {
    printf("\n Parent: PID: %i", getpid());
    printf("\n Parent: PPID: %i", getppid());
  }

  // Child process
  if (pid_of_child == 0) {
    printf("\n Child:  PID: %i", getpid());
    printf("\n Child:  PPID: %i", getppid());
  }
}