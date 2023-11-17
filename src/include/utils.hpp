#pragma once

#include <socketv.hpp>

#include <string>

#if defined(WINDOWS)

#include <ws2tcpip.h>

#elif defined(UNIX)

#include <netinet/in.h>

#endif

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

    inline int get_last_error_code() {
        #if defined(WINDOWS)

        return WSAGetLastError();
    
        #elif defined(UNIX)

        return errno;

        #endif
    }

    inline std::string get_last_error_message() {
        #if defined(WINDOWS)

        char *err_message = NULL;

        int result = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&err_message, 0, NULL);

        if (result == 0) {
            return "";
        }

        std::string err_std_str = err_message;
        LocalFree(err_message);

        return err_std_str;

        #elif defined(UNIX)

        return std::string(::strerror(errno));

        #endif
    }
}