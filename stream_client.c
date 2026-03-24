#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8081

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Lỗi tạo socket \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\n Địa chỉ không hợp lệ \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\n Kết nối thất bại \n");
        return -1;
    }

    printf("Đã kết nối đến server.\n");
    printf("Nhập văn bản (gõ 'exit' để thoát):\n");
    while (1) {
        printf("> ");
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = 0;

            if (strcmp(buffer, "exit") == 0) {
                break;
            }
            int len = strlen(buffer);
            if (len > 0) {
                send(sock, buffer, len, 0);
            }
        }
    }

    close(sock);
    return 0;
}