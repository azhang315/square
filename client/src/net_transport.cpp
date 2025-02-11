// // net_transport.cpp
// #include <net_transport.h>
// // udp_client.cpp
// #include <iostream>
// #include <boost/bind/bind.hpp>
// #include <string>
// #include <unordered_map>
// #include <chrono>

// using boost::asio::ip::udp;

// class DatagramClient {
// public:
//     DatagramClient(boost::asio::io_context& io_context, const std::string& server_ip, unsigned short server_port)
//         : socket_(io_context), server_endpoint_(boost::asio::ip::make_address(server_ip), server_port) {
//         socket_.open(udp::v4());
//     }

//     void send_packet(const std::string& data, bool reliable = false) {
//         if (reliable) {
//             uint32_t sequence_number = next_sequence_number_++;
//             pending_acks_[sequence_number] = {data, std::chrono::steady_clock::now()};
//             send_with_sequence(data, sequence_number);
//         } else {
//             socket_.async_send_to(
//                 boost::asio::buffer(data),
//                 server_endpoint_,
//                 boost::bind(&DatagramClient::handle_send, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
//             );
//         }
//     }

// private:
//     void send_with_sequence(const std::string& data, uint32_t sequence_number) {
//         std::string packet = std::to_string(sequence_number) + ":" + data;
//         socket_.async_send_to(
//             boost::asio::buffer(packet),
//             server_endpoint_,
//             boost::bind(&DatagramClient::handle_send, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
//         );
//     }

//     void handle_send(const boost::system::error_code& error, std::size_t bytes_transferred) {
//         if (!error) {
//             std::cout << "Successfully sent " << bytes_transferred << " bytes." << std::endl;
//         } else {
//             std::cerr << "Send failed: " << error.message() << std::endl;
//         }
//     }

//     void handle_ack(uint32_t sequence_number) {
//         pending_acks_.erase(sequence_number);
//     }

//     struct PendingAck {
//         std::string data;
//         std::chrono::steady_clock::time_point timestamp;
//     };

//     udp::socket socket_;
//     udp::endpoint server_endpoint_;
//     uint32_t next_sequence_number_ = 0;
//     std::unordered_map<uint32_t, PendingAck> pending_acks_;
// };

// int main() {
//     try {
//         boost::asio::io_context io_context;
//         DatagramClient client(io_context, "127.0.0.1", 8080);

//         client.send_packet("Hello from ASIO client!", true);

//         io_context.run();
//     } catch (std::exception& e) {
//         std::cerr << "Exception: " << e.what() << std::endl;
//     }

//     return 0;
// }
