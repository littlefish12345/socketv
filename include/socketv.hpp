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
            */
            listener listen(std::string connection_type, address addr);

            /*
                connection_type: tcp/udp
                ip_type: ipv4/ipv6
            */
            listener listen(std::string connection_type, std::string ip_type, std::string ip_str, unsigned short port);

            connection connect(std::string connection_type, address remote_addr);

            connection connect(std::string connection_type, std::string ip_type, std::string remote_ip_str, unsigned short remote_port);

        private:
            #if defined(WIN)
                WSADATA ws;
            #elif defined(UNIX)
            #endif
    };
}