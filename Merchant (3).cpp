//vanunuraz@gmail.com
#include "Merchant.hpp"
#include "HistoryAction.hpp"
#include "Game.hpp"
#include <stdexcept>

namespace coup{
    Merchant::Merchant(Game &game, const std::string &name): Player(game, name), isArrested(false)
    {}
    std::string Merchant::role() const{
        return "Merchant";
    }

    void Merchant::whileLeast3(){
        if(!isAlive){
            throw std::runtime_error("Merchant is not alive");
        }
        if(coins>= 3) {
            coins+= 1;
        }
    }

    void Merchant::arrested(){
        if(!isAlive){
            throw std::runtime_error("Merchant is not alive");
        }
        if(!isArrested) {
            throw std::runtime_error("Merchant is not flagged as arrested");
        }
        //Looking for the history for get the player who arrested the Merchant
        bool found= false;
        for(const HistoryAction &action: game.getHistory()){
            if(action.action== "Arrest" && action.playerOn== this){
                if(action.playerBy!= nullptr && action.playerBy->getIsAlive()){
                    action.playerBy->updateCoins(-1); //Arrestor loses 1 coin
                    this->updateCoins(1); //Merchant Get 1 coin
                    found= true;
                    break;
                }
            }
        }
        if (!found){
            throw std::runtime_error("No arrest action found for this merchant");
        }
        isArrested= false;
    }

}
