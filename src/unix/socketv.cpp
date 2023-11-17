#include <socketv.hpp>
#include <utils.hpp>

#include <string>
#include <cerrno>
#include <arpa/inet.h>
#include <sys/socket.h>

namespace SV {
    socketv::socketv() {
        backlog =  SOCKETV_DEFAULT_BACKLOG;
    }

    void socketv::set_backlog(int new_backlog) {
        backlog = new_backlog;
    }

    listener socketv::listen(ConnType connection_type, address addr) {
        listener l;
        l.open = false;
        l.local_addr = addr;
        l.listener_conn_type = connection_type;

        l.socket = ::socket(ip_type_to_af_int(addr.address_ip_type), conn_type_to_type_int(connection_type), conn_type_to_protocol_int(connection_type));
        if (l.socket == -1) {
            throw exception("SV::listen", "invalid socket", errno, get_last_error_message());
        }

        if (addr.address_ip_type == ipv4) {
            sockaddr_in sin;
            sin.sin_family = AF_INET;
            sin.sin_addr = *(in_addr *)&addr.address_ipv4_addr;
            sin.sin_port = ::htons(addr.address_port);
            if (::bind(l.socket, (sockaddr *)&sin, sizeof(sin)) == -1) {
                throw exception("SV::listen", "bind error", errno, get_last_error_message());
            }
        } else if (addr.address_ip_type == ipv6) {
            sockaddr_in6 sin;
            sin.sin6_family = AF_INET6;
            sin.sin6_addr = *(in6_addr *)&addr.address_ipv6_addr;
            sin.sin6_port = ::htons(addr.address_port);
            if (::bind(l.socket, (sockaddr *)&sin, sizeof(sin)) == -1) {
                throw exception("SV::listen", "bind error", errno, get_last_error_message());
            }
        }

        if (connection_type == tcp) {
            if (::listen(l.socket, backlog) == -1) {
                throw exception("SV::listen", "listen error", errno, get_last_error_message());
            }
        }

        l.open = true;

        return l;
    }

    sender socketv::connect(ConnType connection_type, address local_addr, address remote_addr) {
        if (local_addr.address_ip_type != remote_addr.address_ip_type) {
            throw exception("SV::connect", "ip type mismatch", 0, "");
        }

        sender c;
        c.open = false;
        c.have_local_addr = true;
        c.remote_addr = remote_addr;
        c.connection_conn_type = connection_type;

        c.socket = ::socket(ip_type_to_af_int(remote_addr.address_ip_type), conn_type_to_type_int(connection_type), conn_type_to_protocol_int(connection_type));
        if (c.socket == -1) {
            throw exception("SV::connect", "invalid socket", errno, get_last_error_message());
        }

        if (local_addr.address_ip_type == ipv4) {
            sockaddr_in sin;
            sin.sin_family = AF_INET;
            sin.sin_addr = *((in_addr *)&local_addr.address_ipv4_addr);
            sin.sin_port = ::htons(local_addr.address_port);
            if (::bind(c.socket, (sockaddr *)&sin, sizeof(sin)) == -1) {
                throw exception("SV::connect", "bind error", errno, get_last_error_message());
            }
        } else if (local_addr.address_ip_type == ipv6) {
            sockaddr_in6 sin;
            sin.sin6_family = AF_INET6;
            sin.sin6_addr = *((in6_addr *)&local_addr.address_ipv6_addr);
            sin.sin6_port = ::htons(local_addr.address_port);
            if (::bind(c.socket, (sockaddr *)&sin, sizeof(sin)) == -1) {
                throw exception("SV::connect", "bind error", errno, get_last_error_message());
            }
        }

        if (connection_type == tcp) {
            if (remote_addr.address_ip_type == ipv4) {
                sockaddr_in sin;
                sin.sin_family = AF_INET;
                sin.sin_addr = *((in_addr *)&remote_addr.address_ipv4_addr);
                sin.sin_port = ::htons(remote_addr.address_port);
                if (::connect(c.socket, (sockaddr *)&sin, sizeof(sin)) == -1) {
                    throw exception("SV::connect", "connect error", errno, get_last_error_message());
                }
            } else if (remote_addr.address_ip_type == ipv6) {
                sockaddr_in6 sin;
                sin.sin6_family = AF_INET6;
                sin.sin6_addr = *((in6_addr *)&remote_addr.address_ipv6_addr);
                sin.sin6_port = ::htons(remote_addr.address_port);
                if (::connect(c.socket, (sockaddr *)&sin, sizeof(sin)) == -1) {
                    throw exception("SV::connect", "connect error", errno, get_last_error_message());
                }
            }
        }

        if (remote_addr.address_ip_type == ipv4) {
            sockaddr_in sin;
            socklen_t size = sizeof(sin);
            ::getsockname(c.socket, (sockaddr *)&sin, &size);
            ::inet_ntop(AF_INET, &sin.sin_addr, c.local_addr.address_ip_str, sizeof(c.local_addr.address_ip_str));
            c.local_addr.address_port = ::ntohs(sin.sin_port);
        } else if (remote_addr.address_ip_type == ipv6) {
            sockaddr_in6 sin;
            socklen_t size = sizeof(sin);
            ::getsockname(c.socket, (sockaddr *)&sin, &size);
            ::inet_ntop(AF_INET6, &sin.sin6_addr, c.local_addr.address_ip_str, sizeof(c.local_addr.address_ip_str));
            c.local_addr.address_port = ::ntohs(sin.sin6_port);
        }

        c.open = true;

        return c;
    }

    sender socketv::connect(ConnType connection_type, address remote_addr) {
        sender c;
        c.open = false;
        c.have_local_addr = true;
        c.remote_addr = remote_addr;
        c.connection_conn_type = connection_type;

        c.socket = ::socket(ip_type_to_af_int(remote_addr.address_ip_type), conn_type_to_type_int(connection_type), conn_type_to_protocol_int(connection_type));
        if (c.socket == -1) {
            throw exception("SV::connect", "invalid socket", errno, get_last_error_message());
        }

        if (connection_type == tcp) {
            if (remote_addr.address_ip_type == ipv4) {
                sockaddr_in sin;
                sin.sin_family = AF_INET;
                sin.sin_addr = *((in_addr *)&remote_addr.address_ipv4_addr);
                sin.sin_port = ::htons(remote_addr.address_port);
                if (::connect(c.socket, (sockaddr *)&sin, sizeof(sin)) == -1) {
                    throw exception("SV::connect", "connect error", errno, get_last_error_message());
                }
            } else if (remote_addr.address_ip_type == ipv6) {
                sockaddr_in6 sin;
                sin.sin6_family = AF_INET6;
                sin.sin6_addr = *((in6_addr *)&remote_addr.address_ipv6_addr);
                sin.sin6_port = ::htons(remote_addr.address_port);
                if (::connect(c.socket, (sockaddr *)&sin, sizeof(sin)) == -1) {
                    throw exception("SV::connect", "connect error", errno, get_last_error_message());
                }
            }
        }

        if (remote_addr.address_ip_type == ipv4) {
            sockaddr_in sin;
            socklen_t size = sizeof(sin);
            ::getsockname(c.socket, (sockaddr *)&sin, &size);
            ::inet_ntop(AF_INET, &sin.sin_addr, c.local_addr.address_ip_str, sizeof(c.local_addr.address_ip_str));
            c.local_addr.address_port = ::ntohs(sin.sin_port);
        } else if (remote_addr.address_ip_type == ipv6) {
            sockaddr_in6 sin;
            socklen_t size = sizeof(sin);
            ::getsockname(c.socket, (sockaddr *)&sin, &size);
            ::inet_ntop(AF_INET6, &sin.sin6_addr, c.local_addr.address_ip_str, sizeof(c.local_addr.address_ip_str));
            c.local_addr.address_port = ::ntohs(sin.sin6_port);
        }

        c.open = true;

        return c;
    }

    sender socketv::connect(address local_addr) {
        sender c;
        c.open = false;
        c.have_local_addr = true;
        c.connection_conn_type = udp;

        c.socket = ::socket(ip_type_to_af_int(local_addr.address_ip_type), conn_type_to_type_int(udp), conn_type_to_protocol_int(udp));
        if (c.socket == -1) {
            throw exception("SV::connect", "invalid socket", errno, get_last_error_message());
        }

        if (local_addr.address_ip_type == ipv4) {
            sockaddr_in sin;
            sin.sin_family = AF_INET;
            sin.sin_addr = *((in_addr *)&local_addr.address_ipv4_addr);
            sin.sin_port = ::htons(local_addr.address_port);
            if (::bind(c.socket, (sockaddr *)&sin, sizeof(sin)) == -1) {
                throw exception("SV::connect", "bind error", errno, get_last_error_message());
            }
        } else if (local_addr.address_ip_type == ipv6) {
            sockaddr_in6 sin;
            sin.sin6_family = AF_INET6;
            sin.sin6_addr = *((in6_addr *)&local_addr.address_ipv6_addr);
            sin.sin6_port = ::htons(local_addr.address_port);
            if (::bind(c.socket, (sockaddr *)&sin, sizeof(sin)) == -1) {
                throw exception("SV::connect", "bind error", errno, get_last_error_message());
            }
        }

        if (local_addr.address_ip_type == ipv4) {
            sockaddr_in sin;
            socklen_t size = sizeof(sin);
            ::getsockname(c.socket, (sockaddr *)&sin, &size);
            ::inet_ntop(AF_INET, &sin.sin_addr, c.local_addr.address_ip_str, sizeof(c.local_addr.address_ip_str));
            c.local_addr.address_port = ::ntohs(sin.sin_port);
        } else if (local_addr.address_ip_type == ipv6) {
            sockaddr_in6 sin;
            socklen_t size = sizeof(sin);
            ::getsockname(c.socket, (sockaddr *)&sin, &size);
            ::inet_ntop(AF_INET6, &sin.sin6_addr, c.local_addr.address_ip_str, sizeof(c.local_addr.address_ip_str));
            c.local_addr.address_port = ::ntohs(sin.sin6_port);
        }

        c.open = true;

        return c;
    }

    sender socketv::connect(IPType ip_type) {
        sender c;
        c.open = false;
        c.have_local_addr = false;
        c.connection_conn_type = udp;
        c.local_addr.address_ip_type = ip_type;

        c.socket = ::socket(ip_type_to_af_int(ip_type), conn_type_to_type_int(udp), conn_type_to_protocol_int(udp));
        if (c.socket == -1) {
            throw exception("SV::connect", "invalid socket", errno, get_last_error_message());
        }

        c.open = true;

        return c;
    }
}