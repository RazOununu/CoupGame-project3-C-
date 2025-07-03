//vanunuraz@gmail.com
#include "Spy.hpp"
#include "Player.hpp"

namespace coup{
    Spy::Spy(Game &game, const string &name): Player(game, name)
    {}

    string Spy::role() const{
        return "Spy";
    }

    //Allows the Spy to view the number of coins another player has
    int Spy::watchCoins(Player &player){
        if(!this->isAlive){
            throw runtime_error("Spy is not alive");
        }
        if(!player.getIsAlive()) {
            throw runtime_error("The player is not in the game");
        }
        return player.getCoins(); //Return the target player's coins
    }

    void Spy::blockArrest(Player &player){
        if(!this->isAlive){
            throw runtime_error("Spy is not alive");
        }
        if(!player.getIsAlive()){
            throw runtime_error("The player is not in the game");
        }
        if(coins>= 10){
            throw runtime_error("You must implement coup, you have 10 coins");
        }
        player.setBlockedFromArrest(true); //Mark the player as protected from arrest
    }

}
