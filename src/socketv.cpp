#include <string>

#include "socketv.hpp"
#include "net_enum_map.hpp"

using namespace SV;

#if defined(WIN)

#include <winsock2.h>

socketv::socketv() {
    int err = WSAStartup(MAKEWORD(2, 2), &ws);
    if (err != 0) {
        throw std::string("WSAStartup failed: ") + std::to_string(err);
    }
}

#elif defined(UNIX)


#include <errno.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>

socketv::socketv() {

}

listener socketv::listen(std::string connection_type, address addr) {
    int sock;

    if (addr.ip_type == 4) {
        sock = ::socket(AF_INET, connection_type_map[connection_type], protocol_type_map[connection_type]);
        if (sock == -1) {
            throw std::string("create socket failed: [Errno ") + std::to_string(errno) + std::string("] ") + std::string(::strerror(errno));
        }

        int opt = 1;
        ::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        struct sockaddr_in sock_addr;
        ::memset(&sock_addr, 0, sizeof(sock_addr));
        sock_addr.sin_family = AF_INET;
        sock_addr.sin_addr = addr.ipv4_addr;
        sock_addr.sin_port = ::htons(addr.port);
        if (::bind(sock, (sockaddr *)&sock_addr, sizeof(sock_addr)) == -1) {
            throw std::string("bind addr failed: [Errno ") + std::to_string(errno) + std::string("] ") + std::string(::strerror(errno));
        }
    } else if (addr.ip_type == 6) {
        sock = ::socket(AF_INET6, connection_type_map[connection_type], protocol_type_map[connection_type]);
        if (sock == -1) {
            throw std::string("create socket failed: [Errno ") + std::to_string(errno) + std::string("] ") + std::string(::strerror(errno));
        }

        int opt = 1;
        ::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        struct sockaddr_in6 sock_addr;
        ::memset(&sock_addr, 0, sizeof(sock_addr));
        sock_addr.sin6_family = AF_INET6;
        sock_addr.sin6_addr = addr.ipv6_addr;
        sock_addr.sin6_port = ::htons(addr.port);
        if (::bind(sock, (sockaddr *)&sock_addr, sizeof(sock_addr)) == -1) {
            throw std::string("bind addr failed: [Errno ") + std::to_string(errno) + std::string("] ") + std::string(::strerror(errno));
        }
    } else {
        throw std::string("uninitialized address struct");
    }

    if (::listen(sock, 4096) == -1) {
        throw std::string("listen failed: [Errno ") + std::to_string(errno) + std::string("] ") + std::string(::strerror(errno));
    }

    return listener(sock, connection_type, addr);
}

listener socketv::listen(std::string connection_type, std::string ip_type, std::string ip_str, unsigned short port) {
    return socketv::listen(connection_type, address(ip_type, ip_str, port));
}

connection socketv::connect(std::string connection_type, address remote_addr) {
    int sock;

    if (remote_addr.ip_type == 4) {
        sock = ::socket(AF_INET, connection_type_map[connection_type], protocol_type_map[connection_type]);
        if (sock == -1) {
            throw std::string("create socket failed: [Errno ") + std::to_string(errno) + std::string("] ") + std::string(::strerror(errno));
        }

        struct sockaddr_in remote_sock_addr;
        ::memset(&remote_sock_addr, 0, sizeof(remote_sock_addr));
        remote_sock_addr.sin_family = AF_INET;
        remote_sock_addr.sin_addr = remote_addr.ipv4_addr;
        remote_sock_addr.sin_port = ::htons(remote_addr.port);
        if (::connect(sock, (sockaddr *)&remote_sock_addr, sizeof(remote_sock_addr)) == -1) {
            throw std::string("connect failed: [Errno ") + std::to_string(errno) + std::string("] ") + std::string(::strerror(errno));
        }
    } else if (remote_addr.ip_type == 6) {
        sock = ::socket(AF_INET6, connection_type_map[connection_type], protocol_type_map[connection_type]);
        if (sock == -1) {
            throw std::string("create socket failed: [Errno ") + std::to_string(errno) + std::string("] ") + std::string(::strerror(errno));
        }

        struct sockaddr_in6 remote_sock_addr;
        ::memset(&remote_sock_addr, 0, sizeof(remote_sock_addr));
        remote_sock_addr.sin6_family = AF_INET;
        remote_sock_addr.sin6_addr = remote_addr.ipv6_addr;
        remote_sock_addr.sin6_port = ::htons(remote_addr.port);
        if (::connect(sock, (sockaddr *)&remote_sock_addr, sizeof(remote_sock_addr)) == -1) {
            throw std::string("connect failed: [Errno ") + std::to_string(errno) + std::string("] ") + std::string(::strerror(errno));
        }
    } else {
        throw std::string("uninitialized address struct");
    }

    return  connection(sock, remote_addr);
}

connection socketv::connect(std::string connection_type, std::string ip_type, std::string remote_ip_str, unsigned short remote_port) {
    return socketv::connect(connection_type, address(ip_type, remote_ip_str, remote_port));
}

#endif