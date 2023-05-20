#include "entity.h"
#include "config.h"

constexpr double FRICTION = 0.02;

BoxEntity::BoxEntity(double x, double y, int width, int length) :
    Entity(x, y, 0.0), width(width), length(length) {}
BoxEntity::BoxEntity(double x, double y, int width, int length, double angle) :
    Entity(x, y, angle), width(width), length(length) {}

SDL_FPoint rotated(Vector2D v, const Vector2D& center, double angle) {
    v.rotate(angle);
    return {static_cast<float>(center.x + v.x), static_cast<float>(center.y + v.y)};
}

void BoxEntity::update_bounds() {
    corners[0] = rotated({- length / 2.0,-width / 2.0}, position, angle);
    corners[1] = rotated({length / 2.0 - 1.0,-width / 2.0},position, angle);
    corners[2] = rotated({length / 2.0 - 1.0,width / 2.0 - 1.0},position, angle);
    corners[3] = rotated( {-length / 2.0,width / 2.0 - 1.0},position, angle);
    SDL_EncloseFPoints(corners, 4, nullptr, &bounds);
}

void BoxEntity::adjust_bounds(float dx, float dy) {
    corners[0].x += dx;
    corners[0].y += dx;
    corners[1].x += dx;
    corners[1].y += dx;
    corners[2].x += dx;
    corners[2].y += dx;
    corners[3].x += dx;
    corners[3].y += dx;
    bounds.x += dx;
    bounds.y += dy;
    position.x += dx;
    position.y += dy;
}

void BoxEntity::move(const double delta) {
    while (angle < 0.0) angle += 2 * PI;
    while (angle > 2 * PI) angle -= 2 * PI;
    acceleration.x -= velocity.x * std::abs(velocity.x) * FRICTION;
    acceleration.y -= velocity.y * std::abs(velocity.y) * FRICTION;

    velocity.x += delta * acceleration.x;
    velocity.y += delta * acceleration.y;

    Vector2D to_move = {velocity.x * delta, velocity.y * delta};
    position.add(to_move);
    acceleration.x = 0.0;
    acceleration.y = 0.0;
    update_bounds();
    if (bounds.x < UI_SIZE) {
        adjust_bounds(UI_SIZE - bounds.x, 0.0);
    } else if (bounds.x + bounds.w >= LOGICAL_WIDTH - UI_SIZE) {
        adjust_bounds((LOGICAL_WIDTH - UI_SIZE) - (bounds.x + bounds.w), 0.0);
    }
    if (bounds.y < 0.0) {
        adjust_bounds(0.0, -bounds.y);
    } else if (bounds.y + bounds.h >= LOGICAL_HEIGHT) {
        adjust_bounds(0.0, LOGICAL_HEIGHT - (bounds.y + bounds.h));
    }
}

bool BoxEntity::intersects(const BoxEntity &other) const {
    if (SDL_HasIntersectionF(&bounds, &other.bounds)) {
        SDL_FRect rect = {static_cast<float>(position.x - length / 2.0),
                          static_cast<float>(position.y - width / 2.0),
                          static_cast<float>(length),
                          static_cast<float>(width)};
        for (unsigned i = 0; i < 4; i++) {
            auto ca = static_cast<float>(cos(-angle));
            auto sa = static_cast<float>(sin(-angle));
            float x1 = other.corners[i].x - static_cast<float>(position.x);
            float y1 = other.corners[i].y - static_cast<float>(position.y);
            float x2 = other.corners[(i + 1) % 4].x - static_cast<float>(position.x);
            float y2 = other.corners[(i + 1) % 4].y - static_cast<float>(position.y);
            float x1r = static_cast<float>(position.x) + x1 * ca - y1 * sa;
            float y1r = static_cast<float>(position.y) + y1 * ca + x1 * sa;
            float x2r = static_cast<float>(position.x) + x2 * ca - y2 * sa;
            float y2r = static_cast<float>(position.y) + y2 * ca + x2 * sa;
            if (SDL_IntersectFRectAndLine(&rect, &x1r, &y1r, &x2r, &y2r)) {
                return true;
            }
        }
    }
    return false;
}

bool BoxEntity::intersects(Vector2D pos, double radius) const {
    pos.subtract(position);
    pos.rotate(-angle);
    pos.add(position);
    Vector2D test = {pos.x, pos.y};
    SDL_FRect rect = {static_cast<float>(position.x - length / 2.0),
                      static_cast<float>(position.y - width / 2.0),
                      static_cast<float>(length),
                      static_cast<float>(width)};

    if (pos.x < rect.x) test.x = rect.x;
    else if (pos.x > rect.x + rect.w) test.x = rect.x + rect.w;
    if (pos.y < rect.y) test.y = rect.y;
    else if (pos.y > rect.y + rect.h) test.y = rect.y + rect.h;
    return test.distance_squared(pos) < radius * radius;
}

void BoxEntity::handle_Collision(BoxEntity &other) {
    Vector2D vec = other.position;
    vec.subtract(position);

    double power = velocity.length() + other.velocity.length();
    acceleration.add_scaled(vec, -power * 0.5);
    other.acceleration.add_scaled(vec, power * 0.5);
}

const Vector2D &BoxEntity::get_position() const {
    return position;
}

size_t Entity::size() const {
    return sizeof(Uint32);
}

void Entity::read(const unsigned char* buf) {
    Uint32 data = reinterpret_cast<const Uint32*>(buf)[0];
    position.x = static_cast<double>(data & 0x7ff);
    position.y = static_cast<double>((data >> 11) & 0x7ff);
    angle = (data >> 22) * 2 * PI / 1024.0;
}

void Entity::write(unsigned char *buf) const {
    Uint32 data = (static_cast<Uint32>(position.x) & 0x7ff) |
            ((static_cast<Uint32>(position.y) & 0x7ff) << 11) |
            ((static_cast<Uint32>(std::lround((1024.0 / (2 * PI)) * angle)) & 0x3ff) << 22);
    reinterpret_cast<Uint32*>(buf)[0] = data;
}

Entity::Entity(double x, double y, double angle) : position(x, y), angle(angle) {}