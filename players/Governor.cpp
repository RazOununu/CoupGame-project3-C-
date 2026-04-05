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
            throw std::runtime_error("You must perform a coup when you have 10 coins");
        }
        if(underSanction){
            underSanction= false;
            throw std::runtime_error("Governor is sanctioned and cannot use tax");
        }
        updateCoins(3);
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
        auto taxEntry= history.rend();
        //Iterate backward through the history to find the last action of the target player
        for(auto it= history.rbegin(); it!= history.rend(); ++it){
            if(it->playerBy== &player){
                if(it->action== "tax"){
                    taxEntry= it; //Save iterator to read coins value later
                }
                break; //Stop after the last action of this player regardless
            }
        }
        // If tax was not the last action, throw an error
        if(taxEntry== history.rend()){
            throw std::runtime_error("Cannot undo tax: last action was not tax");
        }
        int taxAmount= taxEntry->coins; //Use the actual amount recorded in history
        player.updateCoins(-taxAmount); //Reverse the exact coins that were gained
        game.addHistoryAction("undoTax", this, &player, -taxAmount); //Keep the undo action in the history
    }

}
