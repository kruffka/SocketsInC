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

#define BUFLEN 256

int main(int argc, char *argv[]){

	int n = 5;
	char msg[BUFLEN];
    
	int sock;
	struct sockaddr_in addr;
	void sigalarm(int);
	if(argc!=3){

		printf("Need ip and port.\n");
		exit(1);

	}

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(sock < 0){

		perror("socket");
		exit(1);

	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2])); 
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	
	if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){

		perror("connect");
		exit(2);
    
	}

	
	for(int i = 1; i <= n; i++){

		// записываем число i в msg и отправляем на сервер
		sprintf(msg, "%d", i);
		send(sock, msg, sizeof(msg), 0);
		printf("Send to server message: %s and sleeping %d sec\n", msg, i);

		sleep(i);
	}
    
	close(sock);
	return 0;
}