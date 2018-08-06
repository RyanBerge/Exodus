#include <SFML/Window/Event.hpp>
#include "settings.h"
#include "game_manager.h"

int main()
{
    std::string settings_path("data/settings.txt");

    Settings settings(settings_path);
    //GameManager game_manager(settings);

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    sf::Clock clock;

    MainMenu menu;

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

        //game_manager.Update(elapsed, window);
        //game_manager.Draw(window);

        menu.Update(elapsed, window);
        menu.Draw(window);

        window.display();
    }


    return 0;
}
