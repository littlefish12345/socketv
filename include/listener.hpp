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
            std::string connection_type;
            address local_addr;
            #if defined(WIN)
            #elif defined(UNIX)
                listener(int socket, std::string connection_type, address local_addr);
                int socket;
            #endif
    };
}