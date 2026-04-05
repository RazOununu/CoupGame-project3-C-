//vanunuraz@gmail.com
//PlayerFactory is responsible for creating Player instances based on a role string.
//It isolates all role-specific construction logic from the Game class, so Game does not need to include or depend on any concrete role headers.

#ifndef PLAYERFACTORY_HPP
#define PLAYERFACTORY_HPP

#include <string>
#include "Player.hpp"

namespace coup{
    class Game;
    Player* createPlayer(Game& game, const std::string& name, const std::string& role);
}

#endif