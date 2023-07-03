#include <string>
#include <vector>

#include "socketv.hpp"

using namespace SV;

#if defined(WIN)

#elif defined(UNIX)

#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>

connection::connection() {

}

connection::connection(int conn, address remote_addr) {
    socket = conn;
    addr = remote_addr;
}

address connection::get_remote_addr() {
    return addr;
}

inline void connection::send(unsigned char *data, size_t length, bool block) {
    size_t all_sended_length = 0;
    ssize_t single_sended_length = 0;
    while(all_sended_length < length) {
        single_sended_length = ::send(socket, data, length, block ? 0 : MSG_DONTWAIT);
        if (single_sended_length < 0) {
            throw std::string("send failed: ") + std::string(::strerror(errno));
        }
        all_sended_length += single_sended_length;
    }
}

inline void connection::send(std::vector<unsigned char> data, bool block) {
    connection::send(&data[0], data.size());
}

inline void connection::send(std::string data, bool block) {
    connection::send((unsigned char*)data.c_str(), data.size());
}

void connection::send(unsigned char *data, size_t length) {
    connection::send(data, length, true);
}

void connection::send(std::vector<unsigned char> data) {
    connection::send(data, true);
}

void connection::send(std::string data) {
    connection::send(data, true);
}

inline size_t connection::recv(unsigned char *buffer, size_t buffer_size, bool block) {
    ssize_t recv_size = ::recv(socket, buffer, buffer_size, block ? 0 : MSG_DONTWAIT);
    if (recv_size < 0) {
        throw std::string("send failed: ") + std::string(::strerror(errno));
    }
    return recv_size;
}

inline std::vector<unsigned char> connection::recv(size_t max_size, bool block) {
    std::vector<unsigned char> buffer(max_size);
    connection::recv(&buffer[0], max_size, block);
    return buffer;
}

size_t connection::recv(unsigned char *buffer, size_t buffer_size) {
    return connection::recv(buffer, buffer_size, true);
}

std::vector<unsigned char> connection::recv(size_t max_size) {
    return connection::recv(max_size);
}

void connection::recv_all(unsigned char *buffer, size_t size) {
    ssize_t recv_size = ::recv(socket, buffer, size,  MSG_WAITALL);
    if (recv_size < 0) {
        throw std::string("send failed: ") + std::string(::strerror(errno));
    }
}

std::vector<unsigned char> connection::recv_all(size_t max_size) {
    std::vector<unsigned char> buffer(max_size);
    connection::recv_all(&buffer[0], max_size);
    return buffer;
}


void connection::close() {
    ::close(socket);
}

#endif