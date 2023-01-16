// title:        Listing_9_2_shared_memory_posix.c
// description:  This c program is an example about inter-process communication
//               via shared memory (POSIX) for Linux
// author:       Dr. Christian Baun
// url:          http://www.christianbaun.de
// license:      GPLv2
// date:         January 15th 2023
// version:      1.6
// gcc_version:  gcc 10.2.1 (Debian 10.2.1-6)
// compile with: gcc Listing_9_2_shared_memory_posix.c -o Listing_9_2_shared_memory_posix -lrt
// nodes:        This program creates a POSIX shared memory segment, 
//               attaches it to the running process, inserts a string into it,
//               reads the string out of it, detaches it and finally erases it.
//               POSIX shared memory segments are created in Linux in the folder 
//               /dev/shm

#include <sys/mman.h>   // for shm_open, mmap, munmap, shm_unlink
#include <fcntl.h>      // for O_CREAT, O_CREAT
#include <stdio.h>      // for printf
#include <stdlib.h>     // for exit
#include <string.h>     // for strcpy
// This is required if you want to stop the program
// for some seconds e.g. with sleep(10);
#include <unistd.h>
#define MAXMEMSIZE 20

int main(int argc, char **argv) {
  int shmem_fd;         // File descriptor of the segment
  int rc_sprintf;       // Return value of sprintf
  void* shmem_pointer;
  const char name[] = "/mymemory";

  // Create shared memory segment
  // IPC_CREAT = Create segment if it does not already exist
  // O_RDWR = Open segment and allow read and write operations
  // 0600 = Access privileges to the new segment
  shmem_fd = shm_open(name, O_CREAT | O_RDWR, 0600);
  if (shmem_fd < 0) {
    printf("Unable to create the shared memory segment.\n");
    perror("shm_open");
    exit(1);
  } else {
    printf("Shared memory segment %s was created.\n", name);
  }

  // Adjust the size of the memory segment
  if (ftruncate(shmem_fd, MAXMEMSIZE) < 0) {
    printf("Unable to adjust the size.\n");
    perror("ftruncate");
    exit(1);
  } else { 
    printf("The size of %s was adjusted.\n", name);
  }
  
  // Attach the shared memory segment
  shmem_pointer = mmap(0, 
                       MAXMEMSIZE, 
                       PROT_WRITE, 
                       MAP_SHARED, 
                       shmem_fd, 
                       0);
  if (shmem_pointer < 0) {
    printf("Unable to attach %s.\n", name);
    perror("mmap");
    exit(1);
  } else {
    printf("%s was attached.\n", name);
  }
  
  // Write a string into the shared memory segment
  rc_sprintf = sprintf(shmem_pointer, "Hello World."); 
  if (rc_sprintf < 0) {
    printf ("The write operation failed.\n");
    exit(1);
  } else { 
    printf("%i characters were written.\n", rc_sprintf);
  }

  // Read the string from the shared memory segment
  if (printf("Content of %s: %s\n", name, shmem_pointer) < 0) {
    printf ("The read operation failed.\n");
    exit(1);
  }
  
  // Detach the shared memory segment
  if (munmap(shmem_pointer, MAXMEMSIZE) < 0) {
    printf("Unable to detach %s.\n", name);
    perror("munmap");
    exit(1);
  } else { 
    printf("%s was detached from the process.\n", name);
  }
  
  // Close the shared memory segment
  if (close(shmem_fd) < 0) {
    printf("Unable to close %s.\n", name);
    perror("close");
    exit(1);
  } else { 
    printf("%s was closed.\n", name);
  }
      
  // Erase the shared memory segment
  if (shm_unlink(name) < 0) {
    printf("Unable to erase %s.\n", name);
    perror("shm_unlink");
    exit(1);
  } else { 
    printf("%s was erased.\n", name);
  }
    
  exit(0);
}