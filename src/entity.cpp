#include "entity.h"

#include <cstdlib>

Entity::Entity(std::string identifier)
{
    if (identifier == "tree_0")
    {
        Spritesheet::Config config;
        config.frames.push_back(sf::IntRect(0, 0, 100, 100));
        config.frames.push_back(sf::IntRect(0, 100, 100, 100));
        config.frames.push_back(sf::IntRect(0, 200, 100, 100));
        sprite = Spritesheet("assets/OverworldTree.png", config);

        sprite.SetFrame(rand());
    }
}

void Entity::Update(sf::Time elapsed, sf::RenderWindow& window)
{

}

void Entity::Draw(sf::RenderWindow& window)
{
    sprite.Draw(window);
}

sf::Sprite& Entity::GetSprite()
{
    return sprite.GetSprite();
}
