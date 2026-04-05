//vanunuraz@gmail.com
#include "PlayerFactory.hpp"
#include "Game.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include <stdexcept>

namespace coup{
    Player* createPlayer(Game& game, const std::string& name, const std::string& role){
        if(role== "Governor"){
            return new Governor(game, name);
        }
        else if(role== "Spy"){
            return new Spy(game, name);
        }
        else if(role== "Baron"){
            return new Baron(game, name);
        }
        else if(role== "General"){
            return new General(game, name);
        }
        else if(role== "Judge"){
            return new Judge(game, name);
        }
        else if(role== "Merchant"){
            return new Merchant(game, name);
        }
        else{
            throw std::runtime_error("Invalid role");
        }
    }
}