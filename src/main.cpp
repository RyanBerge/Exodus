#include <SFML/Window/Event.hpp>
#include "settings.h"
#include "game_manager.h"

int main()
{
    Settings::LoadSettings("data/settings.txt");

    GameManager game_manager;

    sf::RenderWindow window(sf::VideoMode(Settings::video_resolution.x, Settings::video_resolution.y), "My window");

    sf::Clock clock;

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

        window.display();
    }


    return 0;
}
