#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
int main(){
    int sockfd;
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t len = sizeof(client_addr);
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    // 2. Bind (Server MUST bind to receive data)
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    printf("UDP Server waiting for data on Port %d...\n", PORT);

    // 3. Chat Loop
    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);

        //fills client addr with the sender details.
        int n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, 0,
                         (struct sockaddr *)&client_addr, &len);
        buffer[n] = '\0'; 
        printf("Client: %s", buffer);

        if (strncmp(buffer, "exit", 4) == 0)
        {
            printf("Server Exit...\n");
            break;
        }

        // Reply
        memset(message, 0, BUFFER_SIZE);
        printf("Server: ");
        fgets(message, BUFFER_SIZE, stdin);

        sendto(sockfd, (const char *)message, strlen(message), 0,
               (const struct sockaddr *)&client_addr, len);

        if (strncmp(message, "exit", 4) == 0)
        {
            break;
        }
    }

    close(sockfd);
    return 0;
}
