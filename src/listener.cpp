#include <string>

#include "socketv.hpp"

using namespace SV;

#if defined(WIN)

#elif defined(UNIX)

#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>

listener::listener() {

}

listener::listener(int sock, address local_addr) {
    socket = sock;
    addr = local_addr;
}

connection listener::accept() {
    address remote_addr;
    int conn;
    if (addr.ip_type == 4) {
        sockaddr_in addr;
        socklen_t addr_len = sizeof(addr);
        conn = ::accept(socket, (sockaddr *)&addr, &addr_len);
        if (conn == -1) {
            throw std::string("accept failed: [Errno ") + std::to_string(errno) + std::string("] ") + std::string(::strerror(errno));
        }
        remote_addr = address(addr.sin_addr, addr.sin_port);
    } else if (addr.ip_type == 6) {
        sockaddr_in6 addr;
        socklen_t addr_len = sizeof(addr);
        conn = ::accept(socket, (sockaddr *)&addr, &addr_len);
        if (conn == -1) {
            throw std::string("accept failed: [Errno ") + std::to_string(errno) + std::string("] ") + std::string(::strerror(errno));
        }
        remote_addr = address(addr.sin6_addr, addr.sin6_port);
    }
    return connection(conn, remote_addr);
}

void listener::close() {

}

#endif