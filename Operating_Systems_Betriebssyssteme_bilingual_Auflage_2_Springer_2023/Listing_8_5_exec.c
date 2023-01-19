// title:        Listing_8_5_exec.c
// description:  This c program demonstrates how to work with the system call exec
// author:       Dr. Christian Baun
// url:          http://www.christianbaun.de
// license:      GPLv2
// date:         January 18th 2023
// version:      1.0
// gcc_version:  gcc 10.2.1 (Debian 10.2.1-6)
// compile with: gcc Listing_8_5_exec.c -o Listing_8_5_exec
// nodes:        The program creates a child process by calling fork. The return value of
//               fork is used in the source code to determine whether it is the parent
//               process or the child process. Parent process and child process use the 
//               library functions for the system calls getpid and getppid to request the 
//               own process ID (PID) and the parent process ID (PPID). The process ID 
//               of the child is obtained by the value of the integer variable pid.
//               The child process replaces itself by calling execl with the date command
//               as an argument. The example uses the library function execl because the 
//               system call exec does not exist as a library function, but there are 
//               several variants, one of which is \verb!execl

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main () {
  int pid;
  // Create a child process
  pid = fork();

  // An error occured --> program abort
  if (pid < 0) {
    printf("fork() caused an error!\n");
    exit(1);
  }
  
  // Parent process
  if (pid > 0) {
    printf("[Parent] Own PID:           %d\n", getpid());
    printf("[Parent] PID of the child:  %d\n", pid);
  }
  
  // Child process
  if (pid == 0) {
    printf("[Child]  Own PID:           %d\n", getpid());
    printf("[Child]  PID of the parent: %d\n", getppid());

    // Current program is replaced by "date"
    // "date" will be the process name in the process table
    execl("/bin/date", "date", "-u", NULL);
  }
  
  printf("[Parent] Program abort.\n");
  return 0;
}