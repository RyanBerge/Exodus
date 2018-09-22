#include "room.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "data_file.h"
#include "utilities.h"

Room::Room()
{
}

Room::Room(RoomID id) : id{id}
{
    light_layer->create(1200, 800);
}

void Room::Update(sf::Time elapsed, sf::RenderWindow& window, Player& player)
{
    for (auto& entity : entities)
    {
        entity.Update(elapsed, window);
    }

    for (auto& enemy : enemies)
    {
        enemy.Update(elapsed, window, player);
    }
}

void Room::Draw(sf::RenderWindow& window)
{
    background.Draw(window);

    for (auto& entity : entities)
    {
        entity.Draw(window);
    }

    for (auto& enemy : enemies)
    {
        enemy.Draw(window);
    }
}

void Room::DrawLighting(sf::RenderWindow& window)
{
    if (light_level == 0)
    {
        return;
    }

    light_layer->clear(sf::Color(0, 0, 0, 0));
    sf::RectangleShape rect(sf::Vector2f(1200, 800));
    rect.setFillColor(sf::Color(0, 0, 0, 255));
    light_layer->draw(rect);

    for (auto& entity : entities)
    {
        entity.DrawLights(*light_layer);
    }

    light_layer->display();

    sf::Sprite light_sprite(light_layer->getTexture());

    light_sprite.setColor(sf::Color(255, 255, 255, light_level));

    window.draw(light_sprite);
}

bool Room::Load()
{
    std::string filepath = "data/rooms/" + id.area + "/" + std::to_string(id.x) + "-" + std::to_string(id.y) + ".txt";

    DataFile data_file;
    if (!data_file.Open(filepath))
    {
        std::cerr << "Exodus: Room could not be loaded: " << filepath << std::endl;
        return false;
    }

    while (data_file.MoreKeys())
    {
        auto data = data_file.GetKey();
        if (data.key == "Entity")
        {
            auto ss = data.ss;

            std::string identifier;
            std::string start_animation;
            int x, y;

            *ss >> identifier;
            *ss >> start_animation;
            *ss >> x;
            *ss >> y;

            std::string subkey{""};
            std::string label{""};
            std::string dungeon_state{""};

            *ss >> subkey;
            while (subkey != "")
            {
                auto subkey_ss = std::stringstream(subkey);
                subkey = "";
                char letter;
                letter = subkey_ss.get();
                while (letter != '=')
                {
                    subkey += letter;
                    letter = subkey_ss.get();
                }

                if (subkey == "label")
                {
                    subkey_ss >> label;
                }
                else if (subkey == "dungeonState")
                {
                    subkey_ss >> dungeon_state;
                }

                subkey = "";
                *ss >> subkey;
            }

            if (dungeon_state == "" || Utilities::CheckDungeonState(dungeon_state, id))
            {
                Entity entity(identifier, label);
                entity.GetSprite().setPosition(x, y);
                entity.SetAnimation(start_animation);
                entities.push_back(entity);
            }
        }
        else if (data.key == "Enemy")
        {
            auto ss = data.ss;

            std::string identifier;
            std::string start_animation;
            int x, y;

            *ss >> identifier;
            *ss >> start_animation;
            *ss >> x;
            *ss >> y;

            Enemy enemy(identifier);
            enemy.GetSprite().setPosition(x, y);
            enemy.SetAnimation(start_animation);
            enemies.push_back(enemy);
        }
        else if (data.key == "Background")
        {
            auto ss = data.ss;

            std::string sprite_path;

            *ss >> sprite_path;

            Spritesheet::Config config;
            config.frames.push_back(Spritesheet::Frame{sf::IntRect(0, 0, 1200, 800), sf::Vector2f(0, 0)});
            background = Spritesheet(sprite_path, config);
            background.SetFrame(0);
        }
        else if (data.key == "Portal")
        {
            auto ss = data.ss;

            float x, y, width, height;
            std::string area;
            int zone_x, zone_y;
            float spawn_x, spawn_y;

            *ss >> x;
            *ss >> y;
            *ss >> width;
            *ss >> height;
            *ss >> area;
            *ss >> zone_x;
            *ss >> zone_y;
            *ss >> spawn_x;
            *ss >> spawn_y;

            sf::FloatRect hitbox(x, y, width, height);
            RoomID room_id{area, zone_x, zone_y};

            Portal portal{room_id, hitbox, sf::Vector2f{spawn_x, spawn_y}};

            portals.push_back(portal);
        }
        else if (data.key == "Trigger")
        {
            auto ss = data.ss;
            std::string identifier;
            float x, y, width, height;

            *ss >> identifier;
            *ss >> x;
            *ss >> y;
            *ss >> width;
            *ss >> height;

            std::string subkey{""};
            std::string dungeon_state{""};

            *ss >> subkey;
            while (subkey != "")
            {
                auto subkey_ss = std::stringstream(subkey);
                subkey = "";
                char letter;
                letter = subkey_ss.get();
                while (letter != '=')
                {
                    subkey += letter;
                    letter = subkey_ss.get();
                }

                if (subkey == "dungeonState")
                {
                    subkey_ss >> dungeon_state;
                }

                subkey = "";
                *ss >> subkey;
            }

            if (dungeon_state == "" || Utilities::CheckDungeonState(dungeon_state, id))
            {
                sf::FloatRect hitbox(x, y, width, height);
                WorldTrigger trigger{identifier, hitbox};
                triggers.push_back(trigger);
            }
        }
        else if (data.key == "LightLevel")
        {
            auto ss = data.ss;
            *ss >> light_level;
        }
    }

    Utilities::SetDungeonState("Explored", id);

    return true;
}
