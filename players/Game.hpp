//vanunuraz@gmail.com
//This class manages the core game logic and flow for the Coup game.
//The Game class is responsible for maintaining the list of active players, controlling turn order, tracking the current player,
//and managing the endgame condition. Also it enforces key game rules such as mandatory coup actions when a player holds 10 or
//more coins and handles special mechanics such as skipping a turn after a bribe. The class also tracks a history of actions
//performed by players in the current round, enabling certain roles to undo specific actions such as bribe or tax.
//Memory management is handled in the destructor by deleting all dynamically allocated Player instances.
//Responsibilities include:
//1. Adding players and assigning roles.
//2. Moving the game forward through player turns.
//3. Tracking and updating the current player.
//4. Enforcing rules related to actions, bribes, and coups.
//5. Managing game history and undo functionality.
//6. Determining when the game is over and declaring a winner.

#ifndef Game_hpp
#define Game_hpp
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "Player.hpp"
#include "HistoryAction.hpp"
using namespace std;

namespace coup{
    class Game{
    private:
        vector<Player*> players; //List of players in the game --> delete vector of pointers
        bool isGameOver; //Flag to determine if the game has ended
        vector<HistoryAction> history; //History of actions for undo functionality
        string currentPlayer; //The name of the player whose turn it is
        bool skipTurn; //Used for handling bribe mechanics
    
        void checkCoupRequired(); //Helper to enforce coup if player has 10+ coins

    public:
        Game();
       ~Game();
        void checkIfGameOver(); //1 player in the game
        Player* createPlayer(const string &name, const string &role); //Create and add player to the game
        void addPlayer(const vector<string>& playersNames); //Also for add players
        bool isPlayerInGame(const Player* player) const; //Check if a player is currently active in the game
        void nextTurn(); //Move to the next turn, skipping eliminated or sanctioned players

        //Getter
        vector<Player*> getPlayers() const; //Get all players (pointers)
        string getCurrentPlayer() const; //Get current player name
        vector<string> getPlayersNames() const; //Get list of active player names
        bool getIsGameOver() const; //Check if game is over
        vector<HistoryAction> getHistory() const; //Get the history of actions
        string winner() const; // Get the winner name, throw if game still active
        bool getSkipTurn() const; //Bool

        //Setters
        void setSkipTurn(bool skipTurn);
        void setCurrentPlayer(const string &name);

        //History
        void addHistoryAction(const string &action, Player *playerBy, Player *playerOn, int coins); //Add a new action to history for potential undo
        void clearPrevHistory(Player *p); //Clear previous actions related to a player because we save just one round during the game
    };
}
#endif
