#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tcp.h>
#include <dos.h>
#include <conio.h>
/*

gcc dos_server.c -o dos_server -IC:\DJGPP\watt32\inc -LC:\DJGPP\watt32\lib -lwatt

dos_server 1234 master_ip 1234





*/
#define BUFFER_SIZE 1024
#define MAX_CONNECTIONS 10

typedef struct {
    int socket;
    struct sockaddr_in address;
} Connection;

Connection connections[MAX_CONNECTIONS];
int connection_count = 0;

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    FILE *floppy;
    size_t bytes_read;

    floppy = fopen("A:\\", "rb");
    if (!floppy) {
        perror("Failed to open floppy disk");
        closesocket(client_socket);
        return;
    }

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, floppy)) > 0) {
        send(client_socket, buffer, bytes_read, 0);
    }

    fclose(floppy);
    closesocket(client_socket);
}

void display_server_status(int port) {
    struct hostent *local_host;
    char local_ip[16];

    // Get local IP address using Watt-32 function
    if (gethostname(local_ip, sizeof(local_ip)) == 0) {
        local_host = gethostbyname(local_ip);
        if (local_host) {
            strcpy(local_ip, inet_ntoa(*(struct in_addr *)*local_host->h_addr_list));
            printf("Server is live on address: %s, Port: %d\n", local_ip, port);
        } else {
            printf("Failed to get local IP address\n");
        }
    }
}

void display_active_connections() {
    printf("Active Connections:\n");
    for (int i = 0; i < connection_count; i++) {
        char client_ip[INET_ADDRSTRLEN];
        inet_ntoa_r(connections[i].address.sin_addr, client_ip);
        printf("\t%s:%d\n", client_ip, ntohs(connections[i].address.sin_port));
    }
}

void register_with_master_server(const char *master_ip, int master_port, int server_port) {
    tcp_Socket master_socket;
    char buffer[BUFFER_SIZE];

    // Initialize Watt-32
    if (!sock_init()) {
        printf("Failed to initialize Watt-32\n");
        return;
    }

    // Open a connection to the master server
    if (!tcp_open(&master_socket, 0, master_ip, master_port, NULL)) {
        printf("Failed to connect to master server %s:%d\n", master_ip, master_port);
        return;
    }

    // Send registration information
    sprintf(buffer, "REGISTER %s %d", inet_ntoa(my_ip_addr), server_port);
    sock_write(&master_socket, buffer, strlen(buffer));

    // Close the connection
    tcp_close(&master_socket);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s [port] [master_ip] [master_port]\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    const char *master_ip = argv[2];
    int master_port = atoi(argv[3]);
    struct sockaddr_in server_addr, client_addr;
    int server_socket, client_socket;
    socklen_t client_len = sizeof(client_addr);

    // Initialize Watt-32
    if (!sock_init()) {
        printf("Failed to initialize Watt-32\n");
        return 1;
    }

    // Register with the master server
    register_with_master_server(master_ip, master_port, port);

    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to an address and port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        closesocket(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    listen(server_socket, MAX_CONNECTIONS);
    printf("Server listening on port %d\n", port);
    display_server_status(port);

    while (1) {
        // Accept an incoming connection
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        // Handle the client in a new connection
        if (connection_count < MAX_CONNECTIONS) {
            connections[connection_count].socket = client_socket;
            connections[connection_count].address = client_addr;
            connection_count++;
            handle_client(client_socket);
            connection_count--;
        } else {
            printf("Maximum connections reached. Connection refused.\n");
            closesocket(client_socket);
        }

        // Display active connections
        display_active_connections();
    }

    closesocket(server_socket);
    return 0;
}
