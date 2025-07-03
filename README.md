//vanunuraz@gmail.com
# Coup Game 
**Author:** Raz Vanunu  
**Email:** vanunuraz@gmail.com  
**Date:** July 2025  

## Overview
This C++ project is an implementation of the board game Coup, modeled as part of an academic assignment focusing on OOP concepts including inheritance (regular and virtual), polymorphism, and the Rule of Three.  
The project includes a fully interactive SFML-based GUI**, a test suite using doctest, and memory management conforming to Valgrind validation.
Players take on various roles with unique abilities and compete by collecting coins, performing actions, and eliminating opponents. The last surviving player wins.


## Core Concepts Implemented
 **Inheritance (Regular + Virtual):**  
  Each role class inherits from the base Player class. Virtual inheritance is used to support multiple derived roles safely in polymorphic contexts.
  
 **Rule of Three:**  
  All classes that manage dynamic memory define:
  - Copy constructor
  - Copy assignment operator
  - Destructor

 **Exception Handling:**  
  Every illegal move throws a meaningful runtime_error, as required.

 **Polymorphism:**  
  role() and ability methods are overridden in each derived class using override.

 **Modularity and Clean Structure:**  
  Each class is separated into .hpp/.cpp, grouped by gameManagement, players, gui, and tests.

 **Memory Safety:**  
  All dynamic memory for players is allocated using new and properly released in the Game destructor using delete. I ensured proper cleanup by overriding the destructor and calling delete for each allocated player. The project passes Valgrind with zero memory leaks.




## Folder Structure

PROJECT3_C++
├── gameManagement/
│ ├── Game.hpp / Game.cpp
│ └── HistoryAction.hpp
├── players/
│ ├── Player.hpp / Player.cpp
│ ├── Governor.hpp / Governor.cpp
│ ├── Spy.hpp / Spy.cpp
│ ├── Baron.hpp / Baron.cpp
│ ├── General.hpp / General.cpp
│ ├── Judge.hpp / Judge.cpp
│ └── Merchant.hpp / Merchant.cpp
├── gui/
│ ├── coupSFML.cpp
│ └── DejaVuSans.ttf
├── tests/
│ ├── test.cpp
│ └── doctest.h
├── Main.cpp
├── Makefile
└── README.md


## Roles and Unique Abilities

| Role         | Special Abilities                                                                                  |
|--------------|----------------------------------------------------------------------------------------------------|
| **Governor** | Takes 3 coins (instead of 2) via tax. Can undo tax from another player.                            |
| **Spy**      | Can inspect coin count of any player and block arrest once. Can bribe (skip another turn).         |
| **Baron**    | Can invest 3 coins to receive 6. If sanctioned, loses at most 1 coin.                              |
| **General**  | Can arrest (steal 1 coin). Can prevent coup (5 coins). Recovers 1 coin if arrested.                |
| **Judge**    | Can undo bribe (removes benefit but not refund). If sanctioned, attacker pays extra.               |
| **Merchant** | If has ≥3 coins at turn start, gets 1 bonus. If arrested, pays 2 to bank instead of 1 to attacker. |


## Game Rules Implemented
- Players can perform: gather, tax, bribe, arrest, sanction, coup.
- If a player has ≥10 coins, they must perform coup.
- Actions update coin balances and history accordingly.
- All blocking/undo mechanisms are implemented with timing constraints.
- Game manages player turns, history, active players, and winner.


## GUI Details
Implemented with SFML, the GUI shows:
- Dynamic status of all players (name, role, coins, alive).
- History log of actions.
- "Next" button to proceed turn by turn.
- Automatic simulation mode.
- Styled in black and white with readable layout.


## Unit Testing
Implemented with **doctest**:
  - All role abilities and interactions
  - Illegal moves 
  - Bribe/undo sequences
  - Sanctions and coin edge cases
  - Game over and winner logic
  - History tracking and blocking
  - GUI logic (partially tested via simulation mode)
All corner cases, reversals, and failure paths are tested.


## Makefile Commands
- `make Main`– compile and run main demo  
- `make test`– run all tests in tests/test.cpp using doctest  
- `make valgrind`– run valgrind memory leak checks  
- `make clean`– clean all build artifacts  
- `make GUI`– graphical simulation  
