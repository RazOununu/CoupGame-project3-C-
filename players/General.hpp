//vanunuraz@gmail.com
//This class defines the "General" role in the Coup game. The General can revive eliminated players and gains a bonus after
//being arrested.

#ifndef GENERAL_HPP
#define GENERAL_HPP

#include <string>
#include "Player.hpp"

namespace coup{
    class General: public Player{
    private:
        bool isArrested= false; //Flag to indicate if the General was arrested recently
    public:
        //Constructor
        General(Game& game, const std::string& name);
        //Role identifier
        std::string role() const override;
        //Special ability
        //Pay 5 coins to revive a player who was eliminated
        void saveFromCoup(Player& player);
        //Get coin if General was arrested
        void getFromArrested();

        //Allow external access for setting arrested status
        void setArrested(bool status);
        bool getArrested() const;
    };

}

#endif
