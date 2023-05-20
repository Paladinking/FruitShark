#include "server.h"
#include <iostream>

void Server::init(WindowState *window_state) {
    ENetAddress address = {ENET_HOST_ANY, 1234};
    State::init(window_state);
    initialize();
    state_size = (ships.size() + sharks.size()) * ships[0].size();
    for (int i = 0; i < PLAYER_COUNT; ++i) {
        inputs.push_back(new bool[6]);
        inputs[i][0] = false;
        inputs[i][1] = false;
        inputs[i][2] = false;
        inputs[i][3] = false;
        inputs[i][4] = false;
        inputs[i][5] = false;
    }
    server = std::unique_ptr<ENetHost, HostDeleter>(enet_host_create(
                &address, PLAYER_COUNT, 2, 0, 0));
    if (server == nullptr) {
        throw net_exception("Could not create server");
    }
    int player_count = 0;
    bool ids[PLAYER_COUNT];
    for (auto& b : ids) b = false;
    while (player_count < PLAYER_COUNT) {
        ENetEvent event;
        if (enet_host_service(server.get(), &event, 1000) < 0) {
            throw net_exception("Failed to service server");
        }
        if (event.type == ENET_EVENT_TYPE_CONNECT) {
            std::cout << event.peer->address.host << ':'
                      << event.peer->address.port << " connected." << std::endl;
            Uint8 id;
            for (id = 0; id < PLAYER_COUNT; ++id) {
                if (!ids[id]) {
                    ids[id] = true;
                    break;
                }
            }
            ++player_count;
            event.peer->data = new Uint8(id);
            ENetPacket* packet = enet_packet_create(&id, sizeof(Uint8), ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send(event.peer, 0, packet);
        } else if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
            ids[*reinterpret_cast<Uint8*>(event.peer->data)] = false;
            delete reinterpret_cast<Uint8*>(event.peer->data);
            event.peer->data = nullptr;
            --player_count;
        } else if (event.type == ENET_EVENT_TYPE_RECEIVE) {
            ids[*reinterpret_cast<Uint8*>(event.peer->data)] = false;
            delete reinterpret_cast<Uint8*>(event.peer->data);
            event.peer->data = nullptr;
            --player_count;
            enet_peer_reset(event.peer);
            enet_packet_destroy(event.packet);
        }
    }
    size_t init_size = 1 + ships.size() * ships[0].size();
    size_t offset = 1;
    for (const auto& shark : sharks) {
        init_size += 1 + shark.size();
    }
    auto buf = new Uint8[init_size];
    buf[0] = ships.size();
    for (const auto& ship : ships) {
        ship.write(buf + offset);
        offset += ship.size();
    }
    for (const auto& shark : sharks) {
        shark.write(buf + offset);
        buf[offset + shark.size()] = static_cast<Uint8>(shark.type);
        offset += shark.size() + 1;
    }
    ENetPacket* packet = enet_packet_create(buf, init_size, ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(server.get(), 0, packet);
    enet_host_flush(server.get());
    delete packet;
}

void Server::tick(Uint64 delta, StateStatus &res) {
    double dDelta = static_cast<double>(delta) / 1000.0;
    ENetEvent event;
    while (enet_host_service(server.get(), &event, 10) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_DISCONNECT:
                // TODO
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                if (event.packet->dataLength != 2 || event.packet->data[0] > 5) {
                    enet_peer_reset(event.peer);
                } else {
                    inputs[*reinterpret_cast<Uint8*>(event.peer->data)][event.packet->data[0]]
                        = event.packet->data[1];
                }
                enet_packet_destroy(event.packet);
                break;
            default:
                enet_peer_reset(event.peer);
        }
    }
    tick_physics(dDelta, inputs);
    for (auto& input : inputs) { // Reset cannon input
        input[4] = false;
        input[5] = false;
    }
}

void Server::cannon_fired(Vector2D position, Vector2D velocity, FruitType type) {
    fruits_in_air.emplace_back(position, velocity, type);
}

void Server::create_bite(Vector2D position) {

}

void Server::ship_destroyed(int id) {

}

void Server::fruit_hit_water(const Fruit &fruit) {

}

void Server::fruit_hit_player(const Fruit &fruit, int player_id) {

}

void Server::pickup_created(const Pickup &pickup) {

}
