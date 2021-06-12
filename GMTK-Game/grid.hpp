#ifndef GRID_HPP
#define GRID_HPP

#include <MyGL/mygl.hpp>
#include "cell.hpp"
#include "robot.hpp"
#include <vector>
#include <string>

enum class Direction
{
    up,
    down,
    left,
    right
};

enum class GameState
{
    won,
    lost,
    in_progress
};

class Grid {
private:
    static const my::Color robotColor1;
    static const my::Color robotColor2;
    std::vector<std::vector<Cell>> m_grid;
    glm::ivec2 startPosition1;
    glm::ivec2 startPosition2;
    Robot robot1;
    Robot robot2;
    Robot& currentBot;
    Robot& otherBot;
    bool playable = true;

    void moveRobot(Robot& robot, Direction dir);

public:
    Grid(const std::string& filename, int windowWidth, int windowHeight, int y);

    Grid& operator=(const Grid& grid);

    GameState move(Direction dir);

    void swapControl();

    void restart();

    void draw(my::GLWindow& window);
};

#endif // GRID_HPP
