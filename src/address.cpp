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

address::address(std::string ip_type_str, std::string ip_str, unsigned short port): ip_str(ip_str), port(port) {
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
}

address::address(in_addr ipv4_addr, unsigned short port): ipv4_addr(ipv4_addr), port(port) {
    ip_type = 4;
    char buffer[INET_ADDRSTRLEN];
    const char *ptr = inet_ntop(AF_INET, &ipv4_addr, buffer, sizeof(buffer));
    if (ptr == NULL) {
        throw std::string("internal ipv4 struct invalid: [Errno ") + std::to_string(errno) + std::string("] ") +  std::string(::strerror(errno));
    }
    ip_str = std::string(ptr);
}

address::address(in6_addr ipv6_addr, unsigned short port): ipv6_addr(ipv6_addr), port(port) {
    ip_type = 6;
    char buffer[INET6_ADDRSTRLEN];
    const char *ptr = inet_ntop(AF_INET6, &ipv6_addr, buffer, sizeof(buffer));
    if (ptr == NULL) {
        throw std::string("internal ipv4 struct invalid: [Errno ") + std::to_string(errno) + std::string("] ") +  std::string(::strerror(errno));
    }
    ip_str = std::string(ptr);
}

std::string address::get_ip_string() {
    return ip_str;
}

unsigned short address::get_port() {
    return port;
}

#endif