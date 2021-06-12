#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <MyGL/mygl.hpp>

class Robot {
private:
    my::Polygon m_sprite;

public:
    int x;
    int y;

    Robot();

    Robot(const my::Color& color, int cellSize, int x, int y);

    my::Polygon& getSprite();
};

#endif // ROBOT_HPP