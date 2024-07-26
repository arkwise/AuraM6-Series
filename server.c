#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netdb.h>

#define MAX_SHARES 100
#define BUFFER_SIZE 1024
#define MAX_CONNECTIONS 100

typedef struct {
    char hostname[256];
    char first_name[50];
    char last_name[50];
    char share_name[256];
} Share;

typedef struct {
    int socket;
    struct sockaddr_in address;
    pthread_t thread;
} Connection;

Share shares[MAX_SHARES];
Connection connections[MAX_CONNECTIONS];
int share_count = 0;
int connection_count = 0;
pthread_mutex_t share_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t connection_mutex = PTHREAD_MUTEX_INITIALIZER;
int server_running = 1;

void* handle_client(void* client_socket) {
    int socket = *(int*)client_socket;
    free(client_socket);

    char buffer[BUFFER_SIZE];
    int bytes_received = recv(socket, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        // Process incoming data (e.g., share registration, share query)
        // This is a simplified example, real implementation would require proper parsing and handling.
        printf("Received: %s\n", buffer);
    }

    close(socket);
    pthread_mutex_lock(&connection_mutex);
    for (int i = 0; i < connection_count; i++) {
        if (connections[i].socket == socket) {
            connections[i] = connections[connection_count - 1];
            connection_count--;
            break;
        }
    }
    pthread_mutex_unlock(&connection_mutex);
    return NULL;
}

void display_server_status(int port) {
    struct ifaddrs *ifaddr, *ifa;
    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    printf("Server is live on the following addresses:\n");
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) continue;
        int family = ifa->ifa_addr->sa_family;

        if (family == AF_INET || family == AF_INET6) {
            int s = getnameinfo(ifa->ifa_addr,
                                (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6),
                                host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (s != 0) {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }
            printf("\tAddress: %s, Port: %d\n", host, port);
        }
    }
    freeifaddrs(ifaddr);
}

void* server_loop(void* arg) {
    int server_socket = *(int*)arg;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    while (server_running) {
        int *client_socket = malloc(sizeof(int));
        *client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (*client_socket < 0) {
            if (!server_running) break; // Exit if server stopped
            perror("Accept failed");
            free(client_socket);
            continue;
        }

        pthread_mutex_lock(&connection_mutex);
        if (connection_count < MAX_CONNECTIONS) {
            connections[connection_count].socket = *client_socket;
            connections[connection_count].address = client_addr;
            pthread_create(&connections[connection_count].thread, NULL, handle_client, client_socket);
            pthread_detach(connections[connection_count].thread);
            connection_count++;
        } else {
            close(*client_socket);
            free(client_socket);
            printf("Maximum connections reached. Connection refused.\n");
        }
        pthread_mutex_unlock(&connection_mutex);
    }
    
    close(server_socket);
    return NULL;
}

void display_active_connections() {
    pthread_mutex_lock(&connection_mutex);
    printf("Active Connections:\n");
    for (int i = 0; i < connection_count; i++) {
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(connections[i].address.sin_addr), client_ip, INET_ADDRSTRLEN);
        printf("\t%s:%d\n", client_ip, ntohs(connections[i].address.sin_port));
    }
    pthread_mutex_unlock(&connection_mutex);
}

void start_server(int *server_socket, pthread_t *server_thread) {
    struct sockaddr_in server_addr;

    *server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (*server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(1234);

    if (bind(*server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(*server_socket);
        exit(EXIT_FAILURE);
    }

    listen(*server_socket, 5);
    printf("Master server listening on port 1234\n");
    display_server_status(1234);

    server_running = 1;
    pthread_create(server_thread, NULL, server_loop, server_socket);
}

void stop_server(int server_socket, pthread_t server_thread) {
    server_running = 0;
    close(server_socket);
    pthread_join(server_thread, NULL);
    printf("Server stopped.\n");
}

void reset_server(int *server_socket, pthread_t *server_thread) {
    stop_server(*server_socket, *server_thread);
    start_server(server_socket, server_thread);
    printf("Server reset.\n");
}

void* command_interface(void* arg) {
    int server_socket = *(int*)arg;
    pthread_t server_thread = *(pthread_t*)(arg + sizeof(int));

    char command[BUFFER_SIZE];
    while (1) {
        printf("Enter command (start/stop/reset/status/exit): ");
        fgets(command, BUFFER_SIZE, stdin);
        command[strcspn(command, "\n")] = 0; // Remove newline character

        if (strcmp(command, "start") == 0) {
            start_server(&server_socket, &server_thread);
        } else if (strcmp(command, "stop") == 0) {
            stop_server(server_socket, server_thread);
        } else if (strcmp(command, "reset") == 0) {
            reset_server(&server_socket, &server_thread);
        } else if (strcmp(command, "status") == 0) {
            display_server_status(1234);
            display_active_connections();
        } else if (strcmp(command, "exit") == 0) {
            stop_server(server_socket, server_thread);
            exit(0);
        } else {
            printf("Unknown command.\n");
        }
    }
    return NULL;
}

int main() {
    int server_socket;
    pthread_t server_thread;

    start_server(&server_socket, &server_thread);

    pthread_t command_thread;
    pthread_create(&command_thread, NULL, command_interface, &server_socket);
    pthread_join(command_thread, NULL);

    return 0;
}
