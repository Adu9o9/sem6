#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(){
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    printf("Connected to Server! Type 'exit' to quit.\n\n");
    while (1)
    {
        // WRITE to Server
        memset(message, 0, BUFFER_SIZE);
        printf("Client: ");
        fgets(message, BUFFER_SIZE, stdin);
        write(sock, message, strlen(message));

        if (strncmp(message, "exit", 4) == 0)
        {
            printf("Client Exit...\n");
            break;
        }

        // READ from Server
        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(sock, buffer, BUFFER_SIZE);
        if (valread <= 0)
        {
            printf("Server disconnected.\n");
            break;
        }
        printf("Server: %s", buffer);

        if (strncmp(buffer, "exit", 4) == 0)
        {
            break;
        }
    }

    close(sock);
    return 0;
}
