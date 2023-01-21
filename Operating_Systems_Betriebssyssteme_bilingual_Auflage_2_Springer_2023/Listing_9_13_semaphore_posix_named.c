// title:        Listing_9_13_semaphore_posix_named.c
// description:  This c program is a simple named semaphore (POSIX) example 
//               for Linux
// author:       Dr. Christian Baun
// url:          http://www.christianbaun.de
// license:      GPLv2
// date:         January 20th 2023
// version:      1.2
// gcc_version:  gcc 10.2.1 (Debian 10.2.1-6)
// compile with: gcc Listing_9_13_semaphore_posix_named.c -o Listing_9_13_semaphore_posix_named -lpthread
// nodes:        This program creates a child process. The parent process and
//               the child process both try to print characters in the command
//               line interface (critical section). Each process may print 
//               only one character at a time. Two named POSIX semaphores are   
//               used to ensure mutual exclusion. 
//               Attention: ipcs does only show System V semaphores and not  
//               POSIX semaphores. 
//               Named POSIX semaphores are created in Linux in the folder 
//               /dev/shm with names of the form sem.<name>

#include <stdio.h>      // for printf
#include <stdlib.h>     // for exit
#include <unistd.h>     // for read, write, close
#include <sys/wait.h>   // for wait
#include <semaphore.h>  // for sem_init, sem_post, sem_wait, sem_destroy
#include <fcntl.h>      // for O_CREAT
  
void main() {
  const char sem1_name[] = "/mysem1";
  const char sem2_name[] = "/mysem2";
  int pid_of_child;
  int returncode_close, returncode_unlink;
  int output;
  
  sem_t *mutex_sem1, *mutex_sem2;

  // Prevent buffering of standard output (stdout)
  setbuf(stdout, NULL);

  // Create new named semaphore /mysem1 with initial value 1
  mutex_sem1 = sem_open(sem1_name, O_CREAT, 0600, 1);
  if (mutex_sem1 == SEM_FAILED) {
    printf("Unable to create the semaphore.\n");
    perror("sem_open");
    // Program abort
    exit(1);    
  } else { 
    printf("Semaphore %s has been created.\n", sem1_name);
  }    

  // Create new named semaphore /mysem2 with initial value 0
  mutex_sem2 = sem_open(sem2_name, O_CREAT, 0600, 0);
  if (mutex_sem2 == SEM_FAILED) {
    printf("Unable to create the semaphore.\n");
    perror("sem_open");
    // Program abort
    exit(1);  
  } else { 
    printf("Semaphore %s has been created.\n", sem2_name);
  }    

  // Return the initial value of the semaphore /mysem1 
  // for verification
  sem_getvalue(mutex_sem1, &output);
  printf("Value of %s: %i\n", sem1_name, output);

  // Return the initial value of the semaphore /mysem2 
  // for verification
  sem_getvalue(mutex_sem2, &output);
  printf("Value of %s: %i\n", sem2_name, output);

  // Create a child process
  pid_of_child = fork();
  
  // An error occured --> program abort
  if (pid_of_child < 0) {
    perror("Fork caused an error!\n");
    // Program abort
    exit(1);
  }

  // Child process
  if (pid_of_child == 0) {
    for (int i=0;i<5;i++) {
      sem_wait(mutex_sem2); 
      // P Operation semaphore /mysem2
      // Critical section (start)
      // Pause. Wait between 0 and 2 seconds
      sleep(rand() % 3);
      printf("B");
      // Critical section (end)
      // V Operation semaphore /mysem1
      sem_post(mutex_sem1); 
    }
    
    exit(0);
  }
  
  // Parent process
  if (pid_of_child > 0) {       
    for (int i=0;i<5;i++) {
      sem_wait(mutex_sem1);
      // P Operation semaphore /mysem1
      // Critical section (start)
      // Pause. Wait between 0 and 2 seconds
      sleep(rand() % 3);
      printf("A");
      // Critical section (end)
      // V Operation semaphore /mysem2
      sem_post(mutex_sem2);
    }
  }

  // Wait for the termination of the child process
  wait(NULL);

  printf("\n");

  // Close semaphore /mysem1 
  returncode_close = sem_close(mutex_sem1);
    if (returncode_close < 0) {
      printf("%s could not be closed.\n", sem1_name);
      exit(1);
  } else { 
      printf("%s has been closed.\n", sem1_name);
  }    

  // Close semaphore /mysem2
  returncode_close = sem_close(mutex_sem2);
    if (returncode_close < 0) {
      printf("%s could not be closed.\n", sem2_name);
      exit(1);
  } else { 
      printf("%s has been closed.\n", sem2_name);
  }
  
  // Remove semaphore /mysem1 
  returncode_unlink = sem_unlink(sem1_name);
    if (returncode_unlink < 0) {
      printf("%s could not be removed.\n", sem1_name);
      exit(1);
  } else { 
      printf("%s has been removed.\n", sem1_name);
  }    

  // Remove semaphore /mysem2 
  returncode_unlink = sem_unlink(sem2_name);
    if (returncode_unlink < 0) {
      printf("%s could not be removed.\n", sem2_name);
      exit(1); 
  } else { 
      printf("%s has been removed.\n", sem2_name);
  }

  exit(0);
}