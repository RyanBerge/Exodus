#include "utilities.h"

#include <cmath>
#include <sstream>

namespace Utilities
{
    sf::Vector2f Normalize(sf::Vector2f vector)
    {
        float magnitude = sqrt(vector.x * vector.x + vector.y * vector.y);
        if (magnitude != 0)
        {
            return sf::Vector2f(vector.x / magnitude, vector.y / magnitude);
        }
        else
        {
            return vector;
        }
    }


    bool CheckCollision(sf::FloatRect left, sf::FloatRect right)
    {
        if (left.intersects(right))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    Spritesheet::Animation ReadAnimation(std::stringstream& ss)
    {
        int start_frame, end_frame, center_x{0}, center_y{0};
        float animation_speed;
        std::string next_animation{"Loop"};
        std::string animation_name;
        ss >> animation_name;
        ss >> start_frame;
        ss >> end_frame;
        ss >> animation_speed;

        std::string subkey("");

        ss >> subkey;
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

            if (subkey == "x")
            {
                subkey_ss >> center_x;
            }
            else if (subkey == "y")
            {
                subkey_ss >> center_y;
            }
            else if (subkey == "next_animation")
            {
                subkey_ss >> next_animation;
            }

            subkey = "";
            ss >> subkey;
        }

        return Spritesheet::Animation{animation_name, start_frame, end_frame, animation_speed, center_x, center_y, next_animation};
    }

    std::vector<std::list<Spritesheet::LightConfig>> ReadLights(std::stringstream& source_stream)
    {
        std::vector<std::list<Spritesheet::LightConfig>> lights;

        std::string line = source_stream.str();
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

            std::stringstream ss(std::string(s_sit, sit));
            std::list<Spritesheet::LightConfig> light_list;

            if (ss.str() != "")
            {
                int x, y;
                std::string identifier, animation;
                bool more = false;

                do
                {
                    more = false;
                    ss >> x;
                    ss >> y;
                    ss >> identifier;
                    ss >> animation;

                    if (*(animation.end() - 1) == ',')
                    {
                        more = true;
                        animation = std::string(animation.begin(), animation.end() - 1);
                    }

                    light_list.push_back(Spritesheet::LightConfig{x, y, identifier, animation});
                }
                while (more);
            }

            lights.push_back(light_list);
        }

        return lights;
    }
}
