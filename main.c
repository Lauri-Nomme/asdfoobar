#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

int main() {
    int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    inet_pton(AF_INET, "145.239.233.232", &(sa.sin_addr));
    sa.sin_port = htons(1723);

    int res = connect(s, (const struct sockaddr*)&sa, sizeof(sa));
    printf("connect - %d\n", res);

    res = fcntl(s, F_SETFL, fcntl(s, F_GETFL, 0) | O_NONBLOCK);
    printf("set nonblocking - %d\n", res);

    int val = 2;
    res = setsockopt(s, SOL_TCP, TCP_KEEPCNT, &val, 4);
    printf("setsockopt(.., SOL_TCP(%08X), TCP_KEEPCNT(%08X), 2) - %d\n", SOL_TCP, TCP_KEEPCNT, res);
    val = 5;
    res = setsockopt(s, SOL_TCP, TCP_KEEPINTVL, &val, 4);
    printf("setsockopt(.., SOL_TCP(%08X), TCP_KEEPINTVL(%08X), 2) - %d\n", SOL_TCP, TCP_KEEPINTVL, res);
    res = setsockopt(s, SOL_TCP, TCP_KEEPIDLE, &val, 4);
    printf("setsockopt(.., SOL_TCP(%08X), TCP_KEEPIDLE(%08X), 2) - %d\n", SOL_TCP, TCP_KEEPIDLE, res);
    val = 1;
    res = setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &val, 4);
    printf("setsockopt(.., SOL_SOCKET(%08X), SO_KEEPALIVE(%08X), 2) - %d\n", SOL_SOCKET, SO_KEEPALIVE, res);

    do {
        char buf[1];
        int res = recv(s, &buf, sizeof(buf), MSG_PEEK);
        printf("recv(.., 1, MSG_PEEK(%08X) - %d\n", MSG_PEEK, res);
        if (res == 0) {
            break;
        }
        sleep(1);
    } while (1);

    close(s);

    return 0;
}
