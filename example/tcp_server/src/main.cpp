#include <cstdio>
#include <cstring>
#include <string>
#include <socketv.hpp>

#define BUFFER_SIZE 8192

int main() {
    try {
        SV::startup();
    } catch(std::string e) {
        printf("%s\n", e.c_str());
        return 0;
    }
    SV::socketv sv;
    SV::connector conn;
    SV::listener listener;

    sv.set_backlog(10);

    try {
        listener = sv.listen(SV::tcp, SV::address(SV::ipv6, "::1", 11451));
    } catch(SV::exception e) {
        printf("%s\n", e.what());
        listener.close();
        return 0;
    }

    char buffer[BUFFER_SIZE];
    unsigned int size;

    try {
        conn = listener.accept();
        printf("%s:%hu\n", conn.get_remote_addr().get_ip_str(), conn.get_remote_addr().get_port());
        printf("%s:%hu\n", conn.get_local_addr().get_ip_str(), conn.get_local_addr().get_port());
        while (true) {
            size = conn.recv(buffer, BUFFER_SIZE-1);
            buffer[size] = 0;
            printf("%s\n", buffer);
            if (std::string(buffer) == "exit") {
                conn.close();
                break;
            }
            conn.send(buffer, size);
        }
    } catch(SV::exception e) {
        printf("%s\n", e.what());
        conn.close();
        listener.close();
        return 0;
    }
    
    listener.close();
    return 0;
}