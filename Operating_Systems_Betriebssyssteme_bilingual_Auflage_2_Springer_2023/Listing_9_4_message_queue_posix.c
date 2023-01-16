// title:        Listing_9_4_message_queue_posix.c
// description:  This c program is an example about inter-process communication
//               via a message queue (POSIX) for Linux
// author:       Dr. Christian Baun
// url:          http://www.christianbaun.de
// license:      GPLv2
// date:         January 15th 2023
// version:      1.1
// gcc_version:  gcc 10.2.1 (Debian 10.2.1-6)
// compile with: gcc Listing_9_4_message_queue_posix.c -o Listing_9_4_message_queue_posix -lrt
// nodes:        This program creates a POSIX message queue, inserts a message 
//               into it, reads the message out of it and finally erases it.
//               POSIX message queues are created in Linux in the folder 
//               /dev/mqueue

#include <stdio.h>      // for printf
#include <stdlib.h>     // for exit
#include <string.h>     // for strcpy
#include <mqueue.h>     // for mq_open

int main(int argc, char **argv) {
  int rc_mq_receive;        // Return code) of mq_receive
  const char mq_name[] = "/myqueue";
  char message[] = "Hello World";
  
  mqd_t mymq_descriptor;    // Message queue descriptor
  int msg_prio = 0;         // Specify message priority
  char recv_buffer[80];     // Receive buffer

  struct mq_attr attr;
  attr.mq_flags = 0;        // Flags. Ignored by mq_open 
  attr.mq_maxmsg = 10;      // Maximum number of messages in the queue 
  attr.mq_msgsize = 80;     // Maximum message size (bytes) 
  attr.mq_curmsgs = 0;      // Number of messages in the queue. Ignored by mq_open

  // Fill the memory area of the receive buffer with zeros
  memset(&recv_buffer, 0, sizeof(recv_buffer)); 
  
  // Create a message queue
  // O_CREAT = Create message queue if it does not already exist
  // O_RDWR = Open message queue and allow read and write operations
  // O_NONBLOCK = Open message queue in a non-blocking way
  // 0600 = Access privileges to the message queue
  mymq_descriptor = mq_open(mq_name, 
                            O_RDWR | O_CREAT | O_NONBLOCK, 
                            0600, 
                            &attr);
  if (mymq_descriptor < 0) {
    printf("Unable to create the message queue.\n");
    perror("mq_open");
    exit(1);
  } else {
    printf("Message queue %s can be used now.\n", mq_name);
  }
      
  // Write a message into the message queue
  if (mq_send(mymq_descriptor, 
              message, 
              strlen(message), 
              msg_prio) == -1) {
    printf("Unable to write the message.\n");
    perror("mq_send");
    exit(1);
  } else {
    printf("Message written: %s\n", message);
  }
  
  // Query the attributes of a message queue
  if (mq_getattr(mymq_descriptor, &attr) == -1) {
    printf("Unable to query the attributes.\n");
    perror("mq_getattr");
  }  
    
  // Query the number of messages in the message queue
  if (attr.mq_curmsgs == 0) {
    printf("%s does not contain messages.\n", mq_name);
  } else if (attr.mq_curmsgs == 1) {
    printf("%s contains %d messages.\n", 
            mq_name, 
            attr.mq_curmsgs);
  } else {
    printf("Messages in %s: %d\n", mq_name, attr.mq_curmsgs);
  }
      
  // Fetch the oldest message of the highest priority
  rc_mq_receive = mq_receive(mymq_descriptor, 
                             recv_buffer, 
                             sizeof(recv_buffer), 
                             &msg_prio);
  if (rc_mq_receive < 0) {
    printf("Unable to fetch a message from the queue.\n");
    perror("mq_receive");
  } else {
    printf("Fetched message: %s\n", recv_buffer);
    printf("Message length %i characters.\n", rc_mq_receive);	
  }
  
  // Close the message queue
  if (mq_close(mymq_descriptor) < 0) {
    printf("Unable to close %s.\n", mq_name);
    perror("mq_close");
    exit(1);
  } else { 
    printf("Message queue %s has been closed.\n", mq_name);
  }
  
  // Erase the message queue
  if (mq_unlink(mq_name) < 0) {
    printf("Unable to erase %s.\n", mq_name);
    perror("mq_unlink");
    exit(1);
  } else { 
    printf("Message queue %s has been erased.\n", mq_name);
  }

  exit(0);
}