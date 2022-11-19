#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define BUFLEN 256
#define MAX_CLIENTS 2

typedef struct client_info {
	unsigned short port;
	int client_id;
	int last_recv;
	int cur_msg;
	int is_free;
	struct sockaddr cli_addr;
} client_info_t;

int main(){
    
	struct sockaddr_in server_addr, client_addr;
	client_info_t clients[MAX_CLIENTS];

	int sockfd;
	char sendbuf[BUFLEN], recvbuf[BUFLEN]; 
	int bytes_read, len = sizeof(struct sockaddr);
	int cli_nbr = 0, cur_cli = 0;
	int new_client;
	struct timeval tv;
	int max_free = 0;

	memset(&clients, 0, MAX_CLIENTS*sizeof(client_info_t));

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if(sockfd < 0){
		perror("sockfd failed");
		exit(1);
	}
	
	memset(&server_addr, 0, sizeof(server_addr)); 

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons( 0 ); // берет свободный порт
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// bind name to socket
	if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
		perror("bind failed");
		exit(2);
	}

	getsockname(sockfd, (struct sockaddr *)&server_addr, &len);
	printf("[SERVER] IP addr = %s port = %d\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));

	tv.tv_sec = 3; // timeout_in_seconds
	tv.tv_usec = 0;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

	int test = 0;

	while(1) {
		test++;
		
		new_client = 1;
		memset(recvbuf, 0, 256);
		bytes_read = recvfrom(sockfd, (char *)recvbuf, BUFLEN, 0, (struct sockaddr *)&client_addr, &len);
		recvbuf[bytes_read] = '\0';
		
			for (int i = 0; i < cli_nbr; i++) {
				if (clients[i].port == htons(client_addr.sin_port)) {
					new_client = 0;
					if (atoi(recvbuf) != 0) clients[i].last_recv = atoi(recvbuf);
					cur_cli = i;
				}
			}

		for (int i = 0; i < cli_nbr; i++) {
			if (clients[i].cur_msg == 8) {
				if (!clients[i].is_free) {
					clients[i].is_free = 1;
					max_free++;
					printf("[SERVER] client %d is free\n", clients[i].client_id);
				}
			} else {
				// cur_cli = i;
				cur_cli = test%cli_nbr;

			}
		}

		if (max_free == MAX_CLIENTS) {
			printf("[SERVER] Max client (%d) limit has been reached\n", MAX_CLIENTS);
			break;
		}

		if (atoi(recvbuf) == 0 && clients[cur_cli].is_free == 1) continue;

		if (new_client) {
			if (bytes_read == -1) continue;

			printf("[SERVER] NEW CLIENT! port %d\n", client_addr.sin_port);
			clients[cli_nbr].port = htons(client_addr.sin_port);
			clients[cli_nbr].client_id = cli_nbr;
			clients[cli_nbr].cur_msg = 1;
			memcpy(&clients[cli_nbr].cli_addr, &client_addr, sizeof(struct sockaddr_in));
			// clients[cli_nbr].is_free = 1;
			cli_nbr++;
		} else {
			// for (int i = 0; i < cli_nbr; i++) {

				if (clients[cur_cli].last_recv != clients[cur_cli].cur_msg) {
					if (clients[cur_cli].is_free) continue;

					printf("[SERVER] Retransmission to client_id %d port %d last_recv = %d\n", clients[cur_cli].client_id, clients[cur_cli].port, clients[cur_cli].last_recv);

					clients[cur_cli].cur_msg = clients[cur_cli].last_recv;
				} else {
					printf("[SERVER] Received message from client %d: %s port %d\n", cur_cli, recvbuf, clients[cur_cli].port);
				}

					sprintf(sendbuf, "%d", clients[cur_cli].cur_msg);
					sendbuf[2] = '\0';
					sendto(sockfd, sendbuf, strlen(sendbuf), 0, (struct sockaddr *)&clients[cur_cli].cli_addr, len); 
					printf("[SERVER] Sent to client %d: %s\n", cur_cli, sendbuf);
					clients[cur_cli].cur_msg++;

			// }

			
			

		}


	}

	close(sockfd);
	

	return 0;
}
