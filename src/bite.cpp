//
// Created by axelh on 2023-05-14.
//
#include "config.h"

#include "bite.h"
Bite::Bite(int x, int y) : x(x), y(y), textures(texture_handler.get_textures(TextureID::TEETH)){

}

void Bite::tick(double delta) {
    passed += delta;
}

void Bite::render() const {
    textures[std::min(static_cast<int>(passed * (4.0 / 0.15)), 4)].render(x, y);
}

bool Bite::is_dead() const{
    return passed >= 0.15;
}
