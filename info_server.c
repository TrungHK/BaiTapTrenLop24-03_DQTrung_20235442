#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

// Hàm hỗ trợ đọc chuỗi ký tự kết thúc bằng '\0' từ socket
int recv_string(int socket, char *buffer) {
    char c;
    int i = 0;
    int bytes_received;
    while ((bytes_received = recv(socket, &c, 1, 0)) > 0) {
        buffer[i++] = c;
        if (c == '\0') break;
    }
    return bytes_received > 0 ? i : -1;
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    printf("Server đang chờ kết nối tại port %d...\n", PORT);
    client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    char dirname[1024];
    if (recv_string(client_socket, dirname) > 0) {
        printf("%s\n", dirname);
    }

    char filename[256];
    long filesize;
    while (1) {
        if (recv_string(client_socket, filename) <= 0) break;
        if (strlen(filename) == 0) break;
        recv(client_socket, &filesize, sizeof(long), 0);
        printf("%s - %ld bytes\n", filename, filesize);
    }

    close(client_socket);
    close(server_fd);
    return 0;
}