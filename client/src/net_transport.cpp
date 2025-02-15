// net_transport.cpp

#include <emscripten.h>
#include <emscripten/posix_socket.h>
#include <log.h>
#include <net_transport.h>
#include <emscripten/threading.h>

void NetTransport::init_posix_socket_bridge() {
    SLOG("NetTransport::init_posix_socket_bridge");

    #ifndef WS_PROXY_BRIDGE_IP
    #endif

    constexpr const char *bridge_url = "ws://127.0.0.1:8080/";
    EMSCRIPTEN_WEBSOCKET_T bridgeSocketFd = emscripten_init_websocket_to_posix_socket_bridge(bridge_url);
    SLOG("bridgeFD: {}", bridgeSocketFd);

    // Synchronously wait until connection has been established.
    uint16_t readyState = 0;
    do {
        SLOG("iter");

      emscripten_websocket_get_ready_state(bridgeSocketFd, &readyState);
      emscripten_thread_sleep(100);
    } while (readyState == 0);
}


#include "net_transport.h"

NetTransport::NetTransport() : io_context_(), socket_(io_context_)
{
    SLOG("NetTransport::init()");
    init_posix_socket_bridge();
}

void NetTransport::send_message(const std::string &data, bool reliable)
{
    if (reliable)
    {
        uint32_t sequence_number = next_sequence_number_++;
        pending_acks_[sequence_number] = {data, std::chrono::steady_clock::now()};
        send_with_sequence(data, sequence_number);
    }
    else
    {
        socket_.async_send_to(
            boost::asio::buffer(data),
            server_endpoint_,
            [this](auto error, auto bytes_transferred)
            { handle_send(error, bytes_transferred); });
    }
}

void NetTransport::send_with_sequence(const std::string &data, uint32_t sequence_number)
{
    std::string packet = std::to_string(sequence_number) + ":" + data;
    socket_.async_send_to(
        boost::asio::buffer(packet),
        server_endpoint_,
        [this](auto error, auto bytes_transferred)
        { handle_send(error, bytes_transferred); });
}

void NetTransport::handle_send(const boost::system::error_code &error, std::size_t bytes_transferred)
{
    if (!error)
    {
        SLOG("Successfully sent {} bytes.", bytes_transferred);
    }
    else
    {
        spdlog::error("Send failed: {}", error.message());
    }
}

void NetTransport::receive_packet()
{
    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_), server_endpoint_,
        [this](auto error, auto bytes_transferred)
        { handle_receive(error, bytes_transferred); });
}

void NetTransport::handle_receive(const boost::system::error_code &error, std::size_t bytes_transferred)
{
    if (!error && bytes_transferred > 0)
    {
        std::string message(recv_buffer_.data(), bytes_transferred);
        SLOG("Received: {}", message);

        // Handle ACKs
        auto delimiter_pos = message.find(":");
        if (delimiter_pos != std::string::npos && message.substr(0, 4) == "ACK:")
        {
            uint32_t seq_num = std::stoul(message.substr(4));
            pending_acks_.erase(seq_num);
            SLOG("ACK received for sequence number: {}", seq_num);
        }
    }
    receive_packet(); // Keep listening for more packets
}

void NetTransport::start_udp_client(const std::string &server_ip, unsigned short server_port)
{
    SLOG("Network client: start");
    try
    {
        int is_worker = EM_ASM_INT(return ENVIRONMENT_IS_WORKER);
        assert(is_worker);
        server_endpoint_ = udp::endpoint(boost::asio::ip::make_address(server_ip), server_port);
        receive_packet();
        std::thread networkThread([this]()
                                  {
            SLOG("thread: udp client recv");
            io_context_.run(); });
        networkThread.detach();
    }
    catch (const std::exception &e)
    {
        SLOG("Exception in start_udp_client: {}", e.what());
    }
}

void NetTransport::stop_udp_client()
{
    io_context_.stop();
    SLOG("Network client: stopped");
}

void NetTransport::handle_event(EventPtr<CanvasLocalUpdateEvent> e)
{
    SLOG("NT <- CANVAS LOCAL");
    send_message("Canvas update", false); // echo server
    e->data;
}