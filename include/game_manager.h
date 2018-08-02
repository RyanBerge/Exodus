#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

enum class Scene
{
    MainMenu, Game
};

class GameManager
{
public:
    GameManager();

    void Update();
    void Draw();

private:
    Scene current_scene;
};

#endif // GAME_MANAGER_H
