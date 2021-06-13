#include <MyGL/mygl.hpp>
#include "grid.hpp"
#include <vector>
#include <algorithm>

void endScreen(my::GLWindow& window, my::Font& font) {
    my::Text end_text("Thank you for playing!", font, 50);
    end_text.setPosition(window.getSize() / 2, true);
    end_text.setColor(my::Color::white);
    my::Text exit_text("Press Escape to return to the level selection screen", font, 20);
    exit_text.setPosition(window.getSize().x / 2, end_text.getPosition().y - 65, true);
    exit_text.setColor(my::Color::white);

    my::Event event;
    while (window.isRunning()) {
        while (window.pollEvent(event)) {
            switch (event.type)
            {
            case my::EventType::windowShouldClose:
                window.close();
                break;
            case my::EventType::keyPressed:
                if (event.keyCode == my::Key::escape) {
                    return;
                }
                break;
            default:
                break;
            }
        }

        window.clear(my::Color::black);
        window.draw(end_text);
        window.draw(exit_text);
        window.display();
    }
}

bool level(int lvl, my::GLWindow& window, my::Font& font, my::Text& win_text, my::Text& lose_text) {
    Grid grid("Levels/lvl" + std::to_string(lvl) + ".txt", 800, 600, window.getSize().y - 50);
    my::Text level("Level " + std::to_string(lvl), font, 35);
    level.setPosition(5, window.getSize().y - 55);
    level.setColor(my::Color::white);

    my::Event event;
    GameState state = GameState::in_progress;
    while (window.isRunning()) {
        while (window.pollEvent(event)) {
            switch (event.type) {
            case my::EventType::windowShouldClose:
                window.close();
                break;

            case my::EventType::keyPressed:
                switch (event.keyCode)
                {
                case my::Key::escape:
                    return false;
                case my::Key::up:
                    state = grid.move(Direction::up);
                    break;
                case my::Key::down:
                    state = grid.move(Direction::down);
                    break;
                case my::Key::left:
                    state = grid.move(Direction::left);
                    break;
                case my::Key::right:
                    state = grid.move(Direction::right);
                    break;
                case my::Key::tab:
                    grid.swapControl();
                    break;
                case my::Key::r:
                    grid.restart();
                    state = GameState::in_progress;
                    break;
                case my::Key::enter:
                case my::Key::np_enter:
                    if (state == GameState::won) {
                        return true;
                    }
                default:
                    break;
                }
                break;

            default:
                break;
            }
        }

        window.clear(my::Color::black);

        grid.draw(window);
        window.draw(level);

        if (state == GameState::lost) {
            window.draw(lose_text);
        } else if (state == GameState::won) {
            window.draw(win_text);
        }

        window.display();
    }
    return false;
}

bool startScreen(my::GLWindow& window, my::Font& titleFont, my::Font& font) {
    my::Text title("Untitled Robot Game", titleFont, 65);
    title.setColor(my::Color::white);
    title.setPosition(window.getSize().x / 2, window.getSize().y / 2 + 150, true);

    my::Text play("   Play   ", font, 30);
    play.setColor(my::Color::white);
    play.setOutlineThickness(3);
    play.setOutlineColor(my::Color::white);
    play.setPosition(window.getSize().x / 2, 200, true);

    const my::Color highlightColor(94, 226, 73);
    const my::Color clearColor(132, 132, 132);

    my::Event event;
    while (window.isRunning()) {
        while (window.pollEvent(event)) {
            switch (event.type)
            {
            case my::EventType::windowShouldClose:
                return false;
                break;
            case my::EventType::mouseMoved:
                if (play.BBoxCollides(my::Rectangle(10, 10, std::lroundf(event.mouse.pos.x), std::lroundf(event.mouse.pos.y)))) {
                    play.setOutlineColor(highlightColor);
                } else {
                    play.setOutlineColor(my::Color::white);
                }
                break;
            case my::EventType::mouseButtonPressed:
                if (event.mouse.button == my::MouseButton::left && play.BBoxCollides(my::Rectangle(10, 10, std::lroundf(event.mouse.pos.x), std::lroundf(event.mouse.pos.y)))) {
                    return true;
                }
                break;
            case my::EventType::keyPressed:
                switch (event.keyCode)
                {
                case my::Key::enter:
                    return true;
                case my::Key::escape:
                    return false;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }

        window.clear(clearColor);
        window.draw(title);
        window.draw(play);
        window.display();
    }
    return false;
}

void levelSelect(my::GLWindow& window, my::Font& font, int levelCount) {
    my::Text lost_text("You lost, Press R to restart", font, 30);
    lost_text.setPosition(window.getSize().x / 2, 30, true);
    lost_text.setColor(my::Color::white);

    my::Text win_text("Level complete! Press Enter to go to the next level", font, 30);
    win_text.setPosition(window.getSize().x / 2, 30, true);
    win_text.setColor(my::Color::white);

    const int tilesPerRow = 5;
    const int gap = 15;
    int tileSize = (window.getSize().x - (tilesPerRow + 1) * gap) / tilesPerRow;

    float x = gap + tileSize / 2.0f;
    float y = window.getSize().y - gap - tileSize / 2.0f;

    const int padSize = tileSize / 10;
    auto padding = [&padSize](int i) { return std::string(padSize, ' ') + '\n' + std::string(padSize / 2, ' ') + std::to_string(i) + std::string(padSize / 2, ' ') + '\n' + std::string(padSize, ' '); };

    std::vector<my::Text> tiles;
    tiles.reserve(levelCount);
    for (int i = 0; i < levelCount; i++) {
        tiles.emplace_back(my::Text(padding(i + 1), font, 30));
        tiles[i].setColor(my::Color::white);
        tiles[i].setOutlineThickness(3);
        tiles[i].setOutlineColor(my::Color::white);
        tiles[i].setPosition({ x, y }, true);

        x += gap + tileSize;
        if ((i + 1) % tilesPerRow == 0) {
            x = gap + tileSize / 2.0f;
            y -= gap + tileSize;
        }
    }

    int currentLevel;
    std::vector<my::Text>::iterator selectedTile(tiles.end());
    my::Rectangle cursor;
    const my::Color highlightColor(94, 226, 73);
    const my::Color clearColor(132, 132, 132);

    my::Event event;
    while (window.isRunning()) {
        while(window.pollEvent(event)) {
            switch (event.type)
            {
            case my::EventType::windowShouldClose:
                window.close();
                break;
            case my::EventType::keyPressed:
                if (event.keyCode == my::Key::escape) {
                    return;
                }
                break;
            case my::EventType::mouseMoved:
                if (selectedTile != tiles.end()) {
                    selectedTile->setOutlineColor(my::Color::white);
                }
                cursor = my::Rectangle(5, 5, std::lroundf(event.mouse.pos.x), std::lroundf(event.mouse.pos.y));
                selectedTile = std::find_if(tiles.begin(), tiles.end(), [&cursor](const my::Text& tile) { return tile.BBoxCollides(cursor); });
                if (selectedTile != tiles.end()) {
                    selectedTile->setOutlineColor(highlightColor);
                }
                break;
            case my::EventType::mouseButtonPressed:
                cursor = my::Rectangle(5, 5, std::lroundf(event.mouse.pos.x), std::lroundf(event.mouse.pos.y));
                selectedTile = std::find_if(tiles.begin(), tiles.end(), [&cursor](const my::Text& tile) { return tile.BBoxCollides(cursor); });
                if (selectedTile != tiles.end()) {
                    currentLevel = std::stoi(selectedTile->getString());
                    while (level(currentLevel++, window, font, win_text, lost_text)) {
                        if (currentLevel > levelCount) {
                            endScreen(window, font);
                            break;
                        }
                    }
                }
                break;
            default:
                break;
            }
        }

        window.clear(clearColor);
        for (my::Text& lvl: tiles) {
            window.draw(lvl);
        }
        window.display();
    }
}

int main() {
    my::GLWindow window(800, 700, "Untitled Robot Game", 4);
    window.setFramerate(60);

    my::Font open_sans("Ressources/OpenSans-Regular.ttf");
    my::Font open_sans_semibold("Ressources/OpenSans-SemiBold.ttf");

    const int max_level = 6;

    while (startScreen(window, open_sans_semibold, open_sans)) {
        levelSelect(window, open_sans, max_level);
    }
}