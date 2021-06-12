#include "cell.hpp"

Cell::Cell(CellType type, const my::Rectangle& sprite, float x, float y) : m_type(type), m_sprite(sprite) {
    m_sprite.setPosition({ x, y }, true);
}

CellType Cell::getType() const {
    return m_type;
}

glm::vec2 Cell::getPosition() const {
    return m_sprite.getPosition();
}

my::Rectangle& Cell::getSprite() {
    return m_sprite;
}