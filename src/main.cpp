#include <SFML/Window/Event.hpp>
#include "settings.h"
#include "game_manager.h"

int main()
{
    std::string settings_path("data/settings.txt");

    Settings settings(settings_path);
    GameManager game_manager(settings);

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    sf::Clock clock;


    Spritesheet::Config play_button_config;
    play_button_config.frames.push_back(sf::IntRect(0, 0, 400, 100));
    play_button_config.frames.push_back(sf::IntRect(0, 100, 400, 100));
    play_button_config.frames.push_back(sf::IntRect(0, 200, 400, 100));

    CursorButton play_button("assets/PlayButton.png", play_button_config);
    play_button.GetSprite().setPosition(sf::Vector2f(200, 100));

    while (window.isOpen())
    {
        sf::Time elapsed = clock.restart();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        game_manager.Update(elapsed, window);
        game_manager.Draw(window);

        play_button.Update(elapsed, window);
        play_button.Draw(window);

        window.display();
    }


    return 0;
}
