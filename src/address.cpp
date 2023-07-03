#include <string>
#include <map>

#include "socketv.hpp"

using namespace SV;

#if defined(WIN)

#elif defined(UNIX)

#include <errno.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>

address::address() {

}

address::address(std::string ip_type_str, std::string ip_str, unsigned short port_num) {
    if (ip_type_str == "ipv4") {
        ip_type = 4;
        if (ip_str == "") {
            ipv4_addr.s_addr = ::htonl(INADDR_ANY);
        } else {
            if (::inet_pton(AF_INET, ip_str.c_str(), &ipv4_addr) != 1) {
                throw std::string("addr invalid");
            }
        }
    } else if (ip_type_str == "ipv6") {
        ip_type = 6;
        if (ip_str == "") {
            ipv6_addr = IN6ADDR_ANY_INIT;
        } else {
            if (::inet_pton(AF_INET6, ip_str.c_str(), &ipv6_addr) != 1) {
                throw std::string("addr invalid");
            }
        }
    } else {
        throw std::string("ip type invalid");
    }
    ip_string = ip_str;
    port = port_num;
}

address::address(in_addr ipv4_addr_internal, unsigned short port_num) {
    ip_type = 4;
    ipv4_addr = ipv4_addr_internal;
    port = port_num;
    char buffer[INET_ADDRSTRLEN];
    const char *ptr = inet_ntop(AF_INET, &ipv4_addr_internal, buffer, sizeof(buffer));
    if (ptr == NULL) {
        throw std::string("internal ipv4 struct invalid: [Errno ") + std::to_string(errno) + std::string("] ") +  std::string(::strerror(errno));
    }
    ip_string = std::string(ptr);
}

address::address(in6_addr ipv6_addr_internal, unsigned short port_num) {
    ip_type = 6;
    ipv6_addr = ipv6_addr_internal;
    port = port_num;
    char buffer[INET6_ADDRSTRLEN];
    const char *ptr = inet_ntop(AF_INET6, &ipv6_addr_internal, buffer, sizeof(buffer));
    if (ptr == NULL) {
        throw std::string("internal ipv4 struct invalid: [Errno ") + std::to_string(errno) + std::string("] ") +  std::string(::strerror(errno));
    }
    ip_string = std::string(ptr);
}

std::string address::get_ip_string() {
    return ip_string;
}

unsigned short address::get_port() {
    return port;
}

#endif