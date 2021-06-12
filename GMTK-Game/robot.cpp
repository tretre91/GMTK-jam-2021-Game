#include "robot.hpp"

Robot::Robot() {}

Robot::Robot(const my::Color& color, int cellSize, int x, int y) : x(x), y(y), m_sprite(10, (cellSize - 2) / 2) {
    m_sprite.setColor(color);
    m_sprite.setOutlineThickness(2);
    m_sprite.setOutlineColor(my::Color::black);
}

my::Polygon& Robot::getSprite() {
    return m_sprite;
}
