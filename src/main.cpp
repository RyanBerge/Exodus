#include "settings.h"
#include "game_manager.h"

#ifdef DEBUG
#include <iostream>
#endif

int main()
{
    #ifdef DEBUG
    std::string settings_path("data/settings.txt");
    #else
    std::string settings_path("../data/settings.txt");
    #endif

    Settings settings(settings_path);
    GameManager game_manager();

    return 0;
}
