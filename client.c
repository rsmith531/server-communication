#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include<arpa/inet.h>
#include <time.h>

#define MAX BUFFER_SIZE
#define PORT 8080
#define SA struct sockaddr

// global buffer size variable
int BUFFER_SIZE = 1500; // 5200, 750

double client_chatter(int sockfd, int buffer_size) {

    // open the file to be sent to the server
    const char *filename = "file.txt";
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("error opening file");
        exit(1);
    }

    char buff[buffer_size]; // character array of size buffer_size
    struct timespec start, end; // timer variables

    bzero(buff, sizeof(buff)); // zero out the buffer
    fread(buff, sizeof(char), buffer_size, file); // read a chunk of the file into the buffer

    clock_gettime(CLOCK_MONOTONIC, &start); // start timer

    printf("\nsending message to server...\n");
    send(sockfd, buff, sizeof(buff), 0); // send the buffer to the server
    printf("message sent...\n");

    bzero(buff, sizeof(buff)); // zero out the buffer

    printf("awaiting response...\n");
    recv(sockfd, buff, sizeof(buff), 0); // read the server's response
    printf("from server: %s", buff); // print the server's response

    clock_gettime(CLOCK_MONOTONIC, &end); // end timer

    double elapsed = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1e9; // calculate elapsed time

    return elapsed;
}

int main() {

	int clientfd; // client socket file descriptor
	struct sockaddr_in servaddr; // server address structure

	// -- client socket create and verification -------------------------------

	clientfd = socket(AF_INET, SOCK_STREAM, 0);
	if (clientfd == -1) {
		perror("socket creation failed");
		exit(0);
	}
	else {
        printf("socket successfully created...\n");
    }

	bzero(&servaddr, sizeof(servaddr));

	// -- assign IP, PORT to the server ---------------------------------------

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// -- connect the client socket to server socket --------------------------

	if (connect(clientfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		perror("server connection failed");
		exit(0);
	}
	else {
        printf("connected to the server...\n");
    }

    // -- client/server chatting ----------------------------------------------

    double timer = client_chatter(clientfd, 1500); // 1500
    printf("\ntime to send 1500 characters: %.6f seconds\n", timer);

    BUFFER_SIZE = 5200;

    timer = client_chatter(clientfd, 5200); // 5200
    printf("\ntime to send 5200 characters: %.6f seconds\n", timer);

    BUFFER_SIZE = 750;

    timer = client_chatter(clientfd, 750); // 750
    printf("\ntime to send 750 characters: %.6f seconds\n", timer);

	// -- close the socket ----------------------------------------------------

	close(clientfd);
}

