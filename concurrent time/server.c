#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define PORT 8888
#define BUF_SIZE 1024

int main()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(1);
    }

    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    char buffer[BUF_SIZE];
    while (1) {
        memset(buffer, 0, BUF_SIZE);
        socklen_t client_len = sizeof(client_addr);
        int recv_len = recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr*)&client_addr, &client_len);
        if (recv_len < 0) {
            perror("Error receiving data from client");
            exit(1);
        }

        if (strcmp(buffer, "TIME\n") == 0) {
            char client[40];
            strcpy(client,inet_ntoa(client_addr.sin_addr));
            printf("Received the time request from client %s:%d\n",client,ntohs(client_addr.sin_port));
            memset(buffer, 0, BUF_SIZE);
            time_t t = time(NULL);
            snprintf(buffer, BUF_SIZE, "%s\n", ctime(&t));
            int send_len = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
        if (send_len < 0) {
                perror("Error sending data to client");
                exit(1);
            }
        printf("Sent time %s to client %s:%d\n",buffer,client,ntohs(client_addr.sin_port));
        }
    }

    close(sockfd);
    return 0;
}
