#include "world_manager.h"
#include "settings.h"
#include "utilities.h"
#include "global.h"

#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <thread>

#define PORTAL_TRANSITION_TIME 1
#define PUSH_TRIGGER_DURATION 0.5

WorldManager::WorldManager() : current_room{new Room(RoomID{"Overworld", 6, 3})}
{
    player.GetSprite().setPosition(sf::Vector2f(500, 500));
}

void WorldManager::Initialize()
{
    fade.setPosition(0, 0);
    fade.setFillColor(sf::Color(0, 0, 0, 0));

    trigger_functions["collapseTorch"] = std::bind(&WorldManager::collapseTorch, this, std::placeholders::_1);
    trigger_functions["fallingBoulder"] = std::bind(&WorldManager::fallingBoulder, this, std::placeholders::_1);
    trigger_functions["spinyPuzzle"] = std::bind(&WorldManager::spinyPuzzle, this, std::placeholders::_1);

    Global::MovePlayer = std::bind(&WorldManager::movePlayer, this, std::placeholders::_1, std::placeholders::_2);
    Global::MoveEntity = std::bind(&WorldManager::moveEntity, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    Global::ChangeRoom = std::bind(&WorldManager::changeRoom, this, std::placeholders::_1);
    Global::Resume = std::bind(&WorldManager::Resume, this);

    pause_menu.Initialize();
}

void WorldManager::Update(sf::Time elapsed, sf::RenderWindow& window)
{
    static sf::Vector2f spawn{0, 0};
    elapsed_seconds = elapsed;

    if (!paused && !window.hasFocus())
    {
        paused = true;
    }

    if (paused)
    {
        auto old_view = window.getView();
        window.setView(sf::View(sf::FloatRect(0, 0, 1200, 800)));
        Resize(window.getSize(), window);
        pause_menu.Update(elapsed, window);
        window.setView(old_view);
    }
    else
    {
        if (room_transition == sf::Vector2i{0, 0} && !portal_transition)
        {
            current_room->Update(elapsed, window, player);
            player.Update(elapsed, window);

            for (auto& portal : current_room->portals)
            {
                if (portal.hitbox.intersects(player.GetSprite().getGlobalBounds()))
                {
                    Utilities::SetAllDungeonStates("Unexplored", current_room->id);
                    setNewRoom(portal.id);
                    portal_transition = true;
                    portal_in = false;
                    transition_time = -PORTAL_TRANSITION_TIME / 2;
                    spawn = portal.spawn;
                    break;
                }
            }
        }
        else if (portal_transition)
        {
            transition_time += elapsed.asSeconds();
            if (!portal_in && transition_time > 0)
            {
                portal_in = true;
                current_room = new_room;
                if (!Utilities::CheckDungeonState("Completed", current_room->id))
                {
                    Utilities::RemoveDungeonState("Unexplored", current_room->id);
                    Utilities::AddDungeonState("Explored", current_room->id);
                }
                player.GetSprite().setPosition(spawn);
                loadAdjacentRooms();
            }

            if (transition_time > 0.5)
            {
                transition_time = 0;
                portal_transition = false;
            }
        }

        if (game_event == "Falling Boulder")
        {
            updateFallingBoulderEvent(elapsed, window);
        }

        if (game_event == "Spiny Puzzle")
        {
            updateSpinyPuzzle(elapsed, window);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            paused = true;
        }

        for (auto& trigger : current_room->triggers)
        {
            if (trigger.hitbox.intersects(player.GetSprite().getGlobalBounds()))
            {
                trigger_flags[trigger.callback_key].push_back(nullptr);
            }
        }

        for (auto& flag : trigger_flags)
        {
            for (auto& trigger : flag.second)
            {
                trigger_functions[flag.first](trigger, elapsed);
            }
        }

        trigger_flags.clear();

        auto it = current_room->entities.begin();
        while (it != current_room->entities.end())
        {
            if (it->IsDead())
            {
                current_room->entities.erase(it);
                it = current_room->entities.begin();
            }
            ++it;
        }
    }
}

void WorldManager::Draw(sf::RenderWindow& window)
{
    if (room_transition == sf::Vector2i{0, 0} && !portal_transition)
    {
        current_room->Draw(window);
        player.Draw(window);
        current_room->DrawLighting(window);
    }
    else if (room_transition != sf::Vector2i{0, 0})
    {
        sf::View current_room_view = window.getView();
        sf::View new_room_view = window.getView();

        new_room_view.move(1200 * -room_transition.x, 800 * -room_transition.y);

        if (!paused)
        {
            float transition_speed = (room_transition.x == 0 ? 400 : 600) * elapsed_seconds.asSeconds();
            new_room_view.move(transition_speed * room_transition.x, transition_speed * room_transition.y);
            current_room_view.move(transition_speed * room_transition.x, transition_speed * room_transition.y);
        }

        window.setView(new_room_view);
        new_room->Draw(window);

        window.setView(current_room_view);
        current_room->Draw(window);

        player.Draw(window);
        current_room->DrawLighting(window);

        window.setView(new_room_view);
        new_room->DrawLighting(window);
        window.setView(current_room_view);

        if (((room_transition.x > 0) ? window.getView().getCenter().x >= 1800 : window.getView().getCenter().x <= -600) ||
            ((room_transition.y > 0) ? window.getView().getCenter().y >= 1200 : window.getView().getCenter().y <= -400))
        {
            current_room = new_room;
            if (!Utilities::CheckDungeonState("Completed", current_room->id))
            {
                Utilities::RemoveDungeonState("Unexplored", current_room->id);
                Utilities::AddDungeonState("Explored", current_room->id);
            }
            loadAdjacentRooms();
            player.GetSprite().move(1200 * -room_transition.x, 800 * -room_transition.y);
            sf::View view = window.getView();
            view.move(1200 * -room_transition.x, 800 * -room_transition.y);
            window.setView(view);
            current_room->Draw(window);
            room_transition.x = 0;
            room_transition.y = 0;
        }
    }

    if (paused)
    {
        auto old_view = window.getView();
        window.setView(sf::View(sf::FloatRect(0, 0, 1200, 800)));
        Resize(window.getSize(), window);
        pause_menu.Draw(window);
        window.setView(old_view);
    }

    if (portal_transition)
    {
        current_room->Draw(window);
        player.Draw(window);
        current_room->DrawLighting(window);
        fade.setFillColor(sf::Color(0, 0, 0, 1 - std::abs(transition_time) * 2 * 255));
        window.draw(fade);
    }
}

void WorldManager::LoadSave(sf::RenderWindow& window)
{
    Room::InitDungeonStates();

    current_room = std::shared_ptr<Room>(new Room(RoomID{"Dungeon1", 2, 1}));
    current_room->Load();
    if (!Utilities::CheckDungeonState("Completed", current_room->id))
    {
        Utilities::RemoveDungeonState("Unexplored", current_room->id);
        Utilities::AddDungeonState("Explored", current_room->id);
    }
    Resize(sf::Vector2u(Settings::video_resolution.x, Settings::video_resolution.y), window);

    loadAdjacentRooms();
}

void WorldManager::loadAdjacentRooms()
{
    std::thread right(&WorldManager::loadRoom, this, RoomID{current_room->id.area, current_room->id.x + 1, current_room->id.y});
    std::thread left(&WorldManager::loadRoom, this, RoomID{current_room->id.area, current_room->id.x - 1, current_room->id.y});
    std::thread down(&WorldManager::loadRoom, this, RoomID{current_room->id.area, current_room->id.x, current_room->id.y + 1});
    std::thread up(&WorldManager::loadRoom, this, RoomID{current_room->id.area, current_room->id.x, current_room->id.y - 1});

    right.detach();
    left.detach();
    down.detach();
    up.detach();

    for (auto& portal : current_room->portals)
    {
        std::thread p(&WorldManager::loadRoom, this, portal.id);
        p.detach();
    }
}

void WorldManager::loadRoom(RoomID id)
{
    Room* room = new Room(id);
    room->Load();

    room_mutex.lock();
    adjacent_rooms.push_back(std::shared_ptr<Room>(room));
    room_mutex.unlock();
}

void WorldManager::setNewRoom(RoomID id)
{
    while (!new_room || !(new_room->id == id))
    {
        room_mutex.lock();
        auto it = adjacent_rooms.begin();
        while (it != adjacent_rooms.end())
        {
            if ((*it)->id == id)
            {
                new_room = *it;
                adjacent_rooms.erase(it);
                break;
            }
            ++it;
        }
        room_mutex.unlock();
    }

    room_mutex.lock();
    adjacent_rooms.clear();
    room_mutex.unlock();
}

void WorldManager::Resize(sf::Vector2u ratio, sf::RenderWindow& window)
{
    float aspect_ratio = 1200.f / 800.f;
    float window_ratio = static_cast<float>(ratio.x) / (static_cast<float>(ratio.y) - 72);

    float viewport_width = 1;
    float viewport_height = 1;
    float viewport_x = 0;
    float viewport_y = 0;

    if (window_ratio > aspect_ratio)
    {
        viewport_width = aspect_ratio / window_ratio;
        viewport_x = (1 - viewport_width) / 2;
    }
    else if (window_ratio < aspect_ratio)
    {
        viewport_height = window_ratio / aspect_ratio;
        viewport_y = (1 - viewport_height) / 2;
    }

    sf::FloatRect view_rect{0, 0, 1200, 800};
    view_rect.left = window.getView().getCenter().x - window.getView().getSize().x / 2;
    view_rect.top = window.getView().getCenter().y - window.getView().getSize().y / 2;

    sf::View view(view_rect);
    view.setViewport(sf::FloatRect(viewport_x, viewport_y + (72.0 / 800.0), viewport_width, viewport_height - (72.0 / 800.0)));
    window.setView(view);
    player.SetHudViewport(sf::FloatRect(viewport_x, viewport_y, viewport_width, viewport_height));
}

sf::Vector2f WorldManager::movePlayer(sf::FloatRect hitbox, sf::Vector2f displacement)
{
    bool vertical = true;
    bool horizontal = true;

    for (auto& entity : current_room->entities)
    {
        auto entity_hitbox = entity.GetHitbox();
        bool temp_vertical = true;
        bool temp_horizontal = true;

        if (entity_hitbox.intersects(sf::FloatRect{hitbox.left + displacement.x, hitbox.top, hitbox.width, hitbox.height}))
        {
            temp_horizontal = false;
        }

        if (entity_hitbox.intersects(sf::FloatRect{hitbox.left, hitbox.top + displacement.y, hitbox.width, hitbox.height}))
        {
            temp_vertical = false;
        }

        if (!temp_horizontal || !temp_vertical)
        {
            Collision collision = entity.Collide(elapsed_seconds, player);

            for (auto& trigger : entity.GetTriggers())
            {
                if (trigger.type == "duration" && collision.collision_timer >= PUSH_TRIGGER_DURATION)
                {
                    bool found = false;
                    for (auto& entity_id : trigger_flags[trigger.callback_key])
                    {
                        if (*reinterpret_cast<int*>(entity_id) == entity.GetId())
                        {
                            found = true;
                            break;
                        }
                    }

                    if (!found)
                    {
                        trigger_flags[trigger.callback_key].push_back(new int(entity.GetId()));
                    }
                }
            }

            if (collision.damage != 0 || collision.knockback != 0)
            {
                sf::Vector2f direction{0, 0};
                if (collision.knockback != 0)
                {
                    direction = entity.GetKnockbackDirection(player);
                }

                player.Damage(collision.damage, collision.knockback, direction);
            }

            if (collision.blocking)
            {
                if (!temp_horizontal)
                {
                    horizontal = false;
                }

                if (!temp_vertical)
                {
                    vertical = false;
                }
            }
        }
    }

    sf::Vector2f final_displacement;

    if (horizontal)
    {
        final_displacement.x = displacement.x;
    }

    if (vertical)
    {
        final_displacement.y = displacement.y;
    }

    return final_displacement;
}

sf::Vector2f WorldManager::moveEntity(sf::FloatRect hitbox, sf::Vector2f displacement, Entity& original_entity)
{
    bool vertical = true;
    bool horizontal = true;

    for (auto& entity : current_room->entities)
    {
        if (entity.GetId() == original_entity.GetId())
        {
            continue;
        }

        auto entity_hitbox = entity.GetHitbox();
        bool temp_vertical = true;
        bool temp_horizontal = true;

        if (entity_hitbox.intersects(sf::FloatRect{hitbox.left + displacement.x, hitbox.top, hitbox.width, hitbox.height}))
        {
            temp_horizontal = false;
        }

        if (entity_hitbox.intersects(sf::FloatRect{hitbox.left, hitbox.top + displacement.y, hitbox.width, hitbox.height}))
        {
            temp_vertical = false;
        }

        if (!temp_horizontal || !temp_vertical)
        {
            Collision collision = entity.EntityCollide(elapsed_seconds, original_entity);

            if (collision.blocking)
            {
                if (!temp_horizontal)
                {
                    horizontal = false;
                }

                if (!temp_vertical)
                {
                    vertical = false;
                }
            }
        }
    }

    sf::Vector2f final_displacement;

    if (horizontal)
    {
        final_displacement.x = displacement.x;
    }

    if (vertical)
    {
        final_displacement.y = displacement.y;
    }

    return final_displacement;
}

void WorldManager::changeRoom(sf::Vector2i room_offset)
{
    game_event = "";
    RoomID id = current_room->id;
    id.x += room_offset.x;
    id.y += room_offset.y;

    setNewRoom(id);

    room_transition = room_offset;
}

void WorldManager::collapseTorch(void* args)
{
    int* entity_id = reinterpret_cast<int*>(args);

    auto& entities = current_room->entities;
    auto it = entities.begin();

    while (it != entities.end() && it->GetId() != *entity_id)
    {
        ++it;
    }

    sf::Vector2f torch_position = it->GetSprite().getPosition();
    sf::FloatRect torch_bounds = it->GetSprite().getGlobalBounds();
    sf::Vector2f player_center;
    player_center.x = player.GetSprite().getGlobalBounds().left + (player.GetSprite().getGlobalBounds().width / 2);
    player_center.y = player.GetSprite().getGlobalBounds().top + (player.GetSprite().getGlobalBounds().height / 2);

    if (player.GetDirection() == Spritesheet::Direction::Down || player.GetDirection() == Spritesheet::Direction::Up)
    {
        return;
    }

    if (player_center.y > torch_bounds.top + torch_bounds.height || player_center.y < torch_bounds.top)
    {
        return;
    }

    Entity falling_torch("falling_torch", "");
    falling_torch.GetSprite().setPosition(torch_position);
    current_room->entities.erase(it);

    if (player.GetDirection() == Spritesheet::Direction::Right)
    {
        falling_torch.SetAnimation("FallRight");
    }
    else if (player.GetDirection() == Spritesheet::Direction::Left)
    {
        falling_torch.SetAnimation("FallLeft");
    }

    current_room->entities.push_back(falling_torch);

    delete entity_id;
}

void WorldManager::fallingBoulder(void*)
{
    game_event = "Falling Boulder";
    Entity falling_boulder("falling_boulder", "Boulder");
    falling_boulder.GetSprite().setPosition(200, -50);
    falling_boulder.SetAnimation("Fall");
    current_room->entities.push_back(falling_boulder);
    player.SetFrozen(true);
    player.SetAnimation("IdleUp");

    auto it = current_room->triggers.begin();
    while (it != current_room->triggers.end())
    {
        if (it->callback_key == "fallingBoulder")
        {
            current_room->triggers.erase(it);
            break;
        }
        ++it;
    }
}

void WorldManager::spinyPuzzle(void*)
{
    game_event = "Spiny Puzzle";
    auto it = current_room->triggers.begin();
    while (it != current_room->triggers.end())
    {
        if (it->callback_key == "spinyPuzzle")
        {
            current_room->triggers.erase(it);
            break;
        }
        ++it;
    }
}

void WorldManager::updateFallingBoulderEvent(sf::Time elapsed, sf::RenderWindow& window)
{
    static std::string phase = "Fall";
    float fall_speed = 500;
    float roll_speed = 70;

    auto it = current_room->entities.begin();

    while (it != current_room->entities.end())
    {
        if (it->GetLabel() == "Boulder")
        {
            if (phase == "Fall")
            {
                it->GetSprite().move(sf::Vector2f(0, fall_speed * elapsed.asSeconds()));
                if (it->GetSprite().getPosition().y >= 150)
                {
                    phase = "Land";

                    auto torch_it = current_room->entities.begin();

                    while (torch_it != current_room->entities.end() && torch_it->GetLabel() != "boulder_torch")
                    {
                        ++torch_it;
                    }

                    Entity falling_torch("falling_torch", "");
                    falling_torch.GetSprite().setPosition(torch_it->GetSprite().getPosition());
                    falling_torch.SetAnimation("FallRight");

                    current_room->entities.erase(torch_it);
                    current_room->entities.insert(it, falling_torch);
                }
            }
            else if (phase == "Land")
            {
                static float timer = 0;
                timer += elapsed.asSeconds();
                if (timer > 0.4)
                {
                    phase = "Roll";
                    it->SetAnimation("Roll");
                    timer = 0;
                }
            }
            else if (phase == "Roll")
            {
                it->GetSprite().move(sf::Vector2f(0, roll_speed * elapsed.asSeconds()));
                if (it->GetSprite().getPosition().y >= 400)
                {
                    phase = "Fade";
                }
            }
            else if (phase == "Fade")
            {
                static float timer = 0;
                timer += elapsed.asSeconds();
                if (timer > 1.75)
                {
                    player.SetFrozen(false);
                    game_event = "";

                    current_room->entities.erase(it);
                    phase = "Fall";
                    timer = 0;
                }
            }
            break;
        }
        ++it;
    }
}

void WorldManager::updateSpinyPuzzle(sf::Time elapsed, sf::RenderWindow& window)
{
    static std::string phase = "Waiting";

    if (phase == "Waiting")
    {
        for (auto& entity : current_room->entities)
        {
            if (entity.GetLabel() == "Spiny")
            {
                return;
            }
        }

        phase = "Success";
    }
    else if (phase == "Success")
    {
        player.SetFrozen(true);
        player.SetAnimation("IdleUp");
        phase = "Fading";
    }
    else if (phase == "Fading")
    {
        static float timer = 0;
        timer += elapsed.asSeconds();

        for (auto& entity : current_room->entities)
        {
            if (entity.GetLabel() == "fakeWall")
            {
                uint8_t opacity = static_cast<uint8_t>(255 - ((timer / 2) * 255));
                entity.GetSprite().setColor(sf::Color{255, 255, 255, opacity});
                if (timer > 2)
                {
                    entity.Kill();
                }
            }
        }

        if (timer > 2)
        {
            player.SetFrozen(false);
            phase = "Waiting";
            game_event = "";
            timer = 0;
            Utilities::RemoveDungeonState("Explored", current_room->id);
            Utilities::AddDungeonState("Completed", current_room->id);
        }
    }
}

void WorldManager::Resume()
{
    paused = false;
}
