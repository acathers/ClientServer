
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<netdb.h>
#include<errno.h>

#define SIZE 15000
#define PORT 65535

int receive_image(int socket) {

    int recv_size = 0, size = 0, read_size, write_size, packet_index = 1, stat;

    char image_buffer[SIZE];
    FILE *image;

    do {
        stat = read(socket, &size, sizeof(int));
    } while (stat < 0);

    printf("Packet received.\n");
    printf("Packet size: %i\n", stat);
    printf("Image size: %i\n", size);
    printf(" \n");

    char *buffer = "Got file";

    do {
        stat = write(socket, &buffer, sizeof(buffer));
    } while (stat < 0);

    printf("Reply sent\n");
    printf(" \n");

    image = fopen("received.jpeg", "w");

    if (image == NULL) {
        printf("Error has occurred. Image file could not be opened\n");
        return -1;
    }



    struct timeval timeout = {10, 0};

    fd_set fds;
    int buffer_fd;

    while (recv_size < size) {

        FD_ZERO(&fds);
        FD_SET(socket, &fds);

        buffer_fd = select(FD_SETSIZE, &fds, NULL, NULL, &timeout);

        if (buffer_fd < 0)
            printf("error: bad file descriptor set.\n");

        if (buffer_fd == 0)
            printf("error: buffer read timeout expired.\n");

        if (buffer_fd > 0) {
            do {
                read_size = read(socket, image_buffer, SIZE);
            } while (read_size < 0);

            printf("Packet number received: %i\n", packet_index);
            printf("Packet size: %i\n", read_size);


            //Write the currently read data into our image file
            write_size = fwrite(image_buffer, 1, read_size, image);
            printf("Written image size: %i\n", write_size);

            if (read_size != write_size) {
                printf("error in read write\n");
            }

            recv_size += read_size;
            packet_index++;
            printf("Total received image size: %i\n", recv_size);
            printf(" \n");
            printf(" \n");
        }

    }

    fclose(image);
    printf("Image successfully Received!\n");
    return 1;
}

void checkHostName(int hostname) {
    if (hostname == -1) {
        perror("gethostname");
        exit(1);
    }
}

void checkHostEntry(struct hostent *hostentry) {
    if (hostentry == NULL) {
        perror("gethostbyname");
        exit(1);
    }
}

int main(int argc, char *argv[]) {

    int socket_desc;
    struct sockaddr_in server;
    char hostbuffer[256];
    char *IPbuffer;
    struct hostent *host_entry;
    int hostname;

    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    checkHostName(hostname);

    host_entry = gethostbyname(hostbuffer);
    checkHostEntry(host_entry);

    IPbuffer = inet_ntoa(*((struct in_addr *)
            host_entry->h_addr_list[0]));

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc == -1) {
        printf("Could not create socket");
    }

    memset(&server, 0, sizeof(server));
    server.sin_addr.s_addr = inet_addr(IPbuffer);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if (connect(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
        printf("%s", strerror(errno));
        close(socket_desc);
        puts("Connect Error");
        return 1;
    }

    puts("Connected\n");
    receive_image(socket_desc);
    close(socket_desc);
    return 0;
}

