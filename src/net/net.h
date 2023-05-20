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

#endif //FRUITSHARK_NET_H
