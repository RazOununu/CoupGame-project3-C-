//vanunuraz@gmail.com
//This file demonstrates a simulation of the Coup game, using all six roles: Governor, Spy, Baron, General, Judge, and Merchant.
//The simulation progresses through turns and includes various actions: gather, tax, undo, invest, coup, etc.
//Differences from the original demo version:
// 1. Uses specific getters and function names, such as getPlayersNames() and getCoins() instead of players() and coins().
// 2. Uses specific role undo methods like undoTax() and undoBribe() instead of a generic undo() method.
// 3. Includes explicit output messages and exception handling to illustrate game logic, turn order and validation of legal
// or illegal actions(e.g. player trying to play out of his turn).
// 4. Removed using namespace std and replaced all standard library references with std:: for clarity and safety.
#include "Player.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include "Game.hpp"
#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace coup;

int main() {
    Game game_1;

    //Create one player for each role using the Game instance and constructors for each role
    Governor governor(game_1, "Moshe");
    Spy spy(game_1, "Yossi");
    Baron baron(game_1, "Meirav");
    General general(game_1, "Reut");
    Judge judge(game_1, "Gilad");

    //Display all current player names
    std::vector<std::string> players = game_1.getPlayersNames();
    for (const std::string &name : players) {
        std::cout << name << std::endl;
    }

    //Show whose turn at the beginning of the game
    std::cout << "First turn: " << game_1.getCurrentPlayer() << std::endl;

    //Each player performs a gather action
    governor.gather();
    spy.gather();
    baron.gather();
    general.gather();
    judge.gather();

    //Trying to play out of turn should raise an exception
    try {
        spy.gather(); //It's not spy's turn
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n'; //Display error message
    }

    //Governor turns and chooses gather action
    governor.gather();

    //Spy make tax in his turn
    spy.tax();

    //Judge tries to undo Governor’s action, not allowed by the rules
    try {
        judge.undoBribe(governor);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n'; // Expected error: Judge cannot undo tax
    }

    //Display current coin count for both players
    std::cout << "Governor coins: " << governor.getCoins() << std::endl; //2
    std::cout << "Spy coins: " << spy.getCoins() << std::endl; //3

    //Governor undoes Spy's tax, legal action for Governor
    governor.undoTax(spy);
    std::cout << "Spy coins after undo: " << spy.getCoins() << std::endl; //1

    //Baron taxes and get 2 coins, general and judge gather and get 1 coin
    baron.tax();
    general.gather();
    judge.gather();

    //Governor taxes and get 3 coins
    governor.tax();
    spy.gather();

    //Baron uses invest 3 and get 6 coins
    baron.invest();
    general.gather();
    judge.gather();

    //Display Baron's coins after invest
    std::cout << "Baron coins: " << baron.getCoins() << std::endl; // Expected: 6

    //Rounds of gathering and taxing
    governor.tax();
    spy.gather();
    baron.gather(); //+1 and should now have 7 coins
    general.gather();
    judge.gather();

    governor.tax(); //Have 10 coins now
    spy.gather();

    //Baron performs a coup against Governor costs 7 coins
    std::cout << "Baron coins before coup: " << baron.getCoins() << std::endl; // Expected: 7+
    baron.coup(governor);

    //Gathers by General and Judge
    general.gather();
    judge.gather();

    //Display players now in the game
    players = game_1.getPlayersNames();
    std::cout << "Players after coup:" << std::endl;
    for (const std::string &name : players) {
        std::cout << name << std::endl; //Governor should not appear now
    }

    return 0;
}
