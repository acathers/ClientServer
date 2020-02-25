
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 65535


int send_file(int socket, char *filename) {

    FILE *file;
    int size, read_size, stat, packet_index;
    char send_buffer[15000], read_buffer[512];
    packet_index = 1;
    file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error Opening Image File");
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    printf("Image size: %i\n", size);

    printf("Sending Image Size\n");
    write(socket, (void *) &size, sizeof(int));

    printf("Sending Image\n");

    do {
        stat = read(socket, &read_buffer, 512);
        printf("Bytes read: %i\n", stat);
    } while (stat < 0);

    printf("Received data in socket\n");
    printf("Socket data: %.*s\n", (int) sizeof(*read_buffer), read_buffer);

    while (!feof(file)) {
        read_size = fread(send_buffer, 1, sizeof(send_buffer) - 1, file);

        do {
            stat = write(socket, send_buffer, read_size);
        }while (stat < 0);

        printf("Packet Number: %i\n", packet_index);
        printf("Packet Size Sent: %i\n", read_size);
        printf(" \n");

        packet_index++;
        bzero(send_buffer, sizeof(send_buffer));
    }
}

int main(int argc, char *argv[]) {
    if (argv[1] == NULL) {
        printf("Error, no image specified");
    }


    int sock_send, sock_accept, addrlen;
    struct sockaddr_in server, client;

    sock_send = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_send == -1) {
        printf("Could not create socket");
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(sock_send, (struct sockaddr *) &server, sizeof(server)) < 0) {
        puts("bind failed");
        return 1;
    }

    puts("bind done");

    listen(sock_send, 3);

    puts("Waiting for incoming connection");
    addrlen = sizeof(struct sockaddr_in);

    if ((sock_accept = accept(sock_send, (struct sockaddr *) &client, (socklen_t *) &addrlen))) {
        puts("Connection accepted");
    }

    fflush(stdout);

    if (sock_accept < 0) {
        perror("Accept Failed");
        return -1;
    }
    printf("%s", argv[1]);
    send_file(sock_accept, argv[1]);

    close(sock_send);
    fflush(stdout);
    return 0;
}