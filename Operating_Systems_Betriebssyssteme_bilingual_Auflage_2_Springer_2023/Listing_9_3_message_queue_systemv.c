// title:        Listing_9_3_message_queue_systemv.c
// description:  This c program is an example about inter-process communication
//               via a message queue (System V) for Linux
// author:       Dr. Christian Baun
// url:          http://www.christianbaun.de
// license:      GPLv2
// date:         January 15th 2023
// version:      1.5
// gcc_version:  gcc 10.2.1 (Debian 10.2.1-6)
// compile with: gcc Listing_9_3_message_queue_systemv.c -o Listing_9_3_message_queue_systemv
// nodes:        This program creates a System V message queue, inserts a 
//               message into queue, reads the message out of the queue and
//               finally erases the message queue.
//               Check the status of existing System V message queues with the 
//               command ipcs -q

#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <sys/msg.h>
#include <string.h>                  // for strcpy                

// Template of a buffer for msgsnd and msgrcv
typedef struct msgbuf {     
  long mtype;                        // Message type      
  char mtext[80];                    // Send buffer
} msg;

int main(int argc, char **argv) {
  int key = 12345;                   // Message queue key
  int rc_msgget;                     // Return code of msgget
  int rc_msgctl;                     // Return code of msgctl
  int rc_msgrcv;                     // Return code of msgrcv
  // Create a receive buffer and a send buffer
  msg sendbuffer, receivebuffer; 
    
  // Create a message queue
  // IPC_CREAT = Create message queue if it does not already exist
  // 0600 = Access privileges to the new message queue
  rc_msgget = msgget(key, IPC_CREAT | 0600);
  if(rc_msgget < 0) {
    printf("Unable to create the message queue.\n");
    perror("msgget");
    exit(1);
  } else {
    printf("Message queue %i with ID %i has been created.\n", 
            key, rc_msgget);
  }
    
  // Specifiy the message type
  sendbuffer.mtype = 1;
  // Write the message into the send buffer
  strncpy(sendbuffer.mtext, "Testmessage", 11);

  // Write a message into the message queue
  if (msgsnd(rc_msgget, 
             &sendbuffer, 
             sizeof(sendbuffer.mtext), 
             IPC_NOWAIT) == -1) {
    printf("Unable to write the message.\n");
    perror("msgsnd");
    exit(1);
  } else {
    printf("Message written: %s\n", sendbuffer.mtext);
  }
    
  // If mtype = 0, read the first message from the message queue
  // If mtype = positive, read the first message of this type from the message queue
  // If mtype = negative, read the first message from the message queue whose type is less than or equal to the absolute value of mtype 

  // Fetch the first message of type 1 from the message queue
  receivebuffer.mtype = 1;
    
  rc_msgrcv = msgrcv(rc_msgget, 
                     &receivebuffer, 
                     sizeof(receivebuffer.mtext), 
                     receivebuffer.mtype, 
                     MSG_NOERROR | IPC_NOWAIT);
  if (rc_msgrcv < 0) {
    printf("Unable to fetch a message from the queue.\n");
    perror("msgrcv");
    exit(1);
  } else {
    printf("Fetched message: %s\n", receivebuffer.mtext);
    printf("Message length: %i characters.\n", rc_msgrcv);	
  }
    
  // Erase the message queue
  rc_msgctl = msgctl(rc_msgget, IPC_RMID, 0);
  if (rc_msgctl < 0) {
    printf("The message queue has been erased.\n");
    perror("msgctl");
    exit(1);
  } else { 
    printf("Message queue %i with ID %i has been erased.\n", 
            key, rc_msgget);
  }
    
  exit(0);
}