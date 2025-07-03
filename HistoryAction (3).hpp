//vanunuraz@gmail.com
#ifndef HISTORYACTION_HPP
#define HISTORYACTION_HPP
#include <iostream>
#include <string>
#include "Player.hpp"
namespace coup{
    struct HistoryAction{
        string action;
        Player *playerBy;
        Player *playerOn;
        int coins;
    };   
}
#endif