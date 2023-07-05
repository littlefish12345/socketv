#pragma once

#include <string>

#ifndef UNIX
#define UNIX
#endif

#if defined(WIN)

#elif defined(UNIX)

#include <netinet/in.h>

#endif

namespace SV {
    class address {
        friend class socketv;
        friend class listener;
        friend class connection;

        public:
            address();

            /*
                connection_type: tcp/udp
                ip_type: ipv4/ipv6
            */
            address(std::string ip_type_str, std::string ip_str, unsigned short port);

            std::string get_ip_string();
            unsigned short get_port();
        private:
            address(in_addr ipv4_addr, unsigned short port);
            address(in6_addr ipv6_addr, unsigned short port);
            std::string ip_str;
            unsigned short port;
            unsigned char ip_type;
            #if defined(WIN)
            #elif defined(UNIX)
                in_addr ipv4_addr;
                in6_addr ipv6_addr;
            #endif
    };
}