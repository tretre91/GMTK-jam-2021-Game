#ifndef CELL_HPP
#define CELL_HPP

#include <MyGL/mygl.hpp>

enum class CellType
{
    wall,
    floor,
    portal_1,
    portal_2
};

class Cell {
private:
    CellType m_type;
    my::Rectangle m_sprite;

public:
    Cell(CellType type, const my::Rectangle& sprite, float x, float y);

    CellType getType() const;
    glm::vec2 getPosition() const;
    my::Rectangle& getSprite();
};

#endif // CELL_HPP

