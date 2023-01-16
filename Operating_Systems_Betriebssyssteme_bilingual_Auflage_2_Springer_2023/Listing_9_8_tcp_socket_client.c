// title:        Listing_9_8_tcp_socket_client.c
// description:  This c program is a TCP socket example for Linux
// author:       Dr. Christian Baun
// url:          http://www.christianbaun.de
// license:      GPLv2
// date:         January 15th 2023
// version:      1.5
// gcc_version:  gcc 10.2.1 (Debian 10.2.1-6)
// compile with: gcc Listing_9_8_tcp_socket_client.c -o Listing_9_8_tcp_socket_client
// nodes:        This program connects with the socket of a server and transmits
//               a message which is typed in at the command line interface by the
//               user. The IP address and port number are specified as arguments on the
//               command line when executing the compiled program:
//               ./Listing_9_8_tcp_socket_client <ip-address> <portnumber>

#include <stdio.h>      // for printf
#include <stdlib.h>     // for atoi, exit
#include <string.h>     // for strlen
#include <sys/socket.h> // for socket
#include <netinet/in.h> // for the structure sockaddr_in
#include <unistd.h>     // for read, write, close
#include <arpa/inet.h>	// for htons, inet_addr

int main(int argc, char *argv[])
{
  int sd;
  int portnumber;
  struct sockaddr_in addr;
    
  // Fill the buffer with zero bytes
  char buffer[1024] = { 0 }; 

  // If hostname and/or port number are missing...
  if (argc < 3) {
    printf("Host name and/or port number are missing.\n");
    exit(1);
  }
     
  // Second argument after the file name = port number
  portnumber = atoi(argv[2]);

  // Create socket
  sd = socket(AF_INET, SOCK_STREAM, 0); 
  if (sd < 0) {
    printf("Unable to create the socket.\n");
    exit(1);
   } else {
    printf("The socket has been created.\n");
  }

  // Fill the memory area of sockaddr_in with zeros
  memset(&addr, 0, sizeof(addr)); 
    
  // Store the socket address in the structure sockaddr_in
  addr.sin_family = AF_INET;          
  addr.sin_port = htons(portnumber);
  addr.sin_addr.s_addr = inet_addr(argv[1]);  
    
  // Send a connection request to the server
  if (connect(sd,              
              (struct sockaddr *) &addr, 
              sizeof(addr)) < 0) { 
    printf("Connection request failed.\n");
    exit(1);
  } else {
    printf("Connection to the server established.\n");
  }
    
  printf("Please enter the message: ");
  
  // Read the message from the command line interface
  fgets(buffer, sizeof(buffer), stdin);   
    
  // Send message
  if (write(sd, buffer, strlen(buffer)) < 0) { 
    printf("The write operation has failed.\n");
    exit(1);
  }
    
  // Fill the buffer with zero bytes
  memset(buffer, 0, sizeof(buffer));  
    
  // Receive message
  if (read(sd, buffer, sizeof(buffer)) < 0) {   
    printf("The read operation has failed.\n");
    exit(1);
  } else {
    printf("%s\n",buffer); 
  }

  // Close the socket
  if (close(sd) == 0) {  
    printf("The socket has been closed.\n");
  }
    
  exit(0);
}