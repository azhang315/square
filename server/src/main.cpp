// udp_server.cpp
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <string>

using boost::asio::ip::udp;

class MinimalUDPServer {
public:
    MinimalUDPServer(boost::asio::io_context& io_context, unsigned short port)
        : socket_(io_context, udp::endpoint(udp::v4(), port)) {
        start_receive();
    }

private:
    void start_receive() {
        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), remote_endpoint_,
            boost::bind(&MinimalUDPServer::handle_receive, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred)
        );
    }

    void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred) {
        if (!error && bytes_transferred > 0) {
            std::string message(recv_buffer_.data(), bytes_transferred);
            std::cout << "Received: " << message << std::endl;

            // Echo the message back to the client
            socket_.async_send_to(
                boost::asio::buffer(message), remote_endpoint_,
                boost::bind(&MinimalUDPServer::handle_send, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred)
            );

            // If the message contains a sequence number, send an ACK
            auto delimiter_pos = message.find(":");
            if (delimiter_pos != std::string::npos) {
                std::string seq_str = message.substr(0, delimiter_pos);
                std::string ack = "ACK:" + seq_str;
                socket_.async_send_to(
                    boost::asio::buffer(ack), remote_endpoint_,
                    boost::bind(&MinimalUDPServer::handle_send, this,
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred)
                );
            }
        }

        start_receive(); // Continue listening for more messages
    }

    void handle_send(const boost::system::error_code& error, std::size_t bytes_transferred) {
        if (!error) {
            std::cout << "Sent " << bytes_transferred << " bytes." << std::endl;
        } else {
            std::cerr << "Send failed: " << error.message() << std::endl;
        }
    }

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::array<char, 1024> recv_buffer_;
};

int main() {
    try {
        boost::asio::io_context io_context;
        MinimalUDPServer server(io_context, 8080);

        std::cout << "UDP server listening on port 8080..." << std::endl;
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
