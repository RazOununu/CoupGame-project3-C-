//vanunuraz@gmail.com
#include "Merchant.hpp"
#include "Game.hpp"
#include <stdexcept>

namespace coup{
    Merchant::Merchant(Game &game, const std::string &name): Player(game, name)
    {}
    std::string Merchant::role() const{
        return "Merchant";
    }

    void Merchant::onTurnStart(){
        whileLeast3();
    }

    void Merchant::whileLeast3(){
        if(!isAlive){
            throw std::runtime_error("Merchant is not alive");
        }
        if(coins>= 3) {
            updateCoins(1);
        }
    }

}
