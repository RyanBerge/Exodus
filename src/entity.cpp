#include "entity.h"

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <set>

#include "data_file.h"
#include "utilities.h"
#include "global.h"

namespace {
    static int id_counter{0};
}

Entity::Entity(std::string identifier, std::string label) : label{label}, type{identifier}
{
    load("data/entities/" + identifier + ".txt");
    id = id_counter++;
}

void Entity::load(std::string filepath)
{
    std::string sprite_path("assets/");
    Spritesheet::Config config;
    std::vector<Spritesheet::Animation> animations;
    std::vector<std::list<Spritesheet::LightConfig>> light_configs;
    std::vector<sf::FloatRect> hitboxes;
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
        else if (data.key == "EntityCollisions")
        {
            auto ss = data.ss;

            *ss >> entity_collisions;
        }
        else if (data.key == "Frames")
        {
            auto ss = data.ss;
            *ss >> config;
        }
        else if (data.key == "Lights")
        {
            auto ss = data.ss;
            light_configs = Utilities::ReadLights(*ss);
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
        else if (data.key == "Damage")
        {
            auto ss = data.ss;
            *ss >> damage;
        }
        else if (data.key == "Healing")
        {
            auto ss = data.ss;
            *ss >> healing;
        }
        else if (data.key == "Health")
        {
            auto ss = data.ss;
            *ss >> health;
        }
        else if (data.key == "Knockback")
        {
            auto ss = data.ss;
            *ss >> knockback;
        }
        else if (data.key == "Trigger")
        {
            auto ss = data.ss;
            std::string collision_key, collision_type;
            *ss >> collision_key;
            *ss >> collision_type;
            triggers.push_back(Trigger{collision_key, collision_type});
        }
        else if (data.key == "Hitbox")
        {
            auto ss = data.ss;
            std::string line = ss->str();
            auto sit = line.begin();

            while (sit != line.end())
            {
                while (sit != line.end() && *sit != '[')
                {
                    ++sit;
                }

                if (sit == line.end())
                {
                    break;
                }

                auto s_sit = ++sit;

                while (sit != line.end() && *sit != ']')
                {
                    ++sit;
                }

                std::string box(s_sit, sit);
                if (box == "default")
                {
                    hitboxes.push_back(sf::FloatRect{-1, -1, -1, -1});
                }
                else if (box == "")
                {
                    hitboxes.push_back(sf::FloatRect{0, 0, 0, 0});
                }
                else
                {
                    std::stringstream stream(box);
                    float left, top, width, height;
                    stream >> left;
                    stream >> top;
                    stream >> width;
                    stream >> height;

                    hitboxes.push_back(sf::FloatRect{left, top, width, height});
                }
            }
        }
        else if (data.key == "Behavior")
        {
            auto ss = data.ss;
            *ss >> behavior;
        }
        else if (data.key == "Damage")
        {
            auto ss = data.ss;
            *ss >> damage;
        }
        else if (data.key == "Knockback")
        {
            auto ss = data.ss;
            *ss >> knockback;
        }
        else if (data.key == "Movespeed")
        {
            auto ss = data.ss;
            *ss >> movespeed;
        }
        else if (data.key == "Loot")
        {
            auto ss = data.ss;
            std::string identifier;
            float drop_chance;
            *ss >> identifier;
            *ss >> drop_chance;

            Loot loot{identifier, drop_chance};
            loot_table.push_back(loot);
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

    std::set<std::string> light_ids;

    for (unsigned i = 0; i < light_configs.size(); ++i)
    {
        for (auto& light : light_configs[i])
        {
            light_ids.insert(light.identifier);
            sprite.AddLightFrame(i, light);
        }
    }

    for (auto& light_id : light_ids)
    {
        lights.push_back(Entity(light_id, ""));
    }

    sprite.SetHitboxes(hitboxes);
}

void Entity::Update(sf::Time elapsed, sf::RenderWindow& window, Player& player)
{
    sprite.Update(elapsed, window);
    for (auto& light : lights)
    {
        light.Update(elapsed, window, player);
    }


    if (behavior == "Spiny")
    {
        spinyUpdate(elapsed, window, player);
    }
    else if (behavior == "Lizard")
    {
        lizardUpdate(elapsed, window, player);
    }

    if (!colliding)
    {
        collision_timer = 0;
    }
    colliding = false;

    if (is_burning)
    {
        burn_duration += elapsed.asSeconds();
        if (burn_duration > 1)
        {
            health -= 1;
            burn_duration = 0;
        }
    }

    if (health <= 0)
    {
        dead = true;
    }
}

void Entity::Draw(sf::RenderWindow& window)
{
    sprite.Draw(window);
}

void Entity::DrawLights(sf::RenderTexture& target)
{
    for (auto& light_config : sprite.GetLights())
    {
        for (auto& light_entity : lights)
        {
            if (light_config.identifier == light_entity.GetType())
            {
                float x = -sprite.GetSprite().getOrigin().x + sprite.GetSprite().getPosition().x + light_config.x;
                float y = -sprite.GetSprite().getOrigin().y + sprite.GetSprite().getPosition().y + light_config.y;
                light_entity.GetSprite().setPosition(x, y);

                if (light_entity.GetAnimation() != light_config.animation)
                {
                    light_entity.SetAnimation(light_config.animation);
                }

                light_entity.drawLighting(target);
                break;
            }
        }
    }
}

void Entity::drawLighting(sf::RenderTexture& target)
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

bool Entity::SetBurning()
{
    if (!sprite.SetAnimation("Burning"))
    {
        return false;
    }
    is_burning = true;
    burn_duration = 0;
    return true;
}

Collision Entity::Collide(sf::Time elapsed, Player& player)
{
    if (dead)
    {
        return Collision{false, 0, 0, 0};
    }

    if (type == "heart")
    {
        if (player.Heal(healing))
        {
            dead = true;
        }
    }

    collision_timer += elapsed.asSeconds();
    colliding = true;
    return Collision{collisions, collision_timer, damage, knockback};
}

Collision Entity::EntityCollide(sf::Time elapsed, Entity& other)
{
    if (this->type == "falling_torch" && other.type == "spiny")
    {
        if ((sprite.GetAnimation() == "BurnRight" || sprite.GetAnimation() == "BurnLeft") && other.GetAnimation() != "Burning")
        {
            other.SetBurning();
        }
    }

    return Collision{entity_collisions, 0, 0, 0};
}

sf::Vector2f Entity::GetKnockbackDirection(Player& player)
{
    sf::FloatRect player_bounds = player.GetSprite().getGlobalBounds();
    sf::Vector2f player_position = {player_bounds.left + (player_bounds.width / 2), player_bounds.top + (player_bounds.height / 2)};
    sf::FloatRect hitbox = sprite.GetHitbox();
    sf::Vector2f center = sf::Vector2f{hitbox.left + (hitbox.width / 2), hitbox.top + (hitbox.height / 2)};

    sf::Vector2f direction = player_position - center;
    return Utilities::Normalize(direction);
}

void Entity::spinyUpdate(sf::Time elapsed, sf::RenderWindow& window, Player& player)
{
    auto direction = follow(player.GetSprite().getPosition(), elapsed);
    if (sprite.GetHitbox().intersects(player.GetHitbox()))
    {
        player.Damage(damage, knockback, direction);
    }
}

void Entity::lizardUpdate(sf::Time elapsed, sf::RenderWindow& window, Player& player)
{
    wander(elapsed);
    sprite.GetSprite().move(velocity * elapsed.asSeconds());
    auto direction = getPlayerDirection(player.GetSprite().getPosition());
    if (sprite.GetHitbox().intersects(player.GetHitbox()))
    {
        player.Damage(damage, knockback, direction);
    }
}

sf::Vector2f Entity::follow(sf::Vector2f player_position, sf::Time elapsed)
{
    auto direction = getPlayerDirection(player_position);
    velocity.x = direction.x * movespeed * elapsed.asSeconds();
    velocity.y = direction.y * movespeed * elapsed.asSeconds();

    velocity = Global::MoveEntity(sprite.GetHitbox(), velocity, *this);
    sprite.GetSprite().move(velocity);
    return direction;
}

void Entity::wander(sf::Time elapsed)
{
    static int direction = 0; // None
    static bool moving = false;
    static float behavior_distance = 0;
    static float current_distance = 0;

    if (direction == 0)
    {
        direction = (rand() % 4) + 1;
        behavior_distance = (rand() % 50) + 50;
    }

    current_distance += elapsed.asSeconds() * movespeed;
    if (current_distance >= behavior_distance)
    {
        if (moving)
        {
            moving = false;
            behavior_distance = (rand() % 50) + 50;
        }
        else
        {
            moving = true;
            direction = (rand() % 4) + 1;
            behavior_distance = (rand() % 50) + 50;
        }
        current_distance = 0;

        switch (direction)
        {
            case 1:
            {
                if (moving)
                {
                    sprite.SetAnimation("Left");
                    velocity = {-movespeed, 0};
                }
                else
                {
                    sprite.StopAnimation();
                    velocity = {0, 0};
                }
            }
            break;
            case 2:
            {
                if (moving)
                {
                    sprite.SetAnimation("Right");
                    velocity = {movespeed, 0};
                }
                else
                {
                    sprite.StopAnimation();
                    velocity = {0, 0};
                }
            }
            break;
            case 3:
            {
                if (moving)
                {
                    sprite.SetAnimation("Up");
                    velocity = {0, -movespeed};
                }
                else
                {
                    sprite.StopAnimation();
                    velocity = {0, 0};
                }
            }
            break;
            case 4:
            {
                if (moving)
                {
                    sprite.SetAnimation("Down");
                    velocity = {0, movespeed};
                }
                else
                {
                    sprite.StopAnimation();
                    velocity = {0, 0};
                }
            }
            break;
        }
    }
}

sf::Vector2f Entity::getPlayerDirection(sf::Vector2f player_position)
{
    sf::Vector2f position = sprite.GetSprite().getPosition();
    sf::Vector2f direction = player_position - position;
    return Utilities::Normalize(direction);
}

sf::Sprite& Entity::GetSprite()
{
    return sprite.GetSprite();
}

std::string Entity::GetAnimation()
{
    return sprite.GetAnimation();
}

std::string Entity::GetLabel()
{
    return label;
}

std::list<Entity>& Entity::GetLights()
{
    return lights;
}

std::list<Trigger>& Entity::GetTriggers()
{
    return triggers;
}

std::list<Loot>& Entity::GetLootTable()
{
    return loot_table;
}

bool Entity::HasCollisions()
{
    return collisions;
}

sf::FloatRect Entity::GetHitbox()
{
    return sprite.GetHitbox();
}

bool Entity::IsDead()
{
    return dead;
}

void Entity::Kill()
{
    dead = true;
}
