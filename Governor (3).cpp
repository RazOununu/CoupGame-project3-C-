//vanunuraz@gmail.com
#include "Governor.hpp"
#include "Game.hpp"
#include <stdexcept>

namespace coup{
    Governor::Governor(Game& game, const std::string& name) :Player(game, name)
    {}

    std::string Governor::role() const{
        return "Governor";
    }

    void Governor::tax(){ //Not copy the player implement because different implement
        if (!isAlive){
            throw std::runtime_error("Governor is not alive");
        }
        if(coins>= 10){
            throw std::runtime_error("Must perform coup with 10 or more coins");
        }
        if(underSanction){
            underSanction= false;
            throw std::runtime_error("Governor is sanctioned and cannot use tax");
        }
        coins+= 3;
        game.addHistoryAction("tax", this, nullptr, 3); 
    }

    //Attempt to undo a tax action performed by another player
    void Governor::undoTax(Player& player){
        // Check if the Governor is alive
        if(!isAlive) {
            throw std::runtime_error("Governor is not alive");
        }
        //Check if the target player is alive
        if(!player.getIsAlive()){
            throw std::runtime_error("Target player is not alive");
        }

        //Retrieve the action history from the game
        const auto& history= game.getHistory();
        bool found= false;
        //Iterate backward through the history to find the last action of the target player
        for(auto it= history.rbegin(); it!= history.rend(); ++it){
            if(it->playerBy== &player){
                if(it->action== "tax"){
                    found= true; //Found the last action and it was tax
                    break;
                }
                else{ //Last action was not tax, cannot undo
                    break;
                }
            }
        }
        // If tax was not the last action, throw an error
        if(!found){
            throw std::runtime_error("Cannot undo tax: last action was not tax");
        }
        player.updateCoins(-2); //Reverse the effect of the tax  and andsubtract 2 coins from the player
        game.addHistoryAction("undoTax", this, &player, -2); //Keep the undo action in the history
    }


}
