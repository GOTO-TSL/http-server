#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define SERV_ADDR "127.0.0.1"
#define SERV_PORT 8080
#define BUF_SIZE 1024

int http(int sock);
int send_msg(int sock, char *msg);

int send_msg(int sock, char *msg) {
    char *send_buf;
    int len;
    int send_size;

    send_buf = msg;
    len = strlen(msg);
    send_size = write(sock, &send_buf, len);
    if (send_size == -1) {
        printf("send error\n");
        return -1;
    }
    printf("%d\n", send_size);
    return 0;
}

int http(int sock) {
    char recv_buf[BUF_SIZE], *buf;
    int recv_size;
    
    recv_size = read(sock, &recv_buf, BUF_SIZE);
    if (recv_size == -1) {
        printf("recieve error\n");
        return -1;
    }

    send_msg(sock, "HTTP/1.1 200 OK\r\n");
    send_msg(sock, "Content-Type: text/html\r\n");
    send_msg(sock, "\r\n");

    printf("%s\n", recv_buf);

    fgets(buf, 1024, stdin);
    write(sock, buf, strlen(buf));

    return 0;
}

int main(void) {
    int sockfd;
    int new_sockfd;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket error\n");
        return -1;
    }
    printf("socket created\n");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);


    // bind
    if (bind(sockfd, (const struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        printf("bind error\n");
        fprintf(stderr, "%s\n", strerror(errno));
        return -1;
    }

    // listen
    if (listen(sockfd, 5) == -1) {
        printf("listen error\n");
        return -1;
    }

    while(1) {
        printf("waiting for connection...\n");

        new_sockfd = accept(sockfd, NULL, NULL);
        if (new_sockfd == -1) {
            printf("accept error\n");
            return -1;
        }

        printf("connecting!\n");

        http(new_sockfd);

        close(new_sockfd);
    }

    close(sockfd);

    return 0;
}