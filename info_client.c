#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Kết nối thất bại\n");
        return -1;
    }

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        
        send(sock, cwd, strlen(cwd) + 1, 0);
    }

    DIR *dir;
    struct dirent *ent;
    struct stat file_stat;

    if ((dir = opendir(cwd)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            
            if (ent->d_type == DT_REG) {
                stat(ent->d_name, &file_stat);
                long size = file_stat.st_size;
                    send(sock, ent->d_name, strlen(ent->d_name) + 1, 0);
                    send(sock, &size, sizeof(long), 0);
            }
        }
        closedir(dir);
    }
        char end_marker = '\0';
    send(sock, &end_marker, 1, 0);
    close(sock);
    return 0;
}