#include "server.h"
#include <iostream>

Server::Server() : Game(100, 100, "For your eyes only") {}

TextureHandler texture_handler = TextureHandler();

bool query_quit() {
    std::string s;
    std::getline(std::cin, s);
    return s == "exit";
}

void Server::init() {
    ENetAddress address = {ENET_HOST_ANY, 1234};
    initialize();
    message_future = std::async(query_quit);

    buffer.resize(1 + (ships.size() + sharks.size()) * ships[0].size(), 0);
    for (int i = 0; i < PLAYER_COUNT; ++i) {
        inputs.push_back(new bool[4]);
        inputs[i][0] = false;
        inputs[i][1] = false;
        inputs[i][2] = false;
        inputs[i][3] = false;
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
        if (message_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            if (message_future.get()) {
                throw base_exception("Server closed");
            } else {
                message_future = std::async(query_quit );
            }
        }
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
    std::cout << "All players here" << std::endl;
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
        buf[offset] = static_cast<Uint8>(shark.type);
        shark.write(buf + offset + 1);
        offset += shark.size() + 1;
    }
    ENetPacket* packet = enet_packet_create(buf, init_size, ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(server.get(), 0, packet);
    enet_host_flush(server.get());
    std::cout << "Sent initial data" << std::endl;
    delete packet;
}



void Server::tick(Uint64 delta) {
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
                    Uint8 id = *reinterpret_cast<Uint8*>(event.peer->data);
                    Uint8 input = event.packet->data[0];
                    bool press = event.packet->data[1] != 0;
                    if (input < 4) inputs[id][input] = press;
                    else if (press) ships[id].handle_down(input == 4, input == 5);
                    else ships[id].handle_up(input == 4, input == 5, *this);
                }
                enet_packet_destroy(event.packet);
                break;
            default:
                enet_peer_reset(event.peer);
        }
    }

    tick_physics(dDelta, inputs);
    if (message_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
        if (message_future.get()) {
            exit_game();
        } else {
            message_future = std::async(query_quit);
        }
    }
    size_t offset = 1;
    buffer[0] = Event::STATE;
    for (const auto& ship : ships) {
        ship.write(buffer.data() + offset);
        offset += ship.size();
    }
    for (const auto& shark : sharks) {
        shark.write(buffer.data() + offset);
        offset += shark.size();
    }
    for (const auto& fruit : fruits_in_air) {
        fruit.write(buffer.data() + offset);
        offset += fruit.size();
    }
    ENetPacket* packet = enet_packet_create(buffer.data(), buffer.size(),
                                           ENET_PACKET_FLAG_RELIABLE | ENET_PACKET_FLAG_NO_ALLOCATE);
    enet_host_broadcast(server.get(), 0, packet);
}

void Server::fruit_fired(Vector2D position, Vector2D velocity, FruitType type, bool cannon) {
    std::cout << "Fruit fired {" << position.x << "," << position.y << "}, {"
        << velocity.x << "," << velocity.y << "}, " << static_cast<int>(type) << ", " << cannon << std::endl;
    buffer.resize(buffer.size() + sizeof(Uint32), 0);
    to_be_added.emplace_back(position, velocity, type);
    Uint8 buf[3 + 4 * sizeof(float)];
    buf[0] = Event::FRUIT_FIRED;
    buf[1] = static_cast<Uint8>(type);
    buf[2] = cannon ? 1 : 0;
    reinterpret_cast<float*>(buf + 3)[0] = static_cast<float>(position.x);
    reinterpret_cast<float*>(buf + 3)[1] = static_cast<float>(position.y);
    reinterpret_cast<float*>(buf + 3)[2] = static_cast<float>(velocity.x);
    reinterpret_cast<float*>(buf + 3)[3] = static_cast<float>(velocity.y);
    ENetPacket* packet = enet_packet_create(buf, 3 + 4 * sizeof(float), ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(server.get(), 0, packet);
}

void Server::ship_destroyed(int id) {
    buffer.resize(buffer.size() - sizeof(Uint32), 0);
    if (ships.size() == 1) {
        exit_game();
    }
}

void Server::fruit_hit_water(int fruit, Vector2D position) {
    std::cout << "Fruit hit water " << fruit << ", {" << position.x << "," << position.y << "}" << std::endl;
    Uint8 buf[1 + sizeof(Uint32) + 2 * sizeof(float)];
    buf[0] = Event::FRUIT_HIT_WATER;
    reinterpret_cast<Uint32*>(buf + 1)[0] = static_cast<Uint32>(fruit);
    reinterpret_cast<float*>(buf + 1 + sizeof(Uint32))[0] = static_cast<float>(position.x);
    reinterpret_cast<float*>(buf + 1 + sizeof(Uint32))[1] = static_cast<float>(position.y);
    ENetPacket* packet = enet_packet_create(buf, 1 + sizeof(Uint32) + 2 * sizeof(float),
                                            ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(server.get(), 0, packet);
}

void Server::fruit_hit_player(int fruit, int player_id) {
    std::cout << "Fruit hit player " << fruit << ", " << player_id << std::endl;
    buffer.resize(buffer.size() - sizeof(Uint32), 0);
    Uint8 buf[2 + sizeof(Uint32)];
    buf[0] = Event::FRUIT_HIT_PLAYER;
    buf[1] = static_cast<Uint8>(player_id);
    reinterpret_cast<Uint32*>(buf + 2)[0] = static_cast<Uint32>(fruit);
    ENetPacket* packet = enet_packet_create(buf, 2 + sizeof(Uint32), ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(server.get(), 0, packet);
}

void Server::pickup_created(int x, int y, FruitType type) {
    std::cout << "Pickup created " << x << ", " << y << ", " << static_cast<int>(type) << std::endl;
    pickups.emplace_back(x, y, type);
    buffer.resize(buffer.size() + sizeof(Uint32), 0);
    Uint8 buf[2 + sizeof(Uint32)];
    buf[0] = Event::PICKUP_CREATED;
    buf[1] = static_cast<Uint8>(type);
    reinterpret_cast<Uint32*>(buf + 2)[0] = ((static_cast<Uint32>(x) & 0x7ff) << 11) |
            (static_cast<Uint32>(y) & 0x7ff);
    ENetPacket* packet = enet_packet_create(buf, 2 + sizeof(Uint32), ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(server.get(), 0, packet);
}

void Server::ship_hurt(Vector2D position, int player_id, int dmg) {
    std::cout << "Ship hurt {" << position.x << "," << position.y << "}, " << player_id << ", " << dmg << std::endl;
    Uint8 buf[2 + 2 * sizeof(float) + sizeof(Uint32)];
    buf[0] = Event::SHIP_HURT;
    buf[1] = static_cast<Uint8>(player_id);
    reinterpret_cast<float*>(buf + 2)[0] = static_cast<float>(position.x);
    reinterpret_cast<float*>(buf + 2)[1] = static_cast<float>(position.y);
    reinterpret_cast<Uint32*>(buf + 2 + 2 * sizeof(float))[0] = static_cast<Uint32>(dmg);
    ENetPacket* packet = enet_packet_create(buf, 2 + 2 * sizeof(float) + sizeof (Uint32),
                                            ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(server.get(), 0, packet);
}

void Server::fruit_eaten(int id) {
    std::cout << "Fruit eaten " << id << std::endl;
    buffer.resize(buffer.size() - sizeof(Uint32));
    Uint8 buf[1 + sizeof(Uint32)];
    buf[0] = Event::FRUIT_EATEN;
    reinterpret_cast<Uint32*>(buf + 1)[0] = static_cast<Uint32>(id);
    ENetPacket* packet = enet_packet_create(buf, 1 + sizeof (Uint32),
                                            ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(server.get(), 0, packet);
}

void Server::pickup_taken(int id) {
    std::cout << "Pickup taken " << id << std::endl;
    Uint8 buf[1 + sizeof(Uint32)];
    buf[0] = Event::PICKUP_TAKEN;
    reinterpret_cast<Uint32*>(buf + 1)[0] = static_cast<Uint32>(id);
    ENetPacket* packet = enet_packet_create(buf, 1 + sizeof (Uint32),
                                            ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(server.get(), 0, packet);
}
