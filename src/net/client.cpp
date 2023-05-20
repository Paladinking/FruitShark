#include "client.h"
#include "entities/ship.h"
#include "entities/shark.h"
#include "asset_handlers/sound.h"
#include <iostream>

Client::Client() : State() {}

void Client::init(WindowState *window_state) {
    State::init(window_state);
    forward = get_press_input(BINDINGS[0][static_cast<Uint8>(Ship::Direction::FORWARDS)]);
    left = get_press_input(BINDINGS[0][static_cast<Uint8>(Ship::Direction::LEFT)]);
    right = get_press_input(BINDINGS[0][static_cast<Uint8>(Ship::Direction::RIGHT)]);
    left_cannon = get_press_input(BINDINGS[0][4]);
    right_cannon = get_press_input(BINDINGS[0][5]);

    client = std::unique_ptr<ENetHost, HostDeleter>(enet_host_create(nullptr, 1, 2, 0, 0));
    if (client == nullptr) {
        throw net_exception("Could not create client");
    }
    ENetAddress address = {0, 1234};
    enet_address_set_host_ip(&address, "127.0.0.1");
    peer = enet_host_connect(client.get(), &address, 2, 0);
    if (peer == nullptr) {
        throw net_exception("Could not connect to server");
    }
    ENetEvent event;
    if (enet_host_service(client.get(), &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        std::cout << "Connected" << std::endl;
    } else {
        throw net_exception("Could not connect to server");
    }
    if (enet_host_service(client.get(), &event, 2000) > 0 && event.type == ENET_EVENT_TYPE_RECEIVE) {
        id = event.packet->data[0];
        std::cout << "Got id: "<< static_cast<int>(id) << std::endl;
        enet_packet_destroy(event.packet);
    } else {
        throw net_exception("Did not receive id");
    }

    while (true) {
        if (enet_host_service(client.get(), &event, 2000) < 0) {
            throw net_exception("Service failed");
        }
        if (event.type == ENET_EVENT_TYPE_RECEIVE) {
            std::cout << "Client got something" << std::endl;
            Uint8 player_count = event.packet->data[0];
            size_t offset = 1;
            for (int i = 0; i < player_count; ++i, offset += 4) {
                ships.emplace_back(0.0, 0.0, COLORS[i], i, 0.0);
                ships.back().read(event.packet->data + offset);
            }
            while (offset < event.packet->dataLength) {
                auto type = static_cast<Shark::Type>(event.packet->data[offset]);
                sharks.push_back(Shark::create_shark(type, 0.0, 0.0));
                sharks.back().read(event.packet->data + offset + 1);
                offset += 5;
            }
            break;
        } else if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
            throw net_exception("Lost connection");
        }

    }
    std::cout << "Client initialized" << std::endl;
}

void Client::load_state(const Uint8 *buffer) {
    size_t offset = 0;
    for (auto& ship : ships) {
        ship.read(buffer + offset);
        offset += ship.size();
    }
    for (auto& shark : sharks) {
        shark.read(buffer + offset);
        offset += shark.size();
    }
}

void Client::handle_event(Uint8 event, const Uint8* buffer) {
    switch (event) {
        case Event::STATE:
            load_state(buffer);
            break;
        case Event::SHIP_HURT:
            ship_hurt({static_cast<double>(reinterpret_cast<const float*>(buffer + 1)[0]),
                     static_cast<double>(reinterpret_cast<const float*>(buffer + 1)[1])},
                     static_cast<int>(buffer[0]),
                     static_cast<int>(reinterpret_cast<const Uint32*>(buffer + 1 + 2 * sizeof(float))[0])
                    );
            break;
        case Event::CANNON_FIRED:
            cannon_fired({static_cast<double>(reinterpret_cast<const float*>(buffer + 1)[0]),
                     static_cast<double>(reinterpret_cast<const float*>(buffer + 1)[1])},
                     {static_cast<double>(reinterpret_cast<const float*>(buffer + 1)[2]),
                      static_cast<double>(reinterpret_cast<const float*>(buffer + 1)[3])},
                      static_cast<FruitType>(buffer[0]));
            break;
        case Event::PICKUP_CREATED:
            pickup_created(
                    static_cast<int>((reinterpret_cast<const Uint32*>(buffer + 1)[0] >> 11) & 0x7ff),
                    static_cast<int>((reinterpret_cast<const Uint32*>(buffer + 1))[0] & 0x7ff),
                    static_cast<FruitType>(buffer[0]));
            break;
        case Event::FRUIT_HIT_PLAYER:
            fruit_hit_player(static_cast<int>(reinterpret_cast<const Uint32*>(buffer + 1)[0]),
                             static_cast<int>(buffer[0]));
            break;
        case Event::FRUIT_HIT_WATER:
            fruit_hit_water(static_cast<int>(reinterpret_cast<const Uint32*>(buffer)[0]),
                            {static_cast<double>(reinterpret_cast<const float*>(buffer + 1)[0]),
                             static_cast<double>(reinterpret_cast<const float*>(buffer + 1)[1])});
            break;
        default:
            throw net_exception("Unknown event");
    }
}

void Client::tick(const Uint64 delta, StateStatus &res) {
    ENetEvent event;
    while (enet_host_service(client.get(), &event, 10) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_NONE:
            case ENET_EVENT_TYPE_CONNECT:
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                // TODO
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                handle_event(event.packet->data[0], event.packet->data + 1);
                enet_packet_destroy(event.packet);
                break;
        }
    }

    double dDelta = static_cast<double>(delta) / 1000.0;
    for (auto& fruit : fruits_in_water) {
        fruit.tick_physics(dDelta);
    }
    //for (const auto& bite : bites) bite.render();
}

void Client::render() {
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    SDL_SetRenderDrawColor(gRenderer, 0x00, 0xB2, 0xFF, 0xFF);
    SDL_RenderFillRect(gRenderer, &game_viewport);
    for (const auto& ship : ships) ship.render();
    for (const auto& shark : sharks) shark.render();
    for (const auto& fruit : fruits_in_air) fruit.render();
    for (const auto& fruit : fruits_in_water) fruit.render();
    for (const auto& pickup : pickups) pickup.render();

    SDL_RenderPresent(gRenderer);
}

void Client::handle_down(const SDL_Keycode key, const Uint8 mouse) {
    Uint8 input;
    if (forward->is_targeted(key, mouse)) {
        input = static_cast<Uint8>(Ship::Direction::FORWARDS);
    } else if (left->is_targeted(key, mouse)) {
        input = static_cast<Uint8>(Ship::Direction::LEFT);
    } else if (right->is_targeted(key, mouse)) {
        input = static_cast<Uint8>(Ship::Direction::RIGHT);
    } else if (left_cannon->is_targeted(key, mouse)) {
        input = 4;
    } else if (right_cannon->is_targeted(key, mouse)) {
        input = 5;
    } else {
        return;
    }
    Uint8 buf[2] = {input, 1};
    ENetPacket* packet = enet_packet_create(buf, 2, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
}

void Client::handle_up(const SDL_Keycode key, const Uint8 mouse) {
    Uint8 input;
    if (forward->is_targeted(key, mouse)) {
        input = static_cast<Uint8>(Ship::Direction::FORWARDS);
    } else if (left->is_targeted(key, mouse)) {
        input = static_cast<Uint8>(Ship::Direction::LEFT);
    } else if (right->is_targeted(key, mouse)) {
        input = static_cast<Uint8>(Ship::Direction::RIGHT);
    } else if (left_cannon->is_targeted(key, mouse)) {
        input = 4;
    } else if (right_cannon->is_targeted(key, mouse)) {
        input = 5;
    } else {
        return;
    }
    Uint8 buf[2] = {input, 0};
    ENetPacket* packet = enet_packet_create(buf, 2, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
}

void Client::cannon_fired(Vector2D position, Vector2D velocity, FruitType type) {
    sound::play(sound::Id::CANNON);
    fruits_in_air.emplace_back(position, velocity, type);
}
void Client::ship_destroyed(int id) {}
void Client::fruit_hit_water(int fruit, Vector2D position) {}

void Client::fruit_hit_player(int fruit, int player_id) {
    fruits_in_air[fruit] = fruits_in_air[fruits_in_air.size() - 1];
    fruits_in_air.pop_back();
}
void Client::pickup_created(int x, int y, FruitType type) {
    pickups.emplace_back(x, y, type);
}
void Client::ship_hurt(Vector2D position, int player_id, int dmg) {
    sound::play(sound::Id::BITE);
    ships[player_id].get_bitten(dmg);
    if (ships[player_id].is_dead()) {
        ships.erase(ships.begin() + player_id);
    }
}
