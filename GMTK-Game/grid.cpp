#include "grid.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>

const my::Color Grid::robotColor1(94, 226, 73);
const my::Color Grid::robotColor2(221, 138, 28);

Direction opposite(Direction dir) {
    switch (dir)
    {
    case Direction::up:
        return Direction::down;
    case Direction::down:
        return Direction::up;
    case Direction::left:
        return Direction::right;
    case Direction::right:
        return Direction::left;
    }
}

void Grid::moveRobot(Robot& robot, Direction dir) {
    try {
        switch (dir) {
        case Direction::up:
            if (m_grid.at(robot.y - 1)[robot.x].getType() != CellType::wall) {
                robot.y--;
            }
            break;
        case Direction::down:
            if (m_grid.at(robot.y + 1)[robot.x].getType() != CellType::wall) {
                robot.y++;
            }
            break;
        case Direction::left:
            if (m_grid[robot.y].at(robot.x - 1).getType() != CellType::wall) {
                robot.x--;
            }
            break;
        case Direction::right:
            if (m_grid[robot.y].at(robot.x + 1).getType() != CellType::wall) {
                robot.x++;
            }
            break;
        }
        robot.getSprite().setPosition(m_grid[robot.y][robot.x].getPosition(), true);
    } catch (...) {
    
    }
}

Grid::Grid(const std::string& filename, int windowWidth, int windowHeight, int grid_y) : startPosition1(), startPosition2(), currentBot(robot1), otherBot(robot2) {
    std::ifstream levelFile(filename);
    if (!levelFile.is_open()) {
        std::cerr << "Failed to open level file \"" << filename << "\"\n";
        return;
    }

    int gridWidth, gridHeight;
    levelFile >> gridWidth >> gridHeight;
    levelFile.ignore();
    const int cellSize = std::min(windowWidth / gridWidth, windowHeight / gridHeight);
    
    my::Rectangle floorCell(cellSize, cellSize);
    floorCell.setColor(204, 204, 204);
    my::Rectangle wallCell(cellSize, cellSize);
    wallCell.setColor(43, 43, 43);
    my::Rectangle portalCell1(cellSize, cellSize);
    portalCell1.setColor(robotColor1);
    my::Rectangle portalCell2(cellSize, cellSize);
    portalCell2.setColor(robotColor2);

    m_grid = std::vector<std::vector<Cell>>(gridHeight);
    const float x_start = (windowWidth - gridWidth * cellSize) / 2.0f + cellSize / 2.0f;
    float y = static_cast<float>(grid_y) - cellSize / 2;//= windowHeight - ((windowHeight - gridHeight * cellSize) / 2.0f + cellSize / 2.0f);
    float x;

    std::string row;
    for (size_t i = 0; i < gridHeight; i++) {
        std::getline(levelFile, row);
        m_grid[i].reserve(gridWidth);
        x = x_start;
        for (size_t j = 0; j < gridWidth; j++) {
            switch (row[j])
            {
            case 'x':
                m_grid[i].emplace_back(Cell(CellType::wall, wallCell, x, y));
                break;

            case ' ':
            case '1':
            case '2':
                m_grid[i].emplace_back(Cell(CellType::floor, floorCell, x, y));
                break;

            case 'a':
                m_grid[i].emplace_back(Cell(CellType::portal_1, portalCell1, x, y));
                break;

            case 'b':
                m_grid[i].emplace_back(Cell(CellType::portal_2, portalCell2, x, y));
                break;
            default:
                break;
            }

            if (row[j] == '1') startPosition1 = { j, i };
            else if (row[j] == '2') startPosition2 = { j, i };

            x += cellSize;
        }
        y -= cellSize;
    }

    levelFile.close();

    robot1 = Robot(robotColor1, cellSize, startPosition1.x, startPosition1.y);
    robot1.getSprite().setPosition(m_grid[robot1.y][robot1.x].getPosition(), true);
    robot2 = Robot(robotColor2, cellSize, startPosition2.x, startPosition2.y);
    robot2.getSprite().setPosition(m_grid[robot2.y][robot2.x].getPosition(), true);
    currentBot.getSprite().setOutlineColor(my::Color::white);
    otherBot.getSprite().setOutlineColor(my::Color::black);
}

Grid& Grid::operator=(const Grid& grid) {
    m_grid = grid.m_grid;
    startPosition1 = grid.startPosition1;
    startPosition2 = grid.startPosition2;
    robot1 = grid.robot1;
    robot2 = grid.robot2;
    currentBot = robot1;
    otherBot = otherBot;
    playable = grid.playable;
    return *this;
}

GameState Grid::move(Direction dir) {
    if (playable) {
        moveRobot(currentBot, dir);
        moveRobot(otherBot, opposite(dir));
        int finished = 0;

        if (m_grid[robot1.y][robot1.x].getType() == CellType::portal_1) {
            robot1.getSprite().setColor(150, 150, 150, 125);
            finished++;
        }

        if (m_grid[robot2.y][robot2.x].getType() == CellType::portal_2) {
            robot2.getSprite().setColor(150, 150, 150, 125);
            finished++;
        }

        if (finished == 2) {
            playable = false;
            return GameState::won;
        }
        else if (finished == 0) {
            return GameState::in_progress;
        } else {
            playable = false;
            return GameState::lost;
        }
    }
}

void Grid::swapControl() {
    std::swap(currentBot, otherBot);
    currentBot.getSprite().setOutlineColor(my::Color::white);
    otherBot.getSprite().setOutlineColor(my::Color::black);
}

void Grid::restart() {
    robot1.x = startPosition1.x;
    robot1.y = startPosition1.y;
    robot1.getSprite().setPosition(m_grid[startPosition1.y][startPosition1.x].getPosition(), true);
    robot1.getSprite().setColor(robotColor1);
    robot2.x = startPosition2.x;
    robot2.y = startPosition2.y;
    robot2.getSprite().setPosition(m_grid[startPosition2.y][startPosition2.x].getPosition(), true);
    robot2.getSprite().setColor(robotColor2);
    playable = true;
}

void Grid::draw(my::GLWindow& window) {
    for (auto& row: m_grid) {
        for (Cell& c: row) {
            window.draw(c.getSprite());
        }
    }

    window.draw(robot1.getSprite());
    window.draw(robot2.getSprite());
}