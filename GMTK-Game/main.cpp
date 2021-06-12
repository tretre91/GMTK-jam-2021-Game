#include <MyGL/mygl.hpp>
#include "grid.hpp"

void endScreen(my::GLWindow& window, my::Font& font) {
    my::Text end_text("Thank you for playing!", font, 40);
    end_text.setPosition(window.getSize() / 2, true);
    end_text.setColor(my::Color::white);
    my::Text exit_text("Press Escape to quit", font, 20);
    exit_text.setPosition(window.getSize().x / 2, end_text.getPosition().y - 40, true);
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
                    window.close();
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

int main() {
    my::GLWindow window(800, 700, "Untitled Robot Game", 4);
    window.setFramerate(60);

    my::Font open_sans("Ressources/OpenSans-Regular.ttf");
    my::Text lost_text("You lost, Press R to restart", open_sans, 30);
    lost_text.setPosition(window.getSize().x / 2, 30, true);
    lost_text.setColor(my::Color::white);

    my::Text win_text("Level complete! Press Enter to go to the next level", open_sans, 30);
    win_text.setPosition(window.getSize().x / 2.0f, 30, true);
    win_text.setColor(my::Color::white);

    int currentLevel = 1;
    const int max_level = 3;
    Grid grid("Levels/lvl" + std::to_string(currentLevel) + ".txt", 800, 600, window.getSize().y - 50);
    my::Text level("Level " + std::to_string(currentLevel), open_sans, 35);
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
                    window.close();
                    break;
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
                        currentLevel++;
                        if (currentLevel > max_level) {
                            endScreen(window, open_sans);
                        } else {
                            grid = Grid("Levels/lvl" + std::to_string(currentLevel) + ".txt", 800, 600, window.getSize().y - 50);
                            level.setContent("Level " + std::to_string(currentLevel));
                            state = GameState::in_progress;
                        }
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
            window.draw(lost_text);
        } else if (state == GameState::won) {
            window.draw(win_text);
        }

        window.display();
    }
}