#include <cstdio>
#include <cstring>
#include <string>
#include <socketv.hpp>

#define BUFFER_SIZE 32768

int main() {
    try {
        SV::startup();
    } catch(std::string e) {
        printf("%s\n", e.c_str());
        return 0;
    }

    SV::socketv sv;
    SV::sender conn;
    try {
        conn = sv.connect(SV::tcp, SV::address(SV::ipv6, "::1", 11451));
    } catch(SV::exception e) {
        printf("%s\n", e.what());
        conn.close();
        return 0;
    }

    printf("%s:%hu\n", conn.get_remote_addr().get_ip_str(), conn.get_remote_addr().get_port());
    printf("%s:%hu\n", conn.get_local_addr().get_ip_str(), conn.get_local_addr().get_port());
        

    char buffer[BUFFER_SIZE];
    unsigned int size;
    try {
        while (true) {
            scanf("%s", &buffer);
            size = strlen(buffer);
            conn.send(buffer, size);
            if (std::string(buffer) == "exit") {
                conn.close();
                break;
            }
            size = conn.recv(buffer, BUFFER_SIZE-1);
            buffer[size] = 0;
            printf("%s\n", buffer);
        }
    } catch(SV::exception e)  {
        printf("%s\n", e.what());
        conn.close();
        return 0;
    }
    conn.close();
    return 0;
}