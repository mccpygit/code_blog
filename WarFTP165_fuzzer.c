#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 21

const char bufferA[203] =
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n";

int main(int args, char *argv[]){
	int sockfd;
	struct sockaddr_in host_addr;

	socklen_t sin_size;
	int send_rcv = 1, yes=1;
	char buffer[1024], payload[5000] = "USER ";

	host_addr.sin_family = AF_INET;
	host_addr.sin_port = htons(PORT);
	if (inet_aton(argv[1], &(host_addr.sin_addr)) == 0){
		printf("Address is invalid!");
		return 0;
	}
	memset(&(host_addr.sin_zero), '\0', 8);

	while(1){	
		if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) > 0){
			printf("Socket successfully created!\n");
		}
		else{
			printf("Failed to create socket");
			exit(0);
		}

		if(connect(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
		{
			printf("Connection failed or fuzzing was complete!!\n");
			close(sockfd);
			return 0;
		}

		send_rcv = recv(sockfd, buffer, 1024, 0);
		printf("Number of bytes recv is %d\n", send_rcv);
		printf("%s\n",buffer);


		strcat(payload, bufferA);
		printf("Sending %d bytes!\nData: %s\n\n", strlen(payload), payload);
		if (send_rcv = send(sockfd, payload, strlen(payload), 0) < 1)
		{
			close(sockfd);
			return 0;
		}

		payload[strlen(payload) - 2] = '\x00';
		close(sockfd);
		printf("Socket Closed.\n");
		sleep(2);
	}

	return 0;
}
