#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#define BUFLEN 256

void reaper(int sig){
	int status;
	while(wait3(&status,WNOHANG,(struct rusage*)0)>=0);
}
 
int main(){

    
	struct sockaddr_in addr;
	int sock, listener;
	char buf[BUFLEN];
	int bytes_read,len = sizeof(struct sockaddr);

	listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(listener < 0){
		perror("socket");
		exit(1);
	}


	addr.sin_family = AF_INET;
	addr.sin_port = htons(0);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// bind name to socket
	if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		perror("bind");
		exit(2);
	}
	listen(listener, 3);

	getsockname(listener, (struct sockaddr *)&addr, &len);
	printf("[SERVER] IP addr %s port = %d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
	signal(SIGCHLD, reaper);
	
	while(1){
		
		sock = accept(listener, 0, 0);
        	if(sock < 0){
			perror("accept");
			exit(3);
		} 

		switch(fork()){
			case -1:
				perror("fork");
				break;    
			case 0:
				
				//child
				close(listener);
				while(1){

					bytes_read = recv(sock, buf, BUFLEN, 0);
					if(bytes_read <= 0) break;
				
					buf[bytes_read] = '\0';
					printf("[SERVER] Receiving massege: %s\n", buf);

				}
		 
				close(sock);
				exit(0);
			    
			default:
				close(sock);
			}
	}
	close(listener);
	return 0;
}
