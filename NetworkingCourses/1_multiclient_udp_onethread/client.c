#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>

#define BUFLEN 256

int main(int argc, char *argv[]){

	int n = 7;
    
	int sockfd;
	struct sockaddr_in client_addr, server_addr;
	int bytes_read, len = sizeof(struct sockaddr);
	char sendbuf[BUFLEN], recvbuf[BUFLEN];

	if(argc!=3){
		printf("Need ip and port.\n");
		exit(1);
	}

	memset(&server_addr, 0, sizeof(server_addr)); 
    memset(&client_addr, 0, sizeof(client_addr));

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if(sockfd < 0){

		perror("socket");
		exit(1);

	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2])); 
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	
	if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
		perror("connect");
		exit(2);
	}

	int lost = 1;

	
		sprintf(sendbuf, "%d", 0);
		sendto(sockfd, sendbuf, strlen(sendbuf), 0, (const struct sockaddr *)&server_addr, len); 
		printf("[CLIENT] Sent to server message: %s (connect)\n", sendbuf);

	for(int i = 1; i <= n; i++){

		sprintf(sendbuf, "%d", i);
		sendto(sockfd, sendbuf, strlen(sendbuf), 0, (const struct sockaddr *)&server_addr, len); 
		printf("[CLIENT] Sent to server message: %s\n", sendbuf);


		bytes_read = recvfrom(sockfd, (char *)recvbuf, BUFLEN, 0, (struct sockaddr *)&client_addr, &len);
		recvbuf[bytes_read] = '\0';
		printf("[CLIENT] Received message from server: %s\n", recvbuf);


		// потеряли пакет
		if (lost % 3 == 0) {
			printf("[CLIENT] Lost packet on purpose, waiting for retransmission of %s\n", recvbuf);
			bytes_read = recvfrom(sockfd, (char *)recvbuf, BUFLEN, 0, (struct sockaddr *)&client_addr, &len);
			recvbuf[bytes_read] = '\0';
			printf("[CLIENT] Received message from server: %s\n", recvbuf);
		}

		lost++;
		sleep(1);
	}
    
	close(sockfd);
	return 0;
}