#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8081
#define BUFFER_SIZE 1024

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

    printf("Stream Server đang chờ kết nối tại port %d...\n", PORT);
    client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    printf("Client đã kết nối!\n");

    char buffer[BUFFER_SIZE];
    char target[] = "0123456789";
    int target_len = strlen(target); 
    
    char tail[10] = ""; 
    char search_buffer[BUFFER_SIZE + 10]; 
    int total_count = 0;
    int bytes_received;

    while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_received] = '\0'; 
        printf("[Nhận được]: %s\n", buffer);

        strcpy(search_buffer, tail);
        strcat(search_buffer, buffer);

        char *ptr = search_buffer;
        int count_this_round = 0;
        while ((ptr = strstr(ptr, target)) != NULL) {
            total_count++;
            count_this_round++;
            ptr += target_len;
        }

        int current_len = strlen(search_buffer);
        if (current_len >= target_len - 1) {
            strcpy(tail, search_buffer + current_len - (target_len - 1));
        } else {
            strcpy(tail, search_buffer);
        }

        if (count_this_round > 0) {
             printf("=> Đã tìm thấy chuỗi! Tổng số lần xuất hiện: %d\n", total_count);
        }
    }

    close(client_socket);
    close(server_fd);
    return 0;
}