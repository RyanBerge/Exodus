#ifndef HUD_H
#define HUD_H

#include <SFML/Graphics/RectangleShape.hpp>
#include "graphics/spritesheet.h"

#include <vector>

#define HUD_DRAW_X 10000
#define HUD_DRAW_Y 10000

class Hud
{
public:
    Hud();

    void Update(sf::Time elapsed, sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);

    void SetHealth(int hp);
    void SetViewport(sf::FloatRect viewport);

private:
    void load(std::string filepath);
    Spritesheet load_heart(std::string filepath);

    int health{5};
    int max_health{5};
    std::vector<Spritesheet> hearts;
    sf::FloatRect viewport;

};

#endif // HUD_H
