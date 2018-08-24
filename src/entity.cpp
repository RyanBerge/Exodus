#include "entity.h"

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "data_file.h"

Entity::Entity(std::string identifier)
{
    load("data/entities/" + identifier + ".txt");
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
            int start_frame;
            int end_frame;
            float animation_speed;
            std::string animation_name;
            *ss >> animation_name;
            *ss >> start_frame;
            *ss >> end_frame;
            *ss >> animation_speed;

            animations.push_back(Spritesheet::Animation{animation_name, start_frame, end_frame, animation_speed});
        }
        else if (data.key == "RandomFrame")
        {
            auto ss = data.ss;
            std::string asd;
            //*ss >> asd;
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

void Entity::SetAnimation(std::string animation_name)
{
    sprite.SetAnimation(animation_name);
}

sf::Sprite& Entity::GetSprite()
{
    return sprite.GetSprite();
}

bool Entity::HasCollisions()
{
    return collisions;
}