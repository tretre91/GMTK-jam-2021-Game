#include "grid.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>

const std::array<my::Color, 2> colors{ my::Color(94, 226, 73), my::Color(221, 138, 28) };

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
    } catch (std::out_of_range) {
    
    }
}

Grid::Grid(const std::string& filename, int windowWidth, int windowHeight, int grid_y) : currentRobot(0), currentState(GameState::in_progress) {
    std::ifstream levelFile(filename);
    if (!levelFile.is_open()) {
        std::cerr << "Failed to open level file \"" << filename << "\"\n";
        currentState = GameState::lost;
        return;
    }

    int gridWidth, gridHeight;
    int nbOfRobots;
    levelFile >> gridWidth >> gridHeight >> nbOfRobots;
    levelFile.ignore();
    startingPositions = std::vector<glm::ivec2>(nbOfRobots);
    robots = std::vector<Robot>(nbOfRobots);
    const int cellSize = std::min(windowWidth / gridWidth, windowHeight / gridHeight);
    
    my::Rectangle floorCell(cellSize, cellSize);
    floorCell.setColor(204, 204, 204);
    my::Rectangle wallCell(cellSize, cellSize);
    wallCell.setColor(43, 43, 43);
    my::Rectangle portalCell1(cellSize, cellSize);
    portalCell1.setColor(colors[0]);
    my::Rectangle portalCell2(cellSize, cellSize);
    portalCell2.setColor(colors[1]);

    m_grid = std::vector<std::vector<Cell>>(gridHeight);
    const float x_start = (windowWidth - gridWidth * cellSize) / 2.0f + cellSize / 2.0f;
    float y = static_cast<float>(grid_y) - cellSize / 2;
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

            case 'a':
                m_grid[i].emplace_back(Cell(CellType::portal_1, portalCell1, x, y));
                break;

            case 'b':
                m_grid[i].emplace_back(Cell(CellType::portal_2, portalCell2, x, y));
                break;

            default:
                m_grid[i].emplace_back(Cell(CellType::floor, floorCell, x, y));
                break;
            }

            if (std::isdigit(row[j])) {
                startingPositions[static_cast<size_t>(row[j]) - '0'] = { j, i };
            }
            x += cellSize;
        }
        y -= cellSize;
    }

    levelFile.close();

    for (size_t i = 0; i < nbOfRobots; i++) {
        robots[i] = Robot(colors[i % 2], cellSize, startingPositions[i].x, startingPositions[i].y);
        robots[i].getSprite().setPosition(m_grid[robots[i].y][robots[i].x].getPosition(), true);
        if (i == currentRobot) {
            robots[i].getSprite().setOutlineColor(my::Color::white);
        } else {
            robots[i].getSprite().setOutlineColor(my::Color::black);
        }
    }
}

GameState Grid::move(Direction dir) {
    if (currentState == GameState::in_progress) {
        dir = currentRobot % 2 == 0 ? dir : opposite(dir);
        int finished = 0;
        for (size_t i = 0; i < robots.size(); i++) {
            if (i % 2 == 0) {
                moveRobot(robots[i], dir);
                if (m_grid[robots[i].y][robots[i].x].getType() == CellType::portal_1) {
                    robots[i].getSprite().setColor(150, 150, 150, 125);
                    finished++;
                }
            } else {
                moveRobot(robots[i], opposite(dir));
                if (m_grid[robots[i].y][robots[i].x].getType() == CellType::portal_2) {
                    robots[i].getSprite().setColor(150, 150, 150, 125);
                    finished++;
                }
            }
        }

        if (finished == robots.size()) {
            currentState = GameState::won;
        } else if (finished == 0) {
            currentState = GameState::in_progress;
        } else {
            currentState = GameState::lost;
        }
    }
    return currentState;
}

void Grid::swapControl() {
    robots[currentRobot].getSprite().setOutlineColor(my::Color::black);
    currentRobot = (currentRobot + 1) % robots.size();
    robots[currentRobot].getSprite().setOutlineColor(my::Color::white);
}

void Grid::restart() {
    for (size_t i = 0; i < robots.size(); i++) {
        robots[i].x = startingPositions[i].x;
        robots[i].y = startingPositions[i].y;
        robots[i].getSprite().setPosition(m_grid[robots[i].y][robots[i].x].getPosition(), true);
        robots[i].getSprite().setColor(colors[i % 2]);
    }
    currentState = GameState::in_progress;
}

void Grid::draw(my::GLWindow& window) {
    for (auto& row: m_grid) {
        for (Cell& c: row) {
            window.draw(c.getSprite());
        }
    }

    for (Robot& robot: robots) {
        window.draw(robot.getSprite());
    }
}