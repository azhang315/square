#pragma once
#include <unordered_map>
#include <chrono>
#include <boost/asio.hpp>

using udp = boost::asio::ip::udp;

struct ClientActivity {
    udp::endpoint endpoint;
    std::chrono::steady_clock::time_point last_active;
};

class ActivityTracker {
private:
    std::unordered_map<std::string, ClientActivity> activity_map;
    const std::chrono::seconds timeout = std::chrono::seconds(60);  // 1-minute window

    std::string endpoint_to_string(const udp::endpoint& endpoint) {
        return endpoint.address().to_string() + ":" + std::to_string(endpoint.port());
    }

public:
    void update_activity(const udp::endpoint& endpoint) {
        std::string key = endpoint_to_string(endpoint);
        activity_map[key] = {endpoint, std::chrono::steady_clock::now()};
    }

    std::vector<udp::endpoint> get_active_clients() {
        std::vector<udp::endpoint> active_clients;
        auto now = std::chrono::steady_clock::now();

        for (auto it = activity_map.begin(); it != activity_map.end(); ) {
            if (now - it->second.last_active <= timeout) {
                active_clients.push_back(it->second.endpoint);
                ++it;
            } else {
                // Remove stale entries
                it = activity_map.erase(it);
            }
        }

        return active_clients;
    }
};
