#include <cstdio>
#include <cstring>
#include <socketv.hpp>

#define BUFFER_SIZE 8192

int main() {
    SV::connection conn;
    try {
        SV::socketv sv;
        conn = sv.connect("tcp", "ipv4", "127.0.0.1", 11451);
    } catch(std::string e) {
        printf("%s\n", e.c_str());
        conn.close();
        return 0;
    }
    try {
        unsigned char buffer[BUFFER_SIZE];
        unsigned int size;
        while (true) {
            scanf("%s", &buffer);
            size = strlen((char *)buffer);
            conn.send(buffer, size);
            conn.recv(buffer, BUFFER_SIZE);
            printf("%s", buffer);
        }
    } catch(std::string e)  {
        printf("%s\n", e.c_str());
        conn.close();
        return 0;
    }
    conn.close();
    return 0;
}