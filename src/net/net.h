#ifndef FRUITSHARK_NET_H
#define FRUITSHARK_NET_H
#include <enet/enet.h>
#include "engine/exceptions.h"

struct HostDeleter {
    void operator()(ENetHost* host) {enet_host_destroy(host);}
};

class net_exception : public base_exception {
public:
    explicit net_exception(std::string msg) : base_exception(std::move(msg)) {};
};

namespace Event {
    constexpr Uint8 STATE = 0, SHIP_HURT = 1,
        FRUIT_FIRED = 2, PICKUP_CREATED = 3, FRUIT_HIT_WATER = 4,
        FRUIT_HIT_PLAYER = 5, FRUIT_EATEN = 6, PICKUP_TAKEN = 7, GAME_OVER = 8;

}

#endif //FRUITSHARK_NET_H
