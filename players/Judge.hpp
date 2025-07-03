//vanunuraz@gmail.com
//This class represents the Judge role, which inherits from the Player base class. The Judge can cancel bribes and penalize
//players who sanctioned them by transferring coins.
#ifndef JUDGE_HPP
#define JUDGE_HPP

#include <string>
#include "Player.hpp"

namespace coup{

    class Judge: public Player{
    public:
        //Constructor
        Judge(Game &game, const std::string &name);
        //Role identifier
        std::string role() const override;
        //Special abilities
        //Cancels the effect of a bribe e.g. revives a player or restores turn
        void undoBribe(Player &player);
        //If arrested, the player that did it pay 1 coins.
        void addToCashDesk();
    };
}
#endif 
