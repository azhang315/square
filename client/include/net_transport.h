#pragma once

#include <cstddef>
#include <event_dispatch.h>
#include <boost/asio.hpp>
#include <string>
#include <unordered_map>
#include <chrono>
#include <log.h>
#include <emscripten/em_asm.h>
#include <array>
#include <thread>

using boost::asio::ip::udp;

class NetTransport : public EventNotifierMixIn<NetTransport>
{
private:
    boost::asio::io_context io_context_;
    udp::socket socket_;
    udp::endpoint server_endpoint_;
    std::array<char, 1024> recv_buffer_;
    uint32_t next_sequence_number_ = 0;

    struct PendingAck
    {
        std::string data;
        std::chrono::steady_clock::time_point timestamp;
    };

    std::unordered_map<uint32_t, PendingAck> pending_acks_;

    void send_message(const std::string &data, bool reliable = false);
    void send_with_sequence(const std::string &data, uint32_t sequence_number);
    void handle_send(const boost::system::error_code &error, std::size_t bytes_transferred);
    void receive_packet();
    void handle_receive(const boost::system::error_code &error, std::size_t bytes_transferred);
    static void init_posix_socket_bridge();

public:
    NetTransport();
    ~NetTransport() = default;

    void start_udp_client(const std::string &server_ip, unsigned short server_port);
    void stop_udp_client();
    void handle_event(EventPtr<CanvasLocalUpdateEvent> e);
};
