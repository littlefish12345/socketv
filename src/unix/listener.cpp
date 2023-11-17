#include <socketv.hpp>
#include <utils.hpp>

#include <string>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

namespace SV {
    sender listener::accept() {
        if (!open) {
            throw exception("SV::listener::accept", "socket closed", 0, "");
        }
    
        sender c;
        c.open = false;
        c.have_local_addr = true;
        c.local_addr = local_addr;
    
        address remote_addr;
        remote_addr.address_ip_type = local_addr.address_ip_type;
        if (local_addr.address_ip_type == ipv4) {
            sockaddr_in remote_sin;
            socklen_t addr_len = sizeof(remote_sin);
            c.socket = ::accept(socket, (sockaddr *)&remote_sin, &addr_len);
            if (c.socket == -1) {
                throw exception("SV::listener::accept", "accept error", errno, get_last_error_message());
            }
            ::inet_ntop(AF_INET, &remote_sin.sin_addr, remote_addr.address_ip_str, sizeof(remote_addr.address_ip_str));
            remote_addr.address_port = ::ntohs(remote_sin.sin_port);
        } else if (local_addr.address_ip_type == ipv6) {
            sockaddr_in6 remote_sin;
            socklen_t addr_len = sizeof(remote_sin);
            c.socket = ::accept(socket, (sockaddr *)&remote_sin, &addr_len);
            if (c.socket == -1) {
                throw exception("SV::listener::accept", "accept error", errno, get_last_error_message());
            }
            ::inet_ntop(AF_INET6, &remote_sin.sin6_addr, remote_addr.address_ip_str, sizeof(remote_addr.address_ip_str));
            remote_addr.address_port = ::ntohs(remote_sin.sin6_port);
        }

        c.remote_addr = remote_addr;
        c.open = true;

        return c;
    }

    int listener::sendto(address addr, char *data, int length) {
        if (listener_conn_type != udp) {
            throw exception("SV::listener::sendto", "unsupport method", 0, "");
        }
        if (addr.address_ip_type != local_addr.address_ip_type) {
            throw exception("SV::listener::sendto", "ip type mismatch", 0, "");
        }
        if (!open) {
            throw exception("SV::listener::sendto", "socket closed", 0, "");
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
        if (sended_size == -1) {
            throw exception("SV::listener::sendto", "sendto error", errno, get_last_error_message());
        }
        return sended_size;
    }

    int listener::recvfrom(address *addr, char *buffer, int buffer_size) {
        if (listener_conn_type != udp) {
            throw exception("SV::listener::sendto", "unsupport method", 0, "");
        }
        if (!open) {
            throw exception("SV::listener::sendto", "socket closed", 0, "");
        }
        socklen_t addr_len;
        int recv_size;
        if (local_addr.address_ip_type == ipv4) {
            sockaddr_in remote_sin;
            addr_len = sizeof(remote_sin);
            recv_size = ::recvfrom(socket, buffer, buffer_size, 0, (sockaddr *)&remote_sin, &addr_len);
            if (recv_size == -1) {
                throw exception("SV::listener::recvfrom", "recvfrom error", errno, get_last_error_message());
            }
            addr->address_ip_type = ipv4;
            *((in_addr *)&addr->address_ipv4_addr) = remote_sin.sin_addr;
            ::inet_ntop(AF_INET, &remote_sin.sin_addr, addr->address_ip_str, sizeof(addr->address_ip_str));
            addr->address_port = ::ntohs(remote_sin.sin_port);
        } else if (local_addr.address_ip_type == ipv6) {
            sockaddr_in6 remote_sin;
            addr_len = sizeof(remote_sin);
            recv_size = ::recvfrom(socket, buffer, buffer_size, 0, (sockaddr *)&remote_sin, &addr_len);
            if (recv_size == -1) {
                throw exception("SV::listener::recvfrom", "recvfrom error", errno, get_last_error_message());
            }
            addr->address_ip_type = ipv6;
            *((in6_addr *)&addr->address_ipv6_addr) = remote_sin.sin6_addr;
            ::inet_ntop(AF_INET6, &remote_sin.sin6_addr, addr->address_ip_str, sizeof(addr->address_ip_str));
            addr->address_port = ::ntohs(remote_sin.sin6_port);
        }
        return recv_size;
    }

    void listener::close() {
        if (!open) {
            return;
        }
        ::close(socket);
        open = true;
    }

    address listener::get_local_addr() {
        return local_addr;
    }
}