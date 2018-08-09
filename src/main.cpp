#include <SFML/Window/Event.hpp>
#include "settings.h"
#include "game_manager.h"

int main()
{
    Settings::LoadSettings("data/settings.txt");


    sf::RenderWindow window(sf::VideoMode(Settings::video_resolution.x, Settings::video_resolution.y), "My window");

    GameManager game_manager(window);

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Time elapsed = clock.restart();
        sf::Event event;

        window.clear(sf::Color::Black);

        game_manager.Update(elapsed, window);
        game_manager.Draw(window);

        window.display();

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::Resized)
            {
                sf::Vector2u ratio = sf::Vector2u(event.size.width, event.size.height);

                game_manager.Resize(ratio);
            }
        }
    }


    return 0;
}
