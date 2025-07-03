//vanunuraz@gmail.com
#include "Judge.hpp"
#include "Game.hpp"
#include "HistoryAction.hpp"
#include <stdexcept>
#include <algorithm>

namespace coup{
    Judge::Judge(Game &game, const std::string &name): Player(game, name) {}

    std::string Judge::role() const{
        return "Judge";
    }

    void Judge::undoBribe(Player &player){
        if(!isAlive){
            throw std::runtime_error("Judge is not alive");
        }
        const std::vector<HistoryAction>& history= game.getHistory();
        //Find the last action performed by the specified player
        auto it= std::find_if(history.rbegin(), history.rend(), [&player](const HistoryAction& action){
            return action.playerBy== &player;});
        if(it== history.rend()){
            throw std::runtime_error("No action found for player");
        }
        //If the last action was not a bribe, cannot undo it
        if(it->action!= "bribe"){
            throw std::runtime_error("Last action was not bribe, cannot undo");
        }
        //Cancel the effect of the bribe 
        game.setSkipTurn(false);
        game.addHistoryAction("undo_bribe", this, &player, 0);
    }

    void Judge::addToCashDesk(){
    if(!isAlive){
        throw std::runtime_error("Judge is not alive");
    }
    if(!underSanction){
        throw std::runtime_error("Judge is not sanctioned");
    }
    //Loop over history to find a player who sanctioned the Judge
    for(const HistoryAction& action: game.getHistory()){
        if(action.action== "sanction" && action.playerOn == this){
            if(action.playerBy!= nullptr && action.playerBy->getIsAlive()){
                action.playerBy->updateCoins(-1); //Sanctioning player loses 1 coin
                this->updateCoins(1); //Judge gains 1 coin
                break; //Only process the first relevant sanction
            }
        }
    }
    this->clearSanction();//Optionally clear the sanction state
}
}
