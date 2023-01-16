// title:        Listing_9_7_tcp_socket_server.c
// description:  This c program is a TCP socket example for Linux
// author:       Dr. Christian Baun
// url:          http://www.christianbaun.de
// license:      GPLv2
// date:         January 15th 2023
// version:      1.5
// gcc_version:  gcc 10.2.1 (Debian 10.2.1-6)
// compile with: gcc Listing_9_7_tcp_socket_server.c -o Listing_9_7_tcp_socket_server
// nodes:        This program creates a socket and waits for an incoming 
//               connection from a client via TCP. The server waits for an 
//               incoming transmission and will reply a string to the client
//               The port number is specified as an argument when executing 
//               the compiled program:
//               ./Listing_9_7_tcp_socket_server <portnumber>

#include <stdio.h>      // for printf
#include <stdlib.h>     // for atoi, exit
#include <string.h>     // for memset
#include <sys/socket.h> // for socket
#include <netinet/in.h> // for the structure sockaddr_in
#include <unistd.h>     // for read, write, close
#include <arpa/inet.h>	// for htons

int main(int argc, char *argv[])
{
  int sd;
  int new_socket;
  int portnumber;
  int addrlen;
  struct sockaddr_in addr;   
 
  // Fill the buffer with zero bytes
  char buffer[1024] = { 0 };    
     
  // The port number must be specified as an argument
  if (argc < 2) {
    printf("The port number is missing.\n");
    exit(1);
  }
     
  // The argument following the file name is the port number
  portnumber = atoi(argv[1]); 
     
  // Store the socket address in the structure sockaddr_in
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(portnumber);
     
  // Create socket
  sd = socket(AF_INET, SOCK_STREAM, 0); 
  if (sd < 0) {
    printf("Unable to create the socket.\n");
    exit(1);
  } else {
    printf("The socket has been created.\n");
  }

  // Bind socket to a port number
  if (bind(sd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
    printf("The port is not available.\n");
    exit(1);
  } else {
    printf("The socket has been bound to the port number.\n");
  }

  // Create a queue for connection requests
  if (listen(sd, 5) == 0) {       
    printf("Waiting for connection requests.\n");
  } else {
    printf("An error occurred during the listen function.\n");
  }

  addrlen = sizeof(addr);
  new_socket = accept(sd, (struct sockaddr *) &addr, &addrlen); 
  if (new_socket < 0) {
    printf("Connection request failed.\n");
    exit(1);
  } else {
    printf("Connection to a client established.\n");
  }
     
  // Fill the contents of the buffer with zero bytes
  memset(buffer, 0, sizeof(buffer)); 

  // Receive message
  if (read(new_socket, buffer, sizeof(buffer)) < 0) {
    printf("The read operation has failed.\n");
    exit(1);
  }
     
  // Print the received message
  printf("Received message: %s\n", buffer);  
     
  char reply[]="Server: Message received.\n";
    
  // Send message
  if (write(new_socket, reply, sizeof(reply)) < 0) {
    printf("The write operation has failed.\n");
    exit(1);
  }

  // Close the socket
  if (close(new_socket) == 0) {     
    printf("The connected socket has been closed.\n");
  }

  // Close the socket
  if (close(sd) == 0) {   
    printf("The socket has been closed.\n");
  }
    
  exit(0);
}