#include "activity_tracker.h"

ActivityTracker activity_tracker;

void NetTransport::handle_incoming_packet(const std::string& data, const udp::endpoint& sender_endpoint) {
    activity_tracker.update_activity(sender_endpoint);  // Track the sender

    // Apply update to canvas
    PixelUpdate update = parse_packet(data);
    canvas_state.apply_update(update);

    // Trigger broadcast to active clients
    broadcast_update(update, sender_endpoint);
}

void NetTransport::broadcast_update(const PixelUpdate& update, const udp::endpoint& sender_endpoint) {
    auto active_clients = activity_tracker.get_active_clients();

    for (const auto& client : active_clients) {
        if (client != sender_endpoint) {  // Optional: Skip sender if desired
            send_packet(serialize(update), client);
        }
    }
}
