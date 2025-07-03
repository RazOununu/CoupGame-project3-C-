//vanunuraz@gmail.com
#include "Player.hpp"
#include "Game.hpp"
#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

namespace coup{
    //Constructor initializes the player's attributes
    Player::Player(Game &game, const string &name) :name(name), game(game), coins(0), isAlive(true), underSanction(false),
    lastArrested(""), isBlockedFromArrest(false) {}

    //Getters 
    //Returns the name of the player
    string Player::getName() const{
        return name;
    }

    //Returns whether the player is still alive
    bool Player::getIsAlive() const{
        return isAlive;
    }

    //Returns the player's current number of coins
    int Player::getCoins() const{
        return coins;
    }

    //Returns whether the player is currently under sanction
    bool Player::isUnderSanction() const {
        return underSanction;
    }

    // Returns whether the player is currently protected from arrest
    bool Player::getBlockedFromArrest() const{
        return isBlockedFromArrest;
    }

    //Returns the name of the last player this player arrested
    string Player::getLastArrested() const{
        return lastArrested;
    }

    //Setters
    //Sets the player's alive status, checks if game should end
    void Player::setIsAlive(bool alive){
        this->isAlive= alive;
        if(!alive){
            game.checkIfGameOver(); //If only one player is left alive, the game ends
        }
    }

    //Enables or disables arrest protection for this player
    void Player::setBlockedFromArrest(bool blockedArrest){
        this->isBlockedFromArrest= blockedArrest;
    }

    //Updates the player's coin count 
    void Player::updateCoins(int delta){
        this->coins+= delta;
    }

    //Clears any active sanction from the player
    void Player::clearSanction(){
        this->underSanction= false;
    }

    //Actions

    //Cannot be used if sanctioned or if player has 10+ coins. +1
    void Player::gather(){
        if(!isAlive){
            throw runtime_error("Player is not alive");
        }    
        if(coins>= 10){
            std::cout<< "Warning: You must perform a coup when you have 10 coins"<< std::endl;
            return;
        }
        if(underSanction){
            underSanction = false;  //Sanction is cleared and action fails
            throw runtime_error("Player is sanctioned from gathering");
        }
        coins+= 1;
        game.addHistoryAction("gather", this, nullptr, 1);
    }

    //Cannot be used if sanctioned or if player has 10+ coins. +2
    void Player::tax(){
        if(!isAlive){
            throw runtime_error("Player is not alive");
        }    
        if(coins>= 10){
            throw runtime_error("You must perform a coup when you have 10 coins");
        }    
        if(underSanction){
            underSanction= false; //Clear
            throw runtime_error("Player is sanctioned from tax");
        }
        coins+= 2;
        game.addHistoryAction("tax", this, nullptr, 2);
    }

    //Pay 4 coins to use bribe ability
    void Player::bribe(){
        if(!isAlive){
            throw runtime_error("Player is not alive");
        }    
        if(coins>= 10){
            throw runtime_error("You must perform a coup when you have 10 coins");
        }    
        if(coins< 4){
            throw runtime_error("Not enough coins to bribe");
        }
        coins-= 4;
        game.setSkipTurn(true); //Set flag to skip the next turn that not skipTurn 
        game.addHistoryAction("bribe", this, nullptr, 4);
    }

    //Arrest another player and get 1 coin from target loses 1. Not possible to arrest the same player twice
    void Player::arrest(Player &target){
        if(!isAlive){
            throw runtime_error("Player is not alive");
        }    
        if(!target.getIsAlive()){
            throw runtime_error("Target is not alive");
        }    
        if(coins>= 10){
            throw runtime_error("You must perform a coup when you have 10 coins");
        }    
        if(target.isBlockedFromArrest){
            throw runtime_error("Target is currently protected from arrest");
        }
        if(lastArrested== target.getName()) {
            throw runtime_error("You cannot arrest the same player in consecutive turns");
        }
        coins+= 1;
        target.coins-= 1;
        lastArrested= target.getName(); //Update this member
        game.addHistoryAction("arrest", this, &target, 1);
    }

    //Sanction another player and prevents them from using gather or tax next turn. Need pay 3 coins.
    void Player::sanction(Player &target){
        if(!isAlive){
            throw runtime_error("Player is not alive");
        }    
        if(!target.getIsAlive()){
            throw runtime_error("Target is not alive");
        }    
        if(coins>= 10){
            throw runtime_error("You must perform a coup when you have 10 coins");
        }    
        if(coins< 3){
            throw runtime_error("Not enough coins to sanction (requires 3)");
        }    
        coins-= 3;
        target.underSanction= true; //Update this member
        game.addHistoryAction("sanction", this, &target, 3);
    }

    //Eliminates a player from the game. Costs 7 coins.
    void Player::coup(Player &target){
        if(!isAlive){
            throw runtime_error("Player is not alive");
        }    
        if(!target.getIsAlive()){
            throw runtime_error("Target is not alive");
        }    
        if(coins< 7){
            throw runtime_error("Not enough coins to perform coup");
        }    
        coins-= 7;
        target.setIsAlive(false); //Update this member
        game.addHistoryAction("coup", this, &target, 7);
    }

}
