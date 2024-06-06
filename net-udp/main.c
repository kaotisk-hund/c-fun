#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>

int main(int argc, char *argv[]) {
	int sockfd;
	struct sockaddr_in6 addr;
	const int port = 9999; // Choose your desired port number

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <interface_ipv6_address>\n", argv[0]);
		return 1;
	}

    printf("Creating socket... ");
	// Create UDP socket
	sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
	if (sockfd == -1) {
		perror("socket");
		return 1;
	}
    printf("Done!\n");

	// Initialize socket address structure
	memset(&addr, 0, sizeof(addr));
	addr.sin6_family = AF_INET6;
	addr.sin6_port = htons(port);

    printf("Converting ip?!... ");
	// Convert interface IPv6 address from command line argument
	if (inet_pton(AF_INET6, argv[1], &addr.sin6_addr) != 1) {
		perror("inet_pton");
		close(sockfd);
		return 1;
	}
    printf("Done!\n");

    printf("Binding... ");
	// Bind socket to the specified interface
	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		perror("bind");
		close(sockfd);
		return 1;
	}
    printf("Done!\n");

	printf("UDP socket bound to [%s]:%d\n", argv[1], port);

	// You can now use the socket for sending/receiving data
	// For example, use recvfrom() and sendto() to communicate over UDP
    char buffer[520] = {0};
    recv(sockfd, &buffer, sizeof(buffer), 0);
    printf("Received: %s\n", buffer);
    char reply[] = "Thank you!";
    send(sockfd, &reply, sizeof(reply), 0);


	// Close the socket when done
	close(sockfd);

	return 0;
}

