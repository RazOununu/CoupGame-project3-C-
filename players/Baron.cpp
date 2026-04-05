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
        if(game.getCurrentPlayer() != name){
            throw std::runtime_error("It is not your turn");
        }
        if(coins>= 10){
            throw std::runtime_error("You must coup when you have 10 or more coins");
        }
        if(coins< 3) {
            throw std::runtime_error("Not enough coins to invest (requires 3)");
        }
        updateCoins(-3);
        updateCoins(6);
        }

    void Baron::getFromSanction(){
        if(!isAlive){
            throw std::runtime_error("Player is not alive");
        }
        if(this->underSanction){
            updateCoins(1);
        }
        else{
            throw std::runtime_error("Player is not sanctioned");
        }
    }

}
