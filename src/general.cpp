#include <socketv.hpp>
#include <utils.hpp>

#if defined(WINDOWS)

#include <ws2tcpip.h>

#elif defined(UNIX)

#endif

namespace SV {
    void startup() {
        #if defined(WINDOWS)

        WSADATA wsadata;
        if (::WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
            throw exception("SV::startup", "WSAStartup error", get_last_error_code(), get_last_error_message());
        }

        #elif defined(UNIX)

        #endif
    }

    void cleanup() {
        #if defined(WINDOWS)

        ::WSACleanup();

        #elif defined(UNIX)

        #endif
    }
}