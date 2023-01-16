// title:        Listing_9_1_shared_memory_systemv.c
// description:  This c program is an example about inter-process communication
//               via shared memory (System V) for Linux
// author:       Dr. Christian Baun
// url:          http://www.christianbaun.de
// license:      GPLv2
// date:         January 15th 2023
// version:      1.6
// gcc_version:  gcc (Debian 10.2.1-6)
// compile with: gcc Listing_9_1_shared_memory_systemv.c -o Listing_9_1_shared_memory_systemv 
// nodes:        This program creates a System V shared memory segment, 
//               attaches it to the running process, inserts a string into it,
//               reads the string out of it, detaches it and finally erases it.
//               Check the status of existing System V shared memory segments  
//               with the command ipcs -m

#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
// This is required if you want to stop the program
// for some seconds e.g. with sleep(10);
#include <unistd.h>     
#define MAXMEMSIZE 20

int main(int argc, char **argv) {
  int key = 12345;
  int return_shmget;
  int return_shmdt;
  int return_sprintf;
  int return_printf;
  int return_shmctl;
  char *shmpointer;

  // Create shared memory segment
  // IPC_CREAT = Create segment if it does not already exist
  // 0600 = Access privileges to the new segment
  return_shmget = shmget(key, MAXMEMSIZE, IPC_CREAT | 0600);
  if (return_shmget < 0) {
    printf("Unable to create the shared memory segment.\n");
    exit(1);
  }
  
  printf("The shared memory segment has been created.\n");
    
  // Attach the shared memory segment
  shmpointer = shmat(return_shmget, 0, 0);
  if (shmpointer==(char *)-1) {
    printf("Unable to attach the shared memory segment.\n");
    exit(1);
  } else { 
    printf("The shared memory segment has been attached.\n");
  }
        
  // Write a string into the shared memory segment
  return_sprintf = sprintf(shmpointer, "Hello World.");
  if (return_sprintf < 0) {
    printf("The write operation failed.\n");
    exit(1);
  } else { 
    printf("%i characters were written.\n", return_sprintf);
  }
        
  // Read the string from the shared memory segment
  if (printf ("Content of the segment: %s\n", shmpointer) < 0) {
    printf("The read operation failed.\n");
    exit(1);
  }
        
  // Detach the shared memory segment
  return_shmdt = shmdt(shmpointer);
  if (return_shmdt < 0) {
    printf("Unable to detach the shared memory segment.\n");
    exit(1);
  } else { 
    printf("The shared memory segment has been detached.\n");
  }
        
  // Erase the shared memory segment
  return_shmctl = shmctl(return_shmget, IPC_RMID, 0);    
  if (return_shmctl == -1) {
    printf("Unable to erase the shared memory segment.\n");
    exit(1);
  } else { 
    printf("The shared memory segment has been erased.\n");
  }

  exit(0);
}
