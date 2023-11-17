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
    SV::listener listener;

    sv.set_backlog(10);

    try {
        listener = sv.listen(SV::udp, SV::address(SV::ipv6, "::1", 11451));
    } catch(SV::exception e) {
        printf("%s\n", e.what());
        listener.close();
        return 0;
    }

    char buffer[BUFFER_SIZE];
    unsigned int size;

    try {
        SV::address addr;
        while (true) {
            size = listener.recvfrom(&addr, buffer, BUFFER_SIZE-1);
            printf("%s:%hu\n", addr.get_ip_str(), addr.get_port());
            printf("%s:%hu\n", listener.get_local_addr().get_ip_str(), listener.get_local_addr().get_port());
            buffer[size] = 0;
            printf("%s\n", buffer);
            if (std::string(buffer) == "exit") {
                listener.close();
                break;
            }
            listener.sendto(addr, buffer, size);
        }
    } catch(SV::exception e) {
        printf("%s\n", e.what());
        listener.close();
        return 0;
    }
    
    listener.close();
    return 0;
}