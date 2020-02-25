
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>

#define PROTOCOL 0
#define IP_ADDRESS "127.0.0.1"
#define PORT 65535
#define BUFFER_SIZE 128
#define FLAG 0

void clearBuf(char *buffer) {
    int i;
    for (i = 0; i < BUFFER_SIZE; i++)
        buffer[i] = '\0';
}


int recvText(char *buf, int s) {
    int i;
    for (i = 0; i < s; i++) {
        printf("%c", buf[i]);
    }
    return 1;
}

int main() {
    int sock;
    struct sockaddr_in address;
    int address_len = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    char buffer[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_DGRAM,
                  PROTOCOL);

    (sock < 0) ? printf("\nFailed to create socket!\n") : printf("\nCreated socket!\n");

    printf("\nPlease enter text to send to server:\n");
    scanf("%s", buffer);
    printf("\nSending message to server:\n");
    sendto(sock, buffer, BUFFER_SIZE,
           FLAG, (struct sockaddr *) &address,
           address_len);
    while (1) {
        clearBuf(buffer);
        recvfrom(sock, buffer,
                 BUFFER_SIZE, FLAG,
                 (struct sockaddr *) &address, &address_len);

        if (recvText(buffer, BUFFER_SIZE)) {
            break;
        }

    }
}