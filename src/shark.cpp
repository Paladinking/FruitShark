#include "shark.h"

Shark::Shark(const double x, const double y) : position(x, y) {
    texture.load_from_file("Shark.png", 20, 20);
}

void Shark::tick(const double delta) {

}

void Shark::render() {
    texture.render(static_cast<int>(position.x), static_cast<int>(position.y));
}