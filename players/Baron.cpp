//vanunuraz@gmail.com
#include "Baron.hpp"
#include "Player.hpp"
#include "Game.hpp"
#include <stdexcept> 

namespace coup{
    Baron::Baron(Game &game, const std::string &name): Player(game, name)
    {}

    std::string Baron::role() const{
        return "Baron";
    }
    //Special ability
    void Baron::invest(){
        if(!isAlive){
            throw std::runtime_error("Player is not alive");
        }
        if(coins>= 10){
            throw std::runtime_error("You must coup when you have 10 or more coins");
        }
        if(coins< 3) {
            throw std::runtime_error("Not enough coins to invest (requires 3)");
        }
        this->coins-= 3;
        this->coins+= 6;
        }

    void Baron::getFromSanction(){
        if(!isAlive){
            throw std::runtime_error("Player is not alive");
        }
        if(this->underSanction){
            this->coins+= 1;
        }
        else{
            throw std::runtime_error("Player is not sanctioned");
        }
    }

}
