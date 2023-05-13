#include "shark.h"

Shark::Shark(const double x, const double y) : Entity(x, y) {
    texture.load_from_file("Shark.png", 20, 20);
}

void Shark::tick(const double delta) {
    if (trail != nullptr) {
        
    }
    Entity::move(delta);
}

void Shark::render() const {
    texture.render(static_cast<int>(position.x), static_cast<int>(position.y));
}