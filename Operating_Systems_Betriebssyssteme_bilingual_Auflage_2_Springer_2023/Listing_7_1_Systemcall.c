// title:        Listing_7_1_Systemcall.c
// description:  This c program demonstrates how to use the library functions
//               and system calls
// author:       Dr. Christian Baun
// url:          http://www.christianbaun.de
// license:      GPLv2
// date:         January 19th 2023
// version:      1.0
// gcc_version:  gcc 10.2.1 (Debian 10.2.1-6)
// compile with: gcc Listing_7_1_Systemcall.c -o Listing_7_1_Systemcall
// nodes:        This program example shows by example of getpid how simple 
//               it is using a library function instead of the system call

#include <syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main(void) {
  unsigned int ID1, ID2;

  // System call
  ID1 = syscall(SYS_getpid);
  printf ("Result of the system call: %d\n", ID1);

  // Wrapper function of the glibc, which calls the system call
  ID2 = getpid();
  printf ("Result of the wrapper function: %d\n", ID2);

  return(0);
}