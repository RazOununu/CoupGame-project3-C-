//vanunuraz@gmail.com
//This file provides a graphical simulation of the Coup game using the SFML library. It fulfills the assignment's GUI requirement
//by visually showing automatic simulation that presents the flow of the game using colors, player states, and actions
//step by step.
//The simulation automatically progresses through player turns, executing legal actions according to their roles and until one
//player remains. Also, showing important state changes, such as coins, actions and whether players are still alive.
//This implementation uses SFML (=Simple and Fast Multimedia Library). No user interaction is required, the GUI shows a dynamic
//and autonomous** visual demo of a complete game session.
//Because we dont learn how to use SFML in this course I try to learn it by myself and use AI to help me with the syntax.

//This file includes: drawScreen(), addStep(), main that implement short game from "Game start" until we get 1 winner, loop for
//closing the window when the user close it.
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include "Game.hpp"
#include "Player.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"

using namespace coup;

//Visual screen: left -> history, right -> current player status
void drawScreen(sf::RenderWindow& window, sf::Font& font,
                const vector<string>& visibleHistory,
                const vector<Player*>& players){
    window.clear(); //Clear the window before drawing

    //Display action history on the left
    int y = 20;
    for (const string& line : visibleHistory) {
        sf::Text text;
        text.setFont(font);
        text.setString(line);
        text.setCharacterSize(22);
        text.setFillColor(sf::Color::White);
        text.setPosition(20, y);
        window.draw(text);
        y += 28;
    }

    //Display player statuses on the right
    y = 20;
    for (const auto& p : players) {
        sf::Text info;
        info.setFont(font);
        info.setCharacterSize(20);
        string status = p->getName() + " (" + p->role() + ") - " + to_string(p->getCoins()) + " coins";
        if (!p->getIsAlive()) {
            status += " [OUT]";
            info.setFillColor(sf::Color(150, 150, 150)); //Grey if not alive
        } else {
            info.setFillColor(sf::Color::Green); //Green if alive
        }
        info.setString(status);
        info.setPosition(600, y);
        window.draw(info);
        y += 30;
    }

    window.display(); //Show the updated screen
}

//Add a new step to the history and update the screen, status of players
void addStep(sf::RenderWindow& window, sf::Font& font,
             vector<string>& visibleHistory,
             const vector<Player*>& players,
             const string& step) {
    visibleHistory.push_back(step); //acsses to new step
    drawScreen(window, font, visibleHistory, players); //Call to update the screen
    sf::sleep(sf::seconds(2)); //Wait 2 seconds between steps
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 600), "Coup Game - SFML"); //Open a window with size 1000x600
    //Load font for displaying text
    sf::Font font;
    if (!font.loadFromFile("gui/DejaVuSans.ttf")) { //Path to the font file
        cerr << "Font load failed\n";
        return 1;
    }
    //Initialize game and players and using my implementation of the Coup game
    Game game;
    vector<string> names= {"Alice", "Bob", "Carol", "David", "Eve", "Frank"};
    game.addPlayer(names);
    vector<Player*> players= game.getPlayers(); //Get the list of players

    vector<string> visibleHistory; //Vector ro store history of actions
    addStep(window, font, visibleHistory, players, "Game started"); //First step in the history: "Game started"

    size_t currentIndex= 0; //Indicator for the current player turn
    bool gameEnded= false; //Flag to indicate if the game has ended
    //Game loop simulate for each player turn until the game ends
    while(!gameEnded && window.isOpen()){
        Player* currentPlayer= players.at(currentIndex % players.size()); //Get the current player based on the index
        currentIndex++;
        if(!currentPlayer->getIsAlive()){ //Check if the current player is alive and if not, skip turn
            continue; //Skip turn if player is not alive
        } 
        
        //Simulate the player's turn:
        try{
            //If player has 10+ coins, must perform a coup as the game rule
            if(currentPlayer->getCoins()>= 10){
                //Find a target to perform coup that is not the current player and is alive
                for(auto& target: players){ 
                    if(target!=currentPlayer && target->getIsAlive()){
                        currentPlayer->coup(*target);
                        target->setIsAlive(false); //coup action kills the target player 
                        addStep(window, font, visibleHistory, players, currentPlayer->getName()+ " performed coup on "+
                        target->getName()); //Add this step to history
                        break;
                    }
                }
            }
            //Action behavior for each player role, choose specific action just for simulation purposes
            //Governor gets 3 coins instead of 2 at tax
            if(currentPlayer->role()== "Governor"){
                currentPlayer->tax(); //Governor performs his tax action
                addStep(window, font, visibleHistory, players, currentPlayer->getName()+ " used tax (3 coins)");//Add this step to history
            }

            //Spy gathers 1 coin
            else if(currentPlayer->role()== "Spy"){
                currentPlayer->gather();
                addStep(window, font, visibleHistory, players, currentPlayer->getName()+ " gathered 1 coin");//Add this step to history
            }

            //If player is a Baron, it can perform a bribe if it has 4 coins
            else if(currentPlayer->role()== "Baron"){
                if(currentPlayer->getCoins()>= 4){ //Check if the player has enough coins to perform a bribe
                    currentPlayer->bribe();
                    addStep(window, font, visibleHistory, players, currentPlayer->getName()+ " performed bribe");//Add this step to history
                }
            }

            //General can arrest another player
            else if(currentPlayer->role()== "General"){
                //Find a target to arrest that is not the current player and is alive
                for(auto& target: players){ 
                    if(target!= currentPlayer && target->getIsAlive()){
                        currentPlayer->arrest(*target);
                        target->setIsAlive(false); //arrest action kills the target player
                        addStep(window, font, visibleHistory, players, currentPlayer->getName()+ " arrested "+ target->getName());
                        break;
                    }
                }
            }

            //Judge can sanction another player if it has 3 coins
            else if(currentPlayer->role()== "Judge"){
                if(currentPlayer->getCoins()>= 3){ //Check if the player has enough coins to sanction
                    //Find a target to sanction, the first alive player that is not the current player
                    for(auto& target: players){ 
                        if(target!= currentPlayer&& target->getIsAlive()){
                            currentPlayer->sanction(*target);
                            addStep(window, font, visibleHistory, players, currentPlayer->getName()+ " sanctioned "+
                            target->getName());//Add this step to history
                            break;
                        }
                    }
                }
            }

            //All another situations, the player can only gather 1 coin in this simulation for GUI purposes
            else{
                currentPlayer->gather();
                addStep(window, font, visibleHistory, players, currentPlayer->getName()+ " gathered 1 coin");//Add this step to history
            }
        }
        //Catch any exceptions that may occur during the player's turn because of illegal actions
        catch(exception& e){
            addStep(window, font, visibleHistory, players, string("Warning: ")+ e.what());//Add the exception message to history
        }

        //Check win condition- only one alive player left
        int aliveCount= 0; //Flag to count the number of players that still alive.
        Player* winner= nullptr; 
        for(auto* p: players){
            if(p->getIsAlive()){
                aliveCount++;
                winner= p; //Keep the last alive player
            }
        }
        if(aliveCount== 1 && winner!= nullptr){ //This is the winner
            addStep(window, font, visibleHistory, players, winner->getName()+ " wins the game!"); //Declare the winner
            gameEnded = true; //Set game ended flag because only one player is alive
        }
    }

    //Final loop to keep the window open until user closes it.
    sf::Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();
    }
    return 0;
}
