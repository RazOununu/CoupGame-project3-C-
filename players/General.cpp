//vanunuraz@gmail.com
#include "General.hpp"
#include "Player.hpp"
#include <stdexcept>

namespace coup{

    //Initializes a General player with default arrest status
    General::General(Game& game, const std::string& name): Player(game, name), isArrested(false)
    {}

    std::string General::role() const{
        return "General";
    }

    //Special ability
    //Revive a player who was eliminated by coup. Costs 5 coins and only allowed if the target player is currently dead
    void General::saveFromCoup(Player& player){
        if(!isAlive){
            throw std::runtime_error("General is not in the game");
        }
        if(player.getIsAlive()){
            throw std::runtime_error("Target player is already alive");
        }
        if(coins< 5){
            throw std::runtime_error("Not enough coins to save from coup");
        }
        if(player.getCoins()>= 10){
            throw std::runtime_error("Player must coup instead of being revived");
        }
        coins-= 5;
        player.setIsAlive(true);
    }

    //If the general was arrested last round, gain 1 coin and clear arrest status
    void General::getFromArrested(){
        if(!isAlive){
            throw std::runtime_error("General is not in the game");
        }
        if(isArrested){
            coins+= 1;
            isArrested= false;
        }
        else{
            throw std::runtime_error("General was not arrested");
        }
    }
    
    //Set and get new member
    void General::setArrested(bool status){
        isArrested= status;
    }
    bool General::getArrested() const{
        return isArrested;
    }

}
