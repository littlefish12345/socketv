#include <socketv.hpp>
#include <utils.hpp>

#include <ws2tcpip.h>

namespace SV {
    void startup() {
        WSADATA wsadata;
        if (::WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
            throw exception("SV::startup", "WSAStartup error", WSAGetLastError(), get_last_error_message());
        }
    }

    void cleanup() {
        ::WSACleanup();
    }
}