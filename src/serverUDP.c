
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#define PROTOCOL 0
#define PORT 65535
#define BUFFER_SIZE 128
#define FLAG 0

void clearBuf(char *buffer) {
    int i;
    for (i = 0; i < BUFFER_SIZE; i++)
        buffer[i] = '\0';
}


int main(int argc, char *argv[]) {

    int sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;
    char buffer[BUFFER_SIZE];
    char *message = "hello from the other side";

    sock = socket(AF_INET, SOCK_DGRAM, PROTOCOL);

    sock < 0 ? printf("\nFailed to create socket\n") : printf("\nsocket created!\n");

    if (bind(sock, (struct sockaddr *) &address, sizeof(address)) == 0)
        printf("\nSuccessfully binded to %d!\n", PORT);
    else
        printf("\nFailed to bind to %d\n", PORT);
    while (1) {
        printf("\nWaiting for message.\n");

        // receive file name
        clearBuf(buffer);

        recvfrom(sock, buffer,
                 BUFFER_SIZE, FLAG,
                 (struct sockaddr *) &address, &addrlen);

        printf("\nMessage Received: %s\n", buffer);

        sendto(sock, message, strlen(message),
               FLAG,
               (struct sockaddr *) &address, addrlen);

        clearBuf(buffer);
    }
}