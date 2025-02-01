// net_transport.hpp
#pragma once
#include <cstddef>

class NetTransport {
public:
    NetTransport();
    ~NetTransport();

    void sendPacket(const char* data, size_t size);
    void receivePacket();
};