#pragma once

#include <string>

#include "address.hpp"
#include "listener.hpp"
#include "connection.hpp"

namespace SV {
    class socketv {
        public:
            socketv();

            /*
                connection_type: tcp/udp
                ip_type: ipv4/ipv6
            */
            listener listen(std::string connection_type, std::string ip_type, std::string ip_str, unsigned short port);

            /*
                connection_type: tcp/udp
            */
            listener listen(std::string connection_type, address addr);

        private:
            #if defined(WIN)
                WSADATA ws;
            #elif defined(UNIX)
            #endif
    };
}