#pragma once

#include <string>
#include <vector>

namespace SV {
    class connection {
        friend class socketv;
        friend class listener;

        public:
            connection();
            address get_remote_addr();

            void send(unsigned char *data, size_t length, bool block);
            void send(std::vector<unsigned char> data, bool block);
            void send(std::string data, bool block);

            void send(unsigned char *data, size_t length); //default block
            void send(std::vector<unsigned char> data);
            void send(std::string data);
    
            size_t recv(unsigned char *buffer, size_t buffer_size, bool block);
            std::vector<unsigned char> recv(size_t max_size, bool block);
            size_t recv(unsigned char *buffer, size_t buffer_size);
            std::vector<unsigned char> recv(size_t max_size);

            void recv_all(unsigned char *buffer, size_t size);
            std::vector<unsigned char> recv_all(size_t size);

            void close();
        private:
            address addr;
            #if defined(WIN)
            #elif defined(UNIX)
                connection(int socket, address remote_addr);
                int socket;
            #endif
    };
}