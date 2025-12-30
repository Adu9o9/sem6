#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(){
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successfully (FD: %d)\n", server_fd);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Bind successful on Port %d\n", PORT);
    if (listen(server_fd, 3) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on Port %d...\n", PORT);
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    printf("Client connected!\n\n");

    //chat loop
    while(1){
        memset(buffer, 0, BUFFER_SIZE);
        //read from client
        int valread = read(new_socket, buffer, BUFFER_SIZE);//read() waits for data
        if (valread <= 0)
        {
            printf("Client disconnected.\n");
            break;
        }
        printf("Client: %s", buffer);
        if (strncmp(buffer, "exit", 4) == 0)
        {
            printf("Server Exit...\n");
            break;
        }

        memset(message, 0, BUFFER_SIZE);
        printf("Server: ");
        fgets(message, BUFFER_SIZE, stdin); // Get input from keyboard
        write(new_socket, message, strlen(message));

        if (strncmp(message, "exit", 4) == 0)
        {
            break;
        }
    }
    close(new_socket);
    close(server_fd);
    return 0;
}
