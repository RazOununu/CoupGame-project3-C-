//vanunuraz@gmail.com
//The Spy class inherits from Player and represents a special role in the game. It can observe other players' coin counts and
//protect them from arrest temporarily.

#ifndef Spy_hpp
#define Spy_hpp

#include <iostream>
#include <string>
#include "Player.hpp"
#include "Game.hpp"

using namespace std;

namespace coup{
    class Spy: public Player{
    public:
        //Constructor
        Spy(Game &game, const string &name);
        //Role identifier
        string role() const override;
        //Special abilities
        int watchCoins(Player &player); //Observe another player's coin count
        void blockArrest(Player &player); //Block arrest action for one turn
    };
}
#endif
