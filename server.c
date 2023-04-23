#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define MAX BUFFER_SIZE
#define PORT 8080
#define SA struct sockaddr

int BUFFER_SIZE= 1500;

// Function designed for chat between client and server.
void server_chatter(int sockfd, int buffer_size) {

	char buff[buffer_size]; // character array of size buffer_size

    bzero(buff, buffer_size);

    // read the message from client and copy it in buffer
    recv(sockfd, buff, sizeof(buff), 0);

    // print buffer which contains the client contents
    printf("\nfrom client: %s\n", buff);
    bzero(buff, buffer_size);

    // send a response to the client
    printf("sending acknowledgement...\n");
    send(sockfd, "ACK", strlen("ACK"), 0);
    printf("acknowledgement sent...\n");
}

// Driver function
int main() {

	int serverfd, clientfd, len; // socket file descriptors
	struct sockaddr_in servaddr, cli; // server & client address structures

	// -- create a socket for the server --------------------------------------

	serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverfd == -1) {
		perror("socket creation failed");
		exit(0);
	}
	else {
        printf("socket successfully created...\n");
    }

	bzero(&servaddr, sizeof(servaddr));

	// -- assign IP, PORT to server address -----------------------------------

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// -- binding newly created socket to given IP and port -------------------

	if ((bind(serverfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		perror("socket bind failed");
		exit(0);
	}
	else {
        printf("socket successfully bound...\n");
    }

	// -- now server is ready to listen for connections -----------------------

	if ((listen(serverfd, 5)) != 0) {
		perror("listen failed");
		exit(0);
	}
	else {
        printf("server listening...\n");
    }

	len = sizeof(cli);

	// -- accept the connection request from client ---------------------------

	clientfd = accept(serverfd, (SA*)&cli, &len);
	if (clientfd < 0) {
		perror("server accept failed");
		exit(0);
	}
	else {
        printf("server accepts the client...\n");
    }

    // -- client/server chatting ----------------------------------------------

    server_chatter(clientfd, 1500);
    server_chatter(clientfd, 5200);
    server_chatter(clientfd, 750);

	// -- after chatting, close the socket ------------------------------------

	close(serverfd);
}

