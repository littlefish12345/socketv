#include <socketv.hpp>
#include <utils.hpp>

#include <string>
#include <ws2tcpip.h>

namespace SV {
    int sender::send(const char *data, int length) {
        if (connection_conn_type != tcp) {
            throw exception("SV::sender::send", "unsupport method", 0, "");
        }
        if (!open) {
            throw exception("SV::sender::send", "socket closed", 0, "");
        }
        int all_sended = 0;
        int single_sended;
        while (all_sended < length) {
            single_sended = ::send(socket, &data[all_sended], length - all_sended, 0);
            if (single_sended < 0) {
                if (WSAGetLastError() == WSAECONNRESET) {
                    close();
                }
                throw exception("SV::sender::send", "send error", WSAGetLastError(), get_last_wsa_error_message());
            }
            all_sended += single_sended;
        }
        return all_sended;
    }

    int sender::recv(char *buffer, int buffer_size) {
        if (connection_conn_type != tcp) {
            throw exception("SV::sender::send", "unsupport method", 0, "");
        }
        if (!open) {
            throw exception("SV::sender::recv", "socket closed", 0, "");
        }
        int recv_size = ::recv(socket, buffer, buffer_size, 0);
        if (recv_size < 0) {
            if (WSAGetLastError() == WSAECONNRESET) {
                open = false;
            }
            throw exception("SV::sender::recv", "recv error", WSAGetLastError(), get_last_wsa_error_message());
        }
        return recv_size;
    }

    int sender::sendto(address addr, char *data, int length) {
        if (connection_conn_type != udp) {
            throw exception("SV::sender::sendto", "unsupport method", 0, "");
        }
        if (addr.address_ip_type != local_addr.address_ip_type) {
            throw exception("SV::sender::sendto", "ip type mismatch", 0, "");
        }
        if (!open) {
            throw exception("SV::sender::sendto", "socket closed", 0, "");
        }
        int sended_size;
        if (addr.address_ip_type == ipv4) {
            sockaddr_in sin;
            sin.sin_family = AF_INET;
            sin.sin_addr = *((in_addr *)&addr.address_ipv4_addr);
            sin.sin_port = ::htons(addr.address_port);
            sended_size = ::sendto(socket, data, length, 0, (sockaddr *)&sin, sizeof(sin));
        } else if (addr.address_ip_type == ipv6) {
            sockaddr_in6 sin;
            sin.sin6_family = AF_INET6;
            sin.sin6_addr = *((in6_addr *)&addr.address_ipv6_addr);
            sin.sin6_port = ::htons(addr.address_port);
            sended_size = ::sendto(socket, data, length, 0, (sockaddr *)&sin, sizeof(sin));
        }
        if (sended_size == SOCKET_ERROR) {
            throw exception("SV::sender::sendto", "sendto error", WSAGetLastError(), get_last_wsa_error_message());
        }
        if (!have_local_addr) {
            if (local_addr.address_ip_type == ipv4) {
                sockaddr_in sin;
                int size = sizeof(sin);
                ::getsockname(socket, (sockaddr *)&sin, &size);
                ::inet_ntop(AF_INET, &sin.sin_addr, local_addr.address_ip_str, sizeof(local_addr.address_ip_str));
                local_addr.address_port = ::ntohs(sin.sin_port);
            } else if (local_addr.address_ip_type == ipv6) {
                sockaddr_in6 sin;
                int size = sizeof(sin);
                ::getsockname(socket, (sockaddr *)&sin, &size);
                ::inet_ntop(AF_INET6, &sin.sin6_addr, local_addr.address_ip_str, sizeof(local_addr.address_ip_str));
                local_addr.address_port = ::ntohs(sin.sin6_port);
            }
            have_local_addr = true;
        }
        return sended_size;
    }

    int sender::recvfrom(address *addr, char *buffer, int buffer_size) {
        if (connection_conn_type != udp) {
            throw exception("SV::sender::sendto", "unsupport method", 0, "");
        }
        if (!open) {
            throw exception("SV::sender::sendto", "socket closed", 0, "");
        }
        int addr_len;
        int recv_size;
        if (local_addr.address_ip_type == ipv4) {
            sockaddr_in remote_sin;
            addr_len = sizeof(remote_sin);
            recv_size = ::recvfrom(socket, buffer, buffer_size, 0, (sockaddr *)&remote_sin, &addr_len);
            if (recv_size == SOCKET_ERROR) {
                throw exception("SV::sender::recvfrom", "recvfrom error", WSAGetLastError(), get_last_wsa_error_message());
            }
            addr->address_ip_type = ipv4;
            *((in_addr *)&addr->address_ipv4_addr) = remote_sin.sin_addr;
            ::inet_ntop(AF_INET, &remote_sin.sin_addr, addr->address_ip_str, sizeof(addr->address_ip_str));
            addr->address_port = ::ntohs(remote_sin.sin_port);
        } else if (local_addr.address_ip_type == ipv6) {
            sockaddr_in6 remote_sin;
            addr_len = sizeof(remote_sin);
            recv_size = ::recvfrom(socket, buffer, buffer_size, 0, (sockaddr *)&remote_sin, &addr_len);
            if (recv_size == SOCKET_ERROR) {
                throw exception("SV::sender::recvfrom", "recvfrom error", WSAGetLastError(), get_last_wsa_error_message());
            }
            addr->address_ip_type = ipv6;
            *((in6_addr *)&addr->address_ipv6_addr) = remote_sin.sin6_addr;
            ::inet_ntop(AF_INET6, &remote_sin.sin6_addr, addr->address_ip_str, sizeof(addr->address_ip_str));
            addr->address_port = ::ntohs(remote_sin.sin6_port);
        }
        return recv_size;
    }

    void sender::close() {
        if (!open) {
            return;
        }
        ::closesocket(socket);
        open = false;
    }

    address sender::get_local_addr() {
        if (!have_local_addr) {
            throw exception("SV::sender::get_local_addr", "did not get local address", 0, "");
        }
        return local_addr;
    }

    address sender::get_remote_addr() {
        if (connection_conn_type == udp) {
            throw exception("SV::sender::get_remote_addr", "udp sender do not have remote address", 0, "");
        }
        return remote_addr;
    }
}