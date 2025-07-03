//vanunuraz@gmail.com
//The Merchant class extends the Player class and represents a role focused on economic growth. It has unique abilities to
//generate additional coins when holding at least 3, and reacts differently to arrests.

#ifndef MERCHANT_HPP
#define MERCHANT_HPP

#include <string>
#include "Player.hpp"

namespace coup{
    class Merchant: public Player{
    private:
        bool isArrested; //Flag indicating if the merchant was arrested

    public:
        //Constructor
        Merchant(Game &game, const std::string &name);
        //Role identifier
        std::string role() const override;
        //Special abilities
        void whileLeast3(); //Get 1 coin if has at least 3 coins
        void arrested(); //Called when Merchant is arrested, insted -1, -2 to the cash.
    };
}
#endif
