#pragma once

#include <string>
#include <map>

#if defined(WIN)

#include <winsock2.h>

namespace SV {

}

#elif defined(UNIX)

#include <netinet/in.h>

namespace SV {
    static std::map<std::string, __socket_type> connection_type_map = {{"tcp", SOCK_STREAM}, {"udp", SOCK_DGRAM}};
    static std::map<std::string, int> protocol_type_map = {{"tcp", IPPROTO_TCP}, {"udp", IPPROTO_UDP}};
}

#endif