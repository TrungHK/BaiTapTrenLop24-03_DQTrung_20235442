#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8888
#define BUFLEN 1024

int main() {
    struct sockaddr_in server_addr, client_addr;
    int s, recv_len;
    socklen_t slen = sizeof(client_addr);
    char buf[BUFLEN];

    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("Lỗi tạo socket");
        return 1;
    }

    memset((char *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(s, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Lỗi bind");
        return 1;
    }
    printf("UDP Echo Server đang chạy ở cổng %d...\n", PORT);
    while(1) {
        if ((recv_len = recvfrom(s, buf, BUFLEN - 1, 0, (struct sockaddr *) &client_addr, &slen)) == -1) {
            perror("Lỗi recvfrom");
            continue;
        }
        buf[recv_len] = '\0';
        printf("Nhận được từ %s:%d - Dữ liệu: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buf);
        if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &client_addr, slen) == -1) {
            perror("Lỗi sendto");
        }
    }

    close(s);
    return 0;
}