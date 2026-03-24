#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8888
#define SERVER "127.0.0.1"
#define BUFLEN 1024

int main() {
    struct sockaddr_in server_addr;
    int s, slen = sizeof(server_addr);
    char buf[BUFLEN];
    char message[BUFLEN];

    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("Lỗi tạo socket");
        return 1;
    }

    memset((char *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_aton(SERVER, &server_addr.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() failed\n");
        return 1;
    }

    while(1) {
        printf("Nhập tin nhắn (Gõ 'exit' để thoát): ");
        fgets(message, BUFLEN, stdin);
        message[strcspn(message, "\n")] = 0;

        if (strcmp(message, "exit") == 0) break;
        if (sendto(s, message, strlen(message), 0, (struct sockaddr *) &server_addr, slen) == -1) {
            perror("Lỗi sendto");
            continue;
        }
        memset(buf, '\0', BUFLEN);
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &server_addr, (socklen_t*)&slen) == -1) {
            perror("Lỗi recvfrom");
            continue;
        }

        puts("Server phản hồi: ");
        puts(buf);
    }

    close(s);
    return 0;
}