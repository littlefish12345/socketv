#pragma once

#if defined(WINDOWS)

#include <ws2tcpip.h>

#define SOCKETV_INVALID_SOCKET INVALID_SOCKET
#define SOCKETV_SOCKET_ERROR SOCKET_ERROR

#define socketv_closesocket(s) ::closesocket(s)

typedef int socketv_socklen;
typedef SOCKADDR socketv_sockaddr;

#elif defined(UNIX)

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define SOCKETV_INVALID_SOCKET -1
#define SOCKETV_SOCKET_ERROR -1

#define socketv_closesocket(s) ::close(s)

typedef socklen_t socketv_socklen;
typedef sockaddr socketv_sockaddr;

#endif