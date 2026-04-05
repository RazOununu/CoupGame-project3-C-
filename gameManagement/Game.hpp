//vanunuraz@gmail.com
//Game is responsible for managing game flow: turn order, player state, action history, and enforcing game rules (e.g. mandatory coup at 10+ coins, bribe skip mechanics).

#ifndef Game_hpp
#define Game_hpp
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "Player.hpp"
#include "HistoryAction.hpp"

namespace coup{
    class Game{
    private:
        std::vector<Player*> players; //List of players in the game --> delete vector of pointers
        std::vector<Player*> ownedPlayers; //Heap-allocated players created via createPlayer(), deleted in destructor
        bool isGameOver; //Flag to determine if the game has ended
        std::vector<HistoryAction> history; //History of actions for undo functionality
        std::string currentPlayer; //The name of the player whose turn it is
        bool skipTurn; //Used for handling bribe mechanics

        void checkCoupRequired(); //Helper to enforce coup if player has 10+ coins

    public:
        Game();
       ~Game();
        //Copying is disabled: Game owns heap-allocated Player instances and copying would produce a shallow copy with shared pointers, causing double deletion.
        //A live game session has no meaningful copy semantic.
        Game(const Game&) = delete;
        Game& operator=(const Game&) = delete;
        void checkIfGameOver(); //1 player in the game
        Player* createPlayer(const std::string &name, const std::string &role); //Create and add player to the game
        void addPlayer(const std::vector<std::string>& playersNames); //Also for add players
        bool isPlayerInGame(const Player* player) const; //Check if a player is currently active in the game
        void nextTurn(); //Move to the next turn, skipping eliminated or sanctioned players

        //Getter
        std::vector<Player*> getPlayers() const; //Get all players (pointers)
        std::string getCurrentPlayer() const; //Get current player name
        std::vector<std::string> getPlayersNames() const; //Get list of active player names
        bool getIsGameOver() const; //Check if game is over
        std::vector<HistoryAction> getHistory() const; //Get the history of actions
        std::string winner() const; // Get the winner name, throw if game still active
        bool getSkipTurn() const; //Bool

        //Setters
        void setSkipTurn(bool skipTurn);
        void setCurrentPlayer(const std::string &name);
        void registerPlayer(Player* player); //Registers an externally constructed player and initializes turn order

        //History
        void addHistoryAction(const std::string &action, Player *playerBy, Player *playerOn, int coins); //Add a new action to history for potential undo
        void clearPrevHistory(Player *p); //Clear previous actions related to a player because we save just one round during the game
    };
}
#endif
