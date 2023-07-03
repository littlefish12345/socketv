#pragma once

#include <string>

#include "connection.hpp"

namespace SV {
    class listener {
        friend class socketv;

        public:
            listener();
            address get_local_address();
            connection accept();
            void close();
        private:
            address addr;
            #if defined(WIN)
            #elif defined(UNIX)
                listener(int sock, address local_addr);
                int socket;
            #endif
    };
}