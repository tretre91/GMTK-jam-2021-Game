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
    std::vector<std::vector<Cell>> m_grid;
    std::vector<glm::ivec2> startingPositions;
    std::vector<Robot> robots;
    size_t currentRobot;
    GameState currentState;

    void moveRobot(Robot& robot, Direction dir);

public:
    Grid(const std::string& filename, int windowWidth, int windowHeight, int y);

    GameState move(Direction dir);

    void swapControl();

    void restart();

    void draw(my::GLWindow& window);
};

#endif // GRID_HPP
