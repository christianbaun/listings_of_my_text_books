// title:        Listing_9_12_semaphore_systemv.c
// description:  This c program is a simple semaphore (System V) example for Linux
// author:       Dr. Christian Baun
// url:          http://www.christianbaun.de
// license:      GPLv2
// date:         January 20th 2023
// version:      1.2
// gcc_version:  gcc 10.2.1 (Debian 10.2.1-6)
// compile with: gcc Listing_9_12_semaphore_systemv.c -o Listing_9_12_semaphore_systemv
// nodes:        This program creates a child process. The parent process and
//               the child process both try to print characters in the command
//               line interface (critical section). Each process may print 
//               only one character at a time. Two System V semaphores are used  
//               to ensure mutual exclusion.
//               System V semaphores can be monitored with command ipcs -s

#include <stdio.h>      // for printf
#include <stdlib.h>     // for exit
#include <unistd.h>     // for read, write, close
#include <sys/wait.h>   // for wait
#include <sys/sem.h>    // for semget, semctl, semop

void main() {
  int pid_of_child;
  int key1=12345;
  int key2=54321;
  int rc_semget1;       // Return code of semget
  int rc_semget2;       // Return code of semget
  int rc_semctl;        // Return code of semctl
  int output;

  // Prevent buffering of standard output (stdout)
  setbuf(stdout, NULL);

  // Create new semaphore group 12345 with one semaphore
  // IPC_CREAT = Create a semaphore if it doesn't exist already
  // IPC_EXCL = Create a new semaphore group, and don't access 
  // any existing one with the same key
  // 0600 = Access privileges to the new semaphore group
  rc_semget1 = semget(key1, 1, IPC_CREAT | IPC_EXCL | 0600);
  if (rc_semget1 < 0) {
    printf("Unable to create the semaphore group.\n");
    perror("semget");
    // Program abort
    exit(1);
  } else { 
    printf("Semaphore group %i with key %i has been created.\n",
           rc_semget1, key1);
  }    

  // Create new semaphore group 54321 with one semaphore
  rc_semget2 = semget(key2, 1, IPC_CREAT | IPC_EXCL | 0600);
  if (rc_semget2 < 0) {
    printf("Unable to create the semaphore group.\n");
    perror("semget");
    // Program abort
    exit(1);
  } else { 
    printf("Semaphore group %i with key %i has been created.\n",
           rc_semget2, key2);
  }   

  // Definition of the P operation. 
  // Decrement the value of a semaphore by one.
  struct sembuf p_operation = {0, -1, 0};  
  
  // Definition of the V operation.
  // Increment the value of a semaphore by one. 
  struct sembuf v_operation = {0, 1, 0};    

  // Set first semaphore of group 12345 to initial value 1
  rc_semctl = semctl(rc_semget1, 0, SETVAL, 1);
  if (rc_semctl < 0) {
    printf("Unable to set the value of %i.\n", key1);
    perror("semctl SETVAL"); 
    exit(1);
  }

  // Set first semaphore of group 54321 to initial value 0
  rc_semctl = semctl(rc_semget2, 0, SETVAL, 0);
  if (rc_semctl < 0) {
    printf("Unable to set the value of %i.\n", key2);
    perror("semctl SETVAL"); 
    exit(1);
  }

  // Return initial value of the first semaphore of
  // semaphore group 12345 for verification
  output = semctl(rc_semget1, 0, GETVAL, 0);
  printf("Value of the semaphore group %i: %i\n", 
         rc_semget1, output);

  // Return initial value of the first semaphore of
  // semaphore group 54321 for verification
  output = semctl(rc_semget2, 0, GETVAL, 0);
  printf("Value of the semaphore group %i: %i\n", 
         rc_semget2, output);

  // Create a child process
  pid_of_child = fork();
  
  // An error occured --> program abort
  if (pid_of_child < 0) {
    perror("Fork caused an error!\n");
    exit(1);
  }

  // Child process
  if (pid_of_child == 0) {
    for (int i=0;i<5;i++) {
      // P Operation semaphore 54321
      semop(rc_semget2, &p_operation, 1); 
      // Critical section (start)
      // Pause. Wait between 0 and 2 seconds
      sleep(rand() % 3);
      printf("B");
      // Critical section (end)
      // V Operation semaphore 12345
      semop(rc_semget1, &v_operation, 1); 
    }
    exit(0);
  }
  
  // Parent process
  if (pid_of_child > 0) {       
    for (int i=0;i<5;i++) {
      semop(rc_semget1, &p_operation, 1); 
      // P Operation semaphore 12345
      // Critical section (start)
      // Pause. Wait between 0 and 2 seconds
      sleep(rand() % 3);
      printf("A");
      // Critical section (end)
      // V Operation semaphore 54321
      semop(rc_semget2, &v_operation, 1); 
    }
  }

  // Wait for the termination of the child process
  wait(NULL);

  printf("\n");

  // Remove semaphore group 12345
  rc_semctl = semctl(rc_semget1, 0, IPC_RMID, 0);
    if (rc_semctl < 0) {
      printf("Unable to remove the semaphore group.\n");
      perror("semctl");
      exit(1);
  } else { 
      printf("Semaphore group %i has been removed.\n", 
             rc_semget1);
  }    

  // Remove semaphore group 54321
  rc_semctl = semctl(rc_semget2, 0, IPC_RMID, 0);
    if (rc_semctl < 0) {
      printf("Unable to remove the semaphore group.\n");
      perror("semctl");
      exit(1);
  } else { 
      printf("Semaphore group %i has been removed.\n", 
             rc_semget2);
  }

  exit(0);
}