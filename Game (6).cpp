//vanunuraz@gmail.com
#include "Game.hpp"
#include "Player.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

namespace coup{
    //Constructor of game board
    Game::Game(): isGameOver(false), currentPlayer(""), skipTurn(false){
        //Clear the vectors
        players.clear();
        history.clear();
        cout<< "Game started"<< endl;
    }

    Game::~Game(){
        for(Player* p: players){
            delete p;
        }
        players.clear();
        //History delete out from the scope
    }

    void Game::checkIfGameOver(){
    int aliveCount= 0; //Flag to ensure if just 1 player alive
    for(Player* p: players){
        if(p->getIsAlive()){
            aliveCount++;
        }
    }
    if(aliveCount== 1){
        this->isGameOver= true;
    }
}

    Player* Game::createPlayer(const string &name, const string &role){
        if(isGameOver){
            throw runtime_error("Game is over, cannot add more players");
        }
        //Used constructors of different players
        if(role== "Governor"){
            return new Governor(*this, name);
        }
        else if (role == "Spy"){
            return new Spy(*this, name);
        }
        else if (role == "Baron"){
            return new Baron(*this, name);
        }
        else if (role == "General"){
            return new General(*this, name);
        }
        else if (role == "Judge"){
            return new Judge(*this, name);
        }
        else if (role == "Merchant"){
            return new Merchant(*this, name);
        }
        else{
            throw runtime_error("Invalid role");
        }
    }

    void Game::addPlayer(const vector<string> &playersNames){
        if(isGameOver){
            throw runtime_error("Game is over, cannot add more players");
        }
        vector<string> roles= {"Governor", "Spy", "Baron", "General", "Judge", "Merchant"};
        bool firstPlayer= players.empty(); //Ensure if empty
        for(const string &name: playersNames){
            string role= roles[rand() % roles.size()]; //Chose role by index
            Player* player= createPlayer(name, role); //Call createPlayer()
            players.push_back(player);
            player->setIsAlive(true);
            cout<< "Player "<< player->getName()<< " added to the game as role: "<< role<< endl;
        }
        if(firstPlayer && !players.empty()){ //This firs player that added so he the current player
            currentPlayer= players[0]->getName();
            cout<< "First player is: "<< currentPlayer<< endl;
        }
    }

    bool Game::isPlayerInGame(const Player* player) const{
        if(isGameOver){
            throw runtime_error("Game is over");
        }
        for(const Player* p: players){
            if(p== player && p->getIsAlive()){
                return true;
            }
        }
        return false;
    }

    void Game::checkCoupRequired(){
        for(Player* p: players){
            if(p->getName()== currentPlayer && p->getIsAlive()){ //Ensure its the current player
                if(p->getCoins()>= 10){
                    throw runtime_error("Player must perform a coup (has 10 or more coins).");
                }
                return;
            }
        }
        throw runtime_error("Current player not found");
    }

    //Functionality of start turn
    void Game::nextTurn(){
        //If the game has ended, we cannot proceed to the next turn
        if(isGameOver){
            throw runtime_error("Game is over, cannot continue");
        }
        //If the previous turn included a bribe, skip this turn once
        if(skipTurn){
            skipTurn = false; //Reset the skip flag
            cout << "Bribe used: skipping this turn" << endl;
            return;
        }
        //Find the pointer to the current player based on their name
        Player* currentPlayerPtr = nullptr;
        for (Player* p : players) {
            if (p->getName() == currentPlayer) {
                currentPlayerPtr = p;
                break;
            }
        }
        //If current player was not found, throw error
        if(currentPlayerPtr == nullptr){
            throw runtime_error("Current player not found");
        }
        //Enforce rule: if player has 10+ coins, they must perform a coup
        checkCoupRequired();
        //If current player is a Merchant, apply their passive ability and if he with 3, get 1
        Merchant* merchantPtr= dynamic_cast<Merchant*>(currentPlayerPtr);
        if(merchantPtr!= nullptr){
            merchantPtr->whileLeast3();
        }
        //Clear previous history actions made by the current player
        clearPrevHistory(currentPlayerPtr);
        //Find the index of the current player in the players list
        size_t index= 0;
        for(size_t i= 0; i< players.size(); ++i) {
            if(players[i]->getName()== currentPlayer){
                index= i;
                break;
            }
        }
        //Move to the next player (circularly)
        size_t nextIndex= (index + 1)% players.size();
        //Skip any players who are not alive
        while(!players[nextIndex]->getIsAlive()){
            nextIndex= (nextIndex + 1)% players.size();
        }
        //Set the next player as the current player
        currentPlayer= players[nextIndex]->getName();
        // Print whose turn it is now
        cout<< "Next turn: "<< currentPlayer<< endl;
    }

    vector<Player*> Game::getPlayers() const{
        return players;
    }

    string Game::getCurrentPlayer() const{
        return currentPlayer;
    }

    vector<string> Game::getPlayersNames() const{
        vector<string> names; //Create vector of names
        for(const Player* p: players){
            if(p->getIsAlive()){
                names.push_back(p->getName());
            }
        }
        return names;
    }

    bool Game::getIsGameOver() const{
        return isGameOver;
    }

    vector<HistoryAction> Game::getHistory() const{
        return history;
    }

    string Game::winner() const{
        if(!isGameOver){ //One player in the game
            throw runtime_error("Game is not over yet");
        }
        for(const Player* p: players){ //Find the player that alive
            if(p->getIsAlive()){
                return p->getName();
            }
        }
        throw runtime_error("No winner found");
    }

    bool Game::getSkipTurn() const{
        return skipTurn;
    }

    void Game::setSkipTurn(bool skipTurn){
        this->skipTurn= skipTurn;
    }

    void Game::setCurrentPlayer(const string &name){
    this->currentPlayer= name;
}

    void Game::addHistoryAction(const string &action, Player *playerBy, Player *playerOn, int coins){
        HistoryAction h;
        h.action= action;
        h.playerBy= playerBy;
        h.playerOn= playerOn;
        h.coins= coins;
        history.push_back(h); //Adding with all details
    }

    void Game::clearPrevHistory(Player *p){
        for(size_t i= 0; i< history.size(); ++i) {
            if(history[i].playerBy == p){
                history.erase(history.begin() + i);
                --i;
            }
        }
    }

}