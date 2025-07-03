//vanunuraz@gmail.com
//The Player class serves as an abstract base class representing a generic player in the Coup game. It defines common attributes
//(such as name, coin count, game state) and basic actions available to all players.
//This class is designed for inheritance specific roles like, derive from Player and override its virtual methods
//to provide role specific behavior.
//The presence of a pure virtual function role() makes Player an abstract class, ensuring it cannot be instantiated directly.
//The virtual destructor ensures proper cleanup when derived objects are deleted through base class pointers.

#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <iostream>
#include <string>
using namespace std;

namespace coup{
    class Game; //Forward declaration without including all this class implemention
    class Player{
    protected:
        string name;
        Game& game;
        int coins;
        bool isAlive;
        bool underSanction; //Can't gather/tax
        string lastArrested; //For the rule that not arrested the same player twice.
        bool isBlockedFromArrest; //According to lastArrested

    public:
        Player(Game& game, const std::string& name);
        virtual ~Player(){} //Virtual destructor to ensure proper cleanup of derived classes through base class pointer
        //Currently empty, but allows extension later if needed

        //Getters
        string getName() const;
        bool getIsAlive() const;
        int getCoins() const;
        bool isUnderSanction() const;
        bool getBlockedFromArrest() const;
        string getLastArrested() const;

        //Setters
        void setIsAlive(bool alive);
        void updateCoins(int delta);
        void setBlockedFromArrest(bool blocked);
        void clearSanction();

        //Actions, virtual for inheritance by another roles.
        virtual void gather(); //Collect 1 coin without any cost
        virtual void tax(); //Collect 2 coins without cost
        virtual void bribe(); //Pay 4 coins to perform an action, can be undone under certain conditions.
        virtual void arrest(Player& player); //Arrest another player, cannot target the same player twice in a row.
        virtual void sanction(Player& player); //Prevent another player from performing economic actions (gather, tax) until next turn, costs 3 coins.
        virtual void coup(Player& player); //Pay 7 coins to eliminate another player, must be used if player has 10 or more coins.

        //Abstract method
        virtual string role() const = 0;
    };
}

#endif
