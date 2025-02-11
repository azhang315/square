// net_transport.h
#pragma once
#include <cstddef>
#include <event_dispatch.h>
#include <spdlog/spdlog.h>
#include <boost/asio.hpp>
#include <string>
#include <unordered_map>
#include <chrono>

using boost::asio::ip::udp;

class NetTransport : public EventNotifierMixIn<NetTransport> {
private:
    udp::socket socket_;
    udp::endpoint server_endpoint_;
    boost::asio::io_context io_context_;
    std::array<char, 1024> recv_buffer_;
    uint32_t next_sequence_number_ = 0;

    struct PendingAck {
        std::string data;
        std::chrono::steady_clock::time_point timestamp;
    };

    std::unordered_map<uint32_t, PendingAck> pending_acks_;

    void send_message(const std::string& data, bool reliable = false) {
        if (reliable) {
            uint32_t sequence_number = next_sequence_number_++;
            pending_acks_[sequence_number] = {data, std::chrono::steady_clock::now()};
            send_with_sequence(data, sequence_number);
        } else {
            socket_.async_send_to(
                boost::asio::buffer(data),
                server_endpoint_,
                [this](auto error, auto bytes_transferred) { handle_send(error, bytes_transferred); }
            );
        }
    }

    void send_with_sequence(const std::string& data, uint32_t sequence_number) {
        std::string packet = std::to_string(sequence_number) + ":" + data;
        socket_.async_send_to(
            boost::asio::buffer(packet),
            server_endpoint_,
            [this](auto error, auto bytes_transferred) { handle_send(error, bytes_transferred); }
        );
    }

    void handle_send(const boost::system::error_code& error, std::size_t bytes_transferred) {
        if (!error) {
            spdlog::info("Successfully sent {} bytes.", bytes_transferred);
        } else {
            spdlog::error("Send failed: {}", error.message());
        }
    }

    void receivePacket() {
        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), server_endpoint_,
            [this](auto error, auto bytes_transferred) { handle_receive(error, bytes_transferred); }
        );
    }

    void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred) {
        if (!error && bytes_transferred > 0) {
            std::string message(recv_buffer_.data(), bytes_transferred);
            spdlog::info("Received: {}", message);

            // Handle ACKs
            auto delimiter_pos = message.find(":");
            if (delimiter_pos != std::string::npos && message.substr(0, 4) == "ACK:") {
                uint32_t seq_num = std::stoul(message.substr(4));
                pending_acks_.erase(seq_num);
                spdlog::info("ACK received for sequence number: {}", seq_num);
            }
        }
        receivePacket(); // Keep listening for more packets
    }

public:
    NetTransport() : socket_(io_context_) {}
    ~NetTransport() = default;

    void start_client(const std::string& server_ip, unsigned short server_port) {
        socket_.open(udp::v4());
        server_endpoint_ = udp::endpoint(boost::asio::ip::make_address(server_ip), server_port);
        receivePacket();
        io_context_.run();
    }

    void handle_event(const Event<CanvasLocalUpdateEvent>& e) {
        spdlog::info("NT <- CANVAS LOCAL");
        send_message("Canvas update", true); // echo server
    }
};
