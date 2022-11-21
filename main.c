/**
 * This program returns a list of IP addresses for the given hostname
 */

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

/* error-checking wrappers */
static void Getaddrinfo(const char *restrict node, const char *restrict service,
			const struct addrinfo *restrict hints,
			struct addrinfo **restrict res);

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("USAGE: %s <hostname>\n", argv[0]);
		exit(1);
	}

	char *hostname = argv[1];

	struct addrinfo hints;
	struct addrinfo *info;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	Getaddrinfo(hostname, NULL, &hints, &info);

	for (struct addrinfo *p = info; p != NULL; p = p->ai_next) {
		void *addr;
		char ipstr[INET6_ADDRSTRLEN];

		if (p->ai_family == AF_INET) {
			// v4
			struct sockaddr_in *ipv4 =
				(struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr)
		} else {
			// v6
			struct sockaddr_in6 *ipv6 =
				(struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
		}

		inet_ntop(p->ai_family, addr, ipstr, INET6_ADDRSTRLEN);
		printf("%s\n", ipstr);
	}

	freeaddrinfo(info);
	exit(0);
}

static void Getaddrinfo(const char *restrict node, const char *restrict service,
			const struct addrinfo *restrict hints,
			struct addrinfo **restrict res) {
	int status = getaddrinfo(node, service, hints, res);
	if (status) {
		fprintf(stderr, "getaddrinfo error: %s\n",
			gai_strerror(status));
		exit(1);
	}
}
