# Coup Game
**Author:** Raz Vanunu  
**Email:** vanunuraz@gmail.com  
**Date:** July 2025  

## Overview
This C++ project is an implementation of the board game Coup, modeled as part of an academic assignment focusing on OOP concepts including inheritance (regular and virtual), polymorphism, and the Rule of Three.  
The project includes a fully interactive SFML-based GUI, a test suite using doctest, and memory management conforming to Valgrind validation.
Players take on various roles with unique abilities and compete by collecting coins, performing actions, and eliminating opponents. The last surviving player wins.


## Core Concepts Implemented
 **Inheritance (Regular + Virtual):**  
  Each role class inherits from the base Player class. Virtual inheritance is used to support multiple derived roles safely in polymorphic contexts.
  
 **Rule of Three / Copy Safety:**
  `Game` owns all dynamically allocated `Player` instances and frees them in its destructor.
  Copying a live game session is intentionally disabled the copy constructor and copy assignment operator are declared `= delete` because a shallow copy would share raw pointers and cause double deletion.
  Role classes do not own dynamic memory and therefore do not require explicit copy operations.

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
│ ├── PlayerFactory.hpp / PlayerFactory.cpp
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

## Code Improvements and Bug Fixes (April 2026)
The following bugs and design issues were identified and fixed after scanning the project by CloudCode:

### Bug Fixes
| # | Issue | Fix |
|---|---|---|
| 1 | **Merchant arrest rule non-functional** — `arrest()` always applied standard transfer; Merchant-specific code was dead and had a case mismatch bug | Applied Merchant logic directly in `Player::arrest()` via `dynamic_cast`; removed dead `arrested()` method |
| 2 | **`Governor::undoTax()` hardcoded −2** — Governor's tax gives 3 coins, but undo only reversed 2 | `undoTax()` now reads the actual value from the history entry |
| 3 | **`gather()` silent at 10 coins** — printed a warning and returned instead of throwing | `gather()` now throws the same `runtime_error` as `tax()` |
| 4 | **Merchant bonus applied at wrong time** — `whileLeast3()` fired on outgoing player, not incoming | Moved the call to after `currentPlayer` is updated in `nextTurn()` |
| 5 | **No turn enforcement** — any player could act at any time | Added `getCurrentPlayer() != name` check to all 7 turn-based actions; blocking/undo methods remain exempt |

### OOP Design Improvements
| # | Issue | Fix |
|---|---|---|
| 6 | **`dynamic_cast<Merchant*>` inside `Game`** — role-specific logic in the game manager | Added `virtual onTurnStart()` hook to `Player`; `Merchant` overrides it |
| 7 | **`Game.cpp` included all 6 role headers** — tight coupling via `createPlayer()` factory | Extracted factory to `PlayerFactory.hpp/.cpp`; `Game` delegates to it |

### Memory and Safety
| # | Issue | Fix |
|---|---|---|
| 8 | **`Game` had no copy constructor or assignment operator** — shallow copy would cause double deletion | Added `= delete` for both in `Game.hpp` |
| 9 | **`HistoryAction` raw pointer lifetime undocumented** | Added comment to `HistoryAction.hpp` making the lifetime contract explicit |
| 10 | **`~Game()` deleted stack players** — `delete` on stack pointers caused segfault | Added `ownedPlayers` vector; only heap players (via `createPlayer()`) are deleted; stack-registered players are not |

### Test Suite
| # | Change |
|---|---|
| 11 | All existing tests updated to call `game.setCurrentPlayer()` before turn-based actions |
| 12 | New tests added for: turn enforcement, Governor 3-coin tax, `undoTax` exact reversal, Merchant arrest coins, `saveFromCoup`, `getFromArrested`, 10-coin rule, Baron invest edge case, Judge behavior, `winner()` pre-game throw |

### Main.cpp and Turn Management (April 2026)
| # | Issue | Fix |
|---|---|---|
| 13 | **`Main.cpp` players not registered in `Game`** — `nextTurn()` threw `"Current player not found"` | Added `Game::registerPlayer(Player*)`; `Main.cpp` calls it once per player after construction |
| 14 | **`Main.cpp` missing turn advancement** — turns never advanced | Added `game_1.nextTurn()` after each regular action; undo/reaction calls have no `nextTurn()` |
| 15 | **History cleared too early** — `clearPrevHistory` on outgoing player closed the reaction window immediately | Moved to incoming player; history survives a full round per assignment rule |
