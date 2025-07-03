//vanunuraz@gmail.com
//Governor class is a special role that can collect extra income and undo tax actions of other players. Inherits from Player and
//overrides the tax behavior with additional abilities.

#ifndef GOVERNOR_HPP
#define GOVERNOR_HPP

#include "Player.hpp"
#include <string>

namespace coup{

    class Governor: public Player{
    public:
        //Constructor
        Governor(Game& game, const std::string& name);

        //Returns role name
        std::string role() const override;

        //Special abilities
        //Get 3 coins instead of 2
        void tax() override;

        //Can undo tax action of another player and remove back 2 coins.
        void undoTax(Player& player);
    };

}

#endif
