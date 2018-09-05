#include "entity.h"

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "data_file.h"
#include "utilities.h"

namespace {
    static int id_counter{0};
}

Entity::Entity(std::string identifier) : type{identifier}
{
    load("data/entities/" + identifier + ".txt");
    id = id_counter++;
}

void Entity::load(std::string filepath)
{
    std::string sprite_path("assets/");
    Spritesheet::Config config;
    std::vector<Spritesheet::Animation> animations;
    bool random_frame;

    DataFile data_file;
    if (!data_file.Open(filepath))
    {
        std::cerr << "Exodus: Entity could not be loaded: " << filepath << std::endl;
        return;
    }

    while (data_file.MoreKeys())
    {
        auto data = data_file.GetKey();
        if (data.key == "Sprite")
        {
            auto ss = data.ss;
            std::string path;

            *ss >> path;
            sprite_path += path;
        }
        else if (data.key == "Collisions")
        {
            auto ss = data.ss;

            *ss >> collisions;
        }
        else if (data.key == "Frames")
        {
            auto ss = data.ss;
            *ss >> config;
        }
        else if (data.key == "Animation")
        {
            auto ss = data.ss;
            animations.push_back(Utilities::ReadAnimation(*ss));
        }
        else if (data.key == "RandomFrame")
        {
            auto ss = data.ss;
            *ss >> random_frame;
        }
    }

    sprite = Spritesheet(sprite_path, config);
    if (random_frame)
    {
        sprite.SetFrame(rand() % config.frames.size());
    }

    for (auto animation : animations)
    {
        sprite.AddAnimation(animation);
    }
}

void Entity::Update(sf::Time elapsed, sf::RenderWindow& window)
{
    sprite.Update(elapsed, window);
}

void Entity::Draw(sf::RenderWindow& window)
{
    sprite.Draw(window);
}

void Entity::DrawLighting(sf::RenderTexture& target)
{
    sprite.DrawLighting(target);
}

int Entity::GetId()
{
    return id;
}

std::string Entity::GetType()
{
    return type;
}

void Entity::SetAnimation(std::string animation_name)
{
    sprite.SetAnimation(animation_name);
}

float Entity::Collide(sf::Time elapsed)
{
    if (!colliding)
    {
        collision_timer = 0;
    }

    collision_timer += elapsed.asSeconds();
    colliding = true;
    return collision_timer;
}

sf::Sprite& Entity::GetSprite()
{
    return sprite.GetSprite();
}

bool Entity::HasCollisions()
{
    return collisions;
}
