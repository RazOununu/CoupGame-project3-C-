//vanunuraz@gmail.com
//The Baron class is a specialized role derived from the Player base class, It implements unique abilities such as investing
//coins and benefiting from sanctions.

#ifndef BARON_HPP
#define BARON_HPP
#include <string>
#include "Player.hpp"

namespace coup{
    class Baron: public Player{
    public:
        //Constructor
        Baron(Game &game, const std::string &name);
        //Role identifier
        std::string role() const override;
        //Special abilities
        void invest(); //Pay 3 coins and get 6 coins
        void getFromSanction(); //If sunction get 1 coin
    };
}
#endif
