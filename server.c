#include <sys/fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define SERV_ADDR "127.0.0.1"
#define SERV_PORT 8080
#define BUF_SIZE 1024

// 関数のプロトタイプ宣言
void http(int sockfd);
int send_msg(int sockfd, char *msg);

// メイン関数
int main(void) {
    int sockfd;
    int new_sockfd;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket error\n");
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);


    // bind
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
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

        http(new_sockfd);

        close(new_sockfd);
    }

    close(sockfd);

    return 0;
}

// http通信を行う関数
void http(int sockfd) {
    char recv_buf[BUF_SIZE];
    int recv_size;
    char *method, *uri_addr, *http_ver;

    
    recv_size = read(sockfd, recv_buf, BUF_SIZE);
    if (recv_size == -1) {
        printf("recieve error\n");
    } else {
        send_msg(sockfd, "HTTP/1.1 200 OK\r\n");
        send_msg(sockfd, "Content-Type: text/html\r\n");
        send_msg(sockfd, "\r\n");

        sscanf(recv_buf, "%s %s %s", method, uri_addr, http_ver);

        if (strcmp(method, "GET") == 0) {
            printf("GET method\n");
        } else {
            send_msg(sockfd, "501 Not implemented.");
        }
    }
}

// レスポンスを送信する関数
int send_msg(int sockfd, char *msg) {
    int len;
    int send_size;

    len = strlen(msg);
    send_size = write(sockfd, msg, len);
    if (send_size == -1) {
        printf("send error\n");
        return -1;
    }
    return len;
}