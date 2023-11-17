#pragma once

#include <socketv.hpp>

#include <cstring>
#include <string>
#include <cerrno>
#include <netinet/in.h>

namespace SV {
    inline int ip_type_to_af_int(IPType ip_type) {
        if (ip_type == ipv4) {
            return AF_INET;
        } else if (ip_type == ipv6) {
            return AF_INET6;
        }
        return -1;
    }

    inline int conn_type_to_type_int(ConnType connection_type) {
        if (connection_type == tcp) {
            return SOCK_STREAM;
        } else if (connection_type == udp) {
            return SOCK_DGRAM;
        }
        return -1;
    }

    inline int conn_type_to_protocol_int(ConnType connection_type) {
        if (connection_type == tcp) {
            return IPPROTO_TCP;
        } else if (connection_type == udp) {
            return IPPROTO_UDP;
        }
        return -1;
    }

    inline std::string get_last_error_message() {
        return std::string(::strerror(errno));
    }
}