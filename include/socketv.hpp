#pragma once

#include <string>
#include <exception>
#include <cstring>

namespace SV {
    #define SOCKETV_DEFAULT_BACKLOG 10
    #define SOCKETV_IP_STRING_LENGTH 50

    enum ConnType {
        tcp,
        udp
    };

    enum IPType {
        ipv4,
        ipv6
    };

    class exception : public std::exception {
        public:
        int error_code;
            std::string system_message;
            std::string func_call;
            std::string func_message;
            exception(std::string func_call, std::string func_message, int error_code, std::string system_message) : func_call(func_call), func_message(func_message), error_code(error_code), system_message(system_message) {}
            virtual const char *what() const noexcept override {
                std::string s = (func_call + " " + func_message + (error_code == 0 ? "" : ": [Errno " + std::to_string(error_code) + (system_message == "" ? "]" : "] " + system_message)));
                char *str = (char *)malloc(s.size() +1);
                memcpy(str, s.c_str(), s.size());
                str[s.size()] = '\0';
                return str;
            }
    };

    class address {
        friend class socketv;
        friend class listener;
        friend class sender;
        public:
            address();
            address(IPType ip_type, const char *ip_str, unsigned short port);
            char *get_ip_str();
            unsigned short get_port();
        private:
            IPType address_ip_type;
            union {
                unsigned char address_ipv4_addr[4];
                unsigned char address_ipv6_addr[16];
            };
            char address_ip_str[SOCKETV_IP_STRING_LENGTH];
            unsigned short address_port;
    };

    class sender {
        friend class socketv;
        friend class listener;
        public:
            int send(const char *data, int length);
            int recv(char *buffer, int buffer_size);
            int sendto(address addr, char *data, int length);
            int recvfrom(address *addr, char *buffer, int buffer_size);
            void close();
            address get_local_addr();
            address get_remote_addr();
        private:
            bool open;
            bool have_local_addr;
            ConnType connection_conn_type;
            address local_addr;
            address remote_addr;
            unsigned long long socket;
    };

    class listener {
        friend class socketv;
        public:
            sender accept();
            int sendto(address addr, char *data, int length);
            int recvfrom(address *addr, char *buffer, int buffer_size);
            void close();
            address get_local_addr();
        private:
            bool open;
            ConnType listener_conn_type;
            address local_addr;
            unsigned long long socket;
    };

    class socketv {
        public:
            socketv();
            void set_backlog(int new_backlog);

            listener listen(ConnType connection_type, address addr);
            sender connect(ConnType connection_type, address local_addr, address remote_addr);
            sender connect(ConnType connection_type, address remote_addr);
            sender connect(address local_addr); //only for udp
            sender connect(IPType ip_type); //only for udp

        private:
            int backlog;
    };

    void startup();
    void cleanup();
}