#include <cstdio>
#include <socketv.hpp>

#define BUFFER_SIZE 8192

int main() {
    SV::connection conn;
    SV::listener listener;
    try {
        SV::socketv sv;
        listener = sv.listen("tcp", "ipv4", "0.0.0.0", 11451);
        conn = listener.accept();
    } catch(std::string e) {
        printf("%s\n", e.c_str());
        conn.close();
        listener.close();
        return 0;
    }
    try {
        printf("%s:%d\n", conn.get_remote_addr().get_ip_string().c_str(), conn.get_remote_addr().get_port());
        unsigned char buffer[BUFFER_SIZE];
        unsigned int size;
        while (true) {
            size = conn.recv(buffer, BUFFER_SIZE);
            conn.send(buffer, size);
        }
    } catch(std::string e)  {
        printf("%s\n", e.c_str());
        conn.close();
        listener.close();
        return 0;
    }
    conn.close();
    listener.close();
    return 0;
}