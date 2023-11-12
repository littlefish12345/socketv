#include <socketv.hpp>
#include <utils.hpp>

#include <cstring>
#include <string>
#include <ws2tcpip.h>

namespace SV {
    address::address() {
        address_ip_type = ipv4;
        memset(address_ipv6_addr, 0, sizeof(address_ipv6_addr));
        memset(address_ip_str, 0, sizeof(address_ip_str));
        address_port = 0;
    }

    address::address(IPType ip_type, const char *ip_str, unsigned short port) {
        address_ip_type = ip_type;
        address_port = port;
        if (ip_type == ipv4) {
            if (std::string(ip_str) == "") {
                *((unsigned long long *)&address_ipv4_addr[0]) = INADDR_ANY;
            } else if (::inet_pton(AF_INET, ip_str, (in_addr *)&address_ipv4_addr) != 1) {
                throw exception("SV::listen", "invalid address", 0, "");
            }
            ::inet_ntop(AF_INET, (in_addr *)&address_ipv4_addr, address_ip_str, sizeof(address_ip_str));
        } else if (ip_type == ipv6) {
            if (std::string(ip_str) == "") {
                *((in6_addr *)&address_ipv6_addr) = IN6ADDR_ANY_INIT;
            } else if (::inet_pton(AF_INET6, ip_str, (in6_addr *)&address_ipv6_addr) != 1) {
                throw exception("SV::listen", "invalid address", 0, "");
            }
            ::inet_ntop(AF_INET6, (in6_addr *)&address_ipv6_addr, address_ip_str, sizeof(address_ip_str));
        }
    }

    char *address::get_ip_str() {
        return address_ip_str;
    }

    unsigned short address::get_port() {
        return address_port;
    }
}