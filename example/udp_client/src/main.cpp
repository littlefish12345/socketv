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
    SV::address remote_addr = SV::address(SV::ipv6, "::1", 11451);
    try {
        conn = sv.connect(SV::ipv6);
    } catch(SV::exception e) {
        printf("%s\n", e.what());
        conn.close();
        return 0;
    }
        
    char buffer[BUFFER_SIZE];
    unsigned int size;
    try {
        while (true) {
            scanf("%s", &buffer);
            size = strlen(buffer);
            conn.sendto(remote_addr, buffer, size);
            printf("%s:%hu\n", remote_addr.get_ip_str(), remote_addr.get_port());
            printf("%s:%hu\n", conn.get_local_addr().get_ip_str(), conn.get_local_addr().get_port());
            if (std::string(buffer) == "exit") {
                conn.close();
                break;
            }
            size = conn.recvfrom(&remote_addr, buffer, BUFFER_SIZE-1);
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