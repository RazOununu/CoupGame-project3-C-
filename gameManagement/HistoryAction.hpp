//vanunuraz@gmail.com
#ifndef HISTORYACTION_HPP
#define HISTORYACTION_HPP
#include <iostream>
#include <string>
#include "Player.hpp"
namespace coup{
    struct HistoryAction{
        std::string action;
        // playerBy and playerOn are non-owning raw pointers into Game's player list.
        // They are valid for the lifetime of the Game object. Game deletes all players
        // only in its destructor, at which point history is also destroyed. No code
        // should dereference these pointers after Game's destructor runs.
        Player *playerBy;
        Player *playerOn;
        int coins;
    };
}
#endif
