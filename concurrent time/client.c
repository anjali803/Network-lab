#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8888
#define BUF_SIZE 1024

int main()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        exit(1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    char buffer[BUF_SIZE];
    while (1)
    {
        printf("Enter 'TIME' to get the current system time, or 'QUIT' to exit: ");
        fgets(buffer, BUF_SIZE, stdin);

        if (strcmp(buffer, "QUIT\n") == 0)
        {
            break;
        }

        int send_len = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (send_len < 0)
        {
            perror("Error sending data to server");
            exit(1);
        }

        memset(buffer, 0, BUF_SIZE);
        int recv_len = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);
        if (recv_len < 0)
        {
            perror("Error receiving data from server");
            exit(1);
        }

        printf("Server time: %s", buffer);
    }

    close(sockfd);
    return 0;
}