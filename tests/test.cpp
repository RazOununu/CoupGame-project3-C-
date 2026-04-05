//vanunuraz@gmail.com
// Comprehensive tests for Coup game and all role actions using doctest framework.
// Turn-based actions require game.setCurrentPlayer() before calling them.
// Blocking/undo actions (undoTax, undoBribe, blockArrest, saveFromCoup) have no turn check and can be called freely.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Game.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"

using namespace std;
using namespace coup;

// ── Basic flow ───────────────────────────────────────────────────────────────

TEST_CASE("Basic game flow") {
    Game game;
    Governor gov(game, "Gov");
    Spy spy(game, "Spy");
    Baron baron(game, "Baron");

    CHECK(gov.getCoins() == 0);

    game.setCurrentPlayer("Gov");
    gov.gather();
    CHECK(gov.getCoins() == 1);

    game.setCurrentPlayer("Spy");
    spy.gather();
    spy.tax();
    CHECK(spy.getCoins() == 3);

    game.setCurrentPlayer("Baron");
    baron.gather();
    baron.tax();
    CHECK(baron.getCoins() == 3);
}

// ── Turn enforcement ─────────────────────────────────────────────────────────

TEST_CASE("Turn enforcement - acting out of turn throws correct message") {
    Game game;
    Governor gov(game, "Gov");
    Spy spy(game, "Spy");

    game.setCurrentPlayer("Gov");
    CHECK_THROWS_WITH(spy.gather(),  "It is not your turn");
    CHECK_THROWS_WITH(spy.tax(),     "It is not your turn");
    CHECK_THROWS_WITH(spy.bribe(),   "It is not your turn");
    CHECK_THROWS_WITH(spy.arrest(gov), "It is not your turn");
    CHECK_THROWS_WITH(spy.sanction(gov), "It is not your turn");
}

TEST_CASE("Blocking and undo actions are always callable out of turn") {
    Game game;
    Spy spy(game, "Spy");
    General gen(game, "Gen");
    Governor gov(game, "Gov");
    Judge judge(game, "Judge");

    // None of these should throw a turn-related error
    game.setCurrentPlayer("Gov");
    CHECK_NOTHROW(spy.blockArrest(gen));
    CHECK_NOTHROW(spy.watchCoins(gen));
    // undoTax with no prior tax throws a game-logic error, not a turn error
    CHECK_THROWS_WITH(gov.undoTax(spy), "Cannot undo tax: last action was not tax");
    // undoBribe with no prior bribe throws a game-logic error, not a turn error
    CHECK_THROWS_WITH(judge.undoBribe(spy), "No action found for player");
}

// ── Governor ─────────────────────────────────────────────────────────────────

TEST_CASE("Governor tax gives 3 coins") {
    Game game;
    Governor gov(game, "Gov");
    Spy spy(game, "Spy");

    game.setCurrentPlayer("Gov");
    gov.tax();
    CHECK(gov.getCoins() == 3);
}

TEST_CASE("Governor undoTax reverses 2 coins for regular player tax") {
    Game game;
    Governor gov(game, "Gov");
    Spy spy(game, "Spy");

    game.setCurrentPlayer("Spy");
    spy.tax();
    CHECK(spy.getCoins() == 2);
    gov.undoTax(spy);
    CHECK(spy.getCoins() == 0);
}

TEST_CASE("Governor undoTax reverses 3 coins when target is a Governor") {
    Game game;
    Governor gov(game, "Gov");
    Governor gov2(game, "Gov2");

    game.setCurrentPlayer("Gov");
    gov.tax();
    CHECK(gov.getCoins() == 3);
    gov2.undoTax(gov);
    CHECK(gov.getCoins() == 0);
}

TEST_CASE("Governor can't undo tax if target last action was not tax") {
    Game game;
    Governor gov(game, "Gov");
    Spy spy(game, "Spy");

    game.setCurrentPlayer("Spy");
    spy.gather();
    CHECK_THROWS_WITH(gov.undoTax(spy), "Cannot undo tax: last action was not tax");
}

// ── Spy ───────────────────────────────────────────────────────────────────────

TEST_CASE("Spy blockArrest protects target from arrest") {
    Game game;
    Spy spy(game, "Spy");
    General gen(game, "Gen");

    spy.blockArrest(gen);
    game.setCurrentPlayer("Spy");
    CHECK_THROWS_WITH(spy.arrest(gen), "Target is currently protected from arrest");
}

TEST_CASE("Spy watchCoins returns correct amount") {
    Game game;
    Spy spy(game, "Spy");
    Baron baron(game, "Baron");

    game.setCurrentPlayer("Baron");
    for (int i = 0; i < 3; ++i) baron.gather();
    CHECK(spy.watchCoins(baron) == 3);
}

TEST_CASE("Spy bribe sets skipTurn and deducts 4 coins") {
    Game game;
    Spy spy(game, "Spy");

    game.setCurrentPlayer("Spy");
    for (int i = 0; i < 4; ++i) spy.gather();
    spy.bribe();
    CHECK(spy.getCoins() == 0);
    CHECK(game.getSkipTurn() == true);
}

TEST_CASE("Spy cannot bribe with less than 4 coins") {
    Game game;
    Spy spy(game, "Spy");

    game.setCurrentPlayer("Spy");
    CHECK_THROWS_WITH(spy.bribe(), "Not enough coins to bribe");
}

// ── Baron ─────────────────────────────────────────────────────────────────────

TEST_CASE("Baron invest - net gain is 3 coins") {
    Game game;
    Baron baron(game, "Baron");

    game.setCurrentPlayer("Baron");
    for (int i = 0; i < 3; ++i) baron.gather();
    baron.invest();
    CHECK(baron.getCoins() == 6);
}

TEST_CASE("Baron invest fails with less than 3 coins") {
    Game game;
    Baron baron(game, "Baron");

    game.setCurrentPlayer("Baron");
    baron.gather();
    baron.gather();
    CHECK_THROWS_WITH(baron.invest(), "Not enough coins to invest (requires 3)");
}

TEST_CASE("Baron invest reaches 10 coins and must coup") {
    Game game;
    Baron baron(game, "Baron");
    Governor gov(game, "Gov");

    game.setCurrentPlayer("Baron");
    for (int i = 0; i < 3; ++i) baron.gather();
    baron.tax();
    baron.invest();
    for (int i = 0; i < 2; ++i) baron.gather();
    CHECK(baron.getCoins() == 10);
    CHECK_THROWS_WITH(baron.gather(), "You must perform a coup when you have 10 coins");
}

TEST_CASE("Baron coup fails if not enough coins") {
    Game game;
    Baron baron(game, "Baron");
    Governor gov(game, "Gov");

    game.setCurrentPlayer("Baron");
    CHECK_THROWS_WITH(baron.coup(gov), "Not enough coins to perform coup");
}

// ── General ───────────────────────────────────────────────────────────────────

TEST_CASE("General arrest steals 1 coin from target") {
    Game game;
    General gen(game, "Gen");
    Spy spy(game, "Spy");

    game.setCurrentPlayer("Spy");
    spy.gather(); // spy has 1
    game.setCurrentPlayer("Gen");
    gen.arrest(spy);
    CHECK(gen.getCoins() == 1);
    CHECK(spy.getCoins() == 0);
}

TEST_CASE("General arrest - cannot arrest same player twice in a row") {
    Game game;
    General gen(game, "Gen");
    Spy spy(game, "Spy");

    game.setCurrentPlayer("Spy");
    spy.gather();
    game.setCurrentPlayer("Gen");
    gen.arrest(spy);
    CHECK_THROWS_WITH(gen.arrest(spy), "You cannot arrest the same player in consecutive turns");
}

TEST_CASE("General coup removes player") {
    Game game;
    General gen(game, "Gen");
    Spy spy(game, "Spy");

    game.setCurrentPlayer("Gen");
    for (int i = 0; i < 7; ++i) gen.gather();
    gen.coup(spy);
    CHECK(!spy.getIsAlive());
}

TEST_CASE("General can't sanction without enough coins") {
    Game game;
    General gen(game, "Gen");
    Spy spy(game, "Spy");

    game.setCurrentPlayer("Gen");
    CHECK_THROWS_WITH(gen.sanction(spy), "Not enough coins to sanction (requires 3)");
}

TEST_CASE("General saveFromCoup restores eliminated player and costs 5 coins") {
    Game game;
    General gen(game, "Gen");
    Spy spy(game, "Spy");
    Baron baron(game, "Baron");

    game.setCurrentPlayer("Baron");
    for (int i = 0; i < 7; ++i) baron.gather();
    baron.coup(spy);
    CHECK(!spy.getIsAlive());

    game.setCurrentPlayer("Gen");
    for (int i = 0; i < 5; ++i) gen.gather();
    gen.saveFromCoup(spy); // out-of-turn reaction, no turn check
    CHECK(spy.getIsAlive());
    CHECK(gen.getCoins() == 0);
}

TEST_CASE("General saveFromCoup fails if target is already alive") {
    Game game;
    General gen(game, "Gen");
    Spy spy(game, "Spy");

    game.setCurrentPlayer("Gen");
    for (int i = 0; i < 5; ++i) gen.gather();
    CHECK_THROWS_WITH(gen.saveFromCoup(spy), "Target player is already alive");
}

TEST_CASE("General saveFromCoup fails without enough coins") {
    Game game;
    General gen(game, "Gen");
    Spy spy(game, "Spy");

    spy.setIsAlive(false);
    CHECK_THROWS_WITH(gen.saveFromCoup(spy), "Not enough coins to save from coup");
}

TEST_CASE("General getFromArrested recovers 1 coin") {
    Game game;
    General gen(game, "Gen");

    gen.setArrested(true);
    gen.getFromArrested();
    CHECK(gen.getCoins() == 1);
}

TEST_CASE("General getFromArrested fails when not arrested") {
    Game game;
    General gen(game, "Gen");

    CHECK_THROWS_WITH(gen.getFromArrested(), "General was not arrested");
}

// ── Judge ─────────────────────────────────────────────────────────────────────

TEST_CASE("Judge undoBribe cancels skipTurn") {
    Game game;
    Spy spy(game, "Spy");
    Judge judge(game, "Judge");

    game.setCurrentPlayer("Spy");
    for (int i = 0; i < 4; ++i) spy.gather();
    spy.bribe();
    CHECK(game.getSkipTurn() == true);
    judge.undoBribe(spy);
    CHECK(game.getSkipTurn() == false);
}

TEST_CASE("Judge undoBribe does not refund coins to the bribing player") {
    Game game;
    Spy spy(game, "Spy");
    Judge judge(game, "Judge");

    game.setCurrentPlayer("Spy");
    for (int i = 0; i < 4; ++i) spy.gather();
    spy.bribe();
    judge.undoBribe(spy);
    CHECK(spy.getCoins() == 0); // coins are NOT returned
}

TEST_CASE("Undo bribe works if bribe is last action") {
    Game game;
    Spy spy(game, "Spy");
    Judge judge(game, "Judge");

    game.setCurrentPlayer("Spy");
    for (int i = 0; i < 4; ++i) spy.gather();
    spy.bribe();
    CHECK_NOTHROW(judge.undoBribe(spy));
}

TEST_CASE("Undo bribe fails if bribe is not the last action") {
    Game game;
    Spy spy(game, "Spy");
    Judge judge(game, "Judge");

    game.setCurrentPlayer("Spy");
    for (int i = 0; i < 5; ++i) spy.gather();
    spy.bribe();
    spy.gather(); // action after bribe
    CHECK_THROWS_WITH(judge.undoBribe(spy), "Last action was not bribe, cannot undo");
}

TEST_CASE("Judge addToCashDesk gains 1 coin when sanctioned") {
    Game game;
    Spy spy(game, "Spy");
    Judge judge(game, "Judge");

    game.setCurrentPlayer("Spy");
    for (int i = 0; i < 3; ++i) spy.gather();
    spy.sanction(judge);
    judge.addToCashDesk();
    CHECK(judge.getCoins() == 1);
}

TEST_CASE("Judge addToCashDesk fails when not sanctioned") {
    Game game;
    Judge judge(game, "Judge");

    CHECK_THROWS(judge.addToCashDesk());
}

// ── Merchant ──────────────────────────────────────────────────────────────────

TEST_CASE("Merchant gather accumulates coins correctly") {
    Game game;
    Merchant merch(game, "Merch");

    game.setCurrentPlayer("Merch");
    for (int i = 0; i < 3; ++i) merch.gather();
    CHECK(merch.getCoins() == 3);
}

TEST_CASE("Merchant arrest rule - attacker gains nothing, merchant pays 2 to bank") {
    Game game;
    General gen(game, "Gen");
    Merchant merch(game, "Merch");

    game.setCurrentPlayer("Merch");
    for (int i = 0; i < 4; ++i) merch.gather(); // merch has 4

    game.setCurrentPlayer("Gen");
    int genBefore = gen.getCoins(); // 0
    gen.arrest(merch);
    CHECK(gen.getCoins() == genBefore);   // attacker gains nothing
    CHECK(merch.getCoins() == 2);         // merchant lost 2, not 1
}

// ── 10-coin rule ──────────────────────────────────────────────────────────────

TEST_CASE("Player must coup at 10 coins - gather throws") {
    Game game;
    Governor gov(game, "Gov");
    Spy spy(game, "Spy");

    game.setCurrentPlayer("Gov");
    for (int i = 0; i < 10; ++i) gov.gather();
    CHECK_THROWS_WITH(gov.gather(), "You must perform a coup when you have 10 coins");
}

TEST_CASE("Player must coup at 10 coins - tax throws") {
    Game game;
    Governor gov(game, "Gov");
    Spy spy(game, "Spy");

    game.setCurrentPlayer("Gov");
    for (int i = 0; i < 10; ++i) gov.gather();
    CHECK_THROWS_WITH(gov.tax(), "You must perform a coup when you have 10 coins");
}

// ── Sanction ──────────────────────────────────────────────────────────────────

TEST_CASE("Sanction prevents gather and clears on attempt") {
    Game game;
    Spy spy(game, "Spy");
    General gen(game, "Gen");

    game.setCurrentPlayer("Gen");
    for (int i = 0; i < 3; ++i) gen.gather();
    gen.sanction(spy);

    game.setCurrentPlayer("Spy");
    CHECK_THROWS(spy.tax());  // sanction blocks and clears
    spy.gather();             // now allowed (sanction was cleared on failed tax)
    CHECK(spy.getCoins() == 1);
}

TEST_CASE("Sanction followed by coup") {
    Game game;
    General gen(game, "Gen");
    Spy spy(game, "Spy");

    game.setCurrentPlayer("Gen");
    for (int i = 0; i < 9; ++i) gen.gather(); // 9 coins
    gen.sanction(spy);                         // -3 → 6
    gen.gather();                              // 7
    gen.gather();                              // 8
    gen.gather();                              // 9
    gen.gather();                              // 10
    gen.coup(spy);                             // -7 → spy eliminated
    CHECK(!spy.getIsAlive());
}

TEST_CASE("Arrest right after sanction") {
    Game game;
    General gen(game, "Gen");
    Spy spy(game, "Spy");

    game.setCurrentPlayer("Gen");
    for (int i = 0; i < 4; ++i) gen.gather();
    gen.sanction(spy);

    spy.clearSanction();

    game.setCurrentPlayer("Spy");
    spy.gather(); // spy has 1

    game.setCurrentPlayer("Gen");
    gen.arrest(spy);
    CHECK(spy.getCoins() == 0);
    CHECK(spy.getIsAlive());
}

// ── Multi-player and game state ───────────────────────────────────────────────

TEST_CASE("Spy bribes, Judge undoes, Governor cannot undo non-existent tax") {
    Game game;
    Spy spy(game, "Spy");
    Judge judge(game, "Judge");
    Governor gov(game, "Gov");

    game.setCurrentPlayer("Spy");
    for (int i = 0; i < 4; ++i) spy.gather();
    spy.bribe();
    judge.undoBribe(spy);

    CHECK(game.getSkipTurn() == false);
    CHECK(spy.getCoins() == 0);          // coins are not refunded by undoBribe
    CHECK_THROWS(gov.undoTax(spy));      // last action was bribe, not tax
}

TEST_CASE("Baron invests, then is sanctioned and continues") {
    Game game;
    Baron baron(game, "Baron");
    General gen(game, "Gen");

    game.setCurrentPlayer("Baron");
    for (int i = 0; i < 3; ++i) baron.gather();
    baron.invest();
    CHECK(baron.getCoins() == 6);

    game.setCurrentPlayer("Gen");
    for (int i = 0; i < 3; ++i) gen.gather();
    gen.sanction(baron);

    baron.clearSanction();

    game.setCurrentPlayer("Baron");
    baron.gather();
    CHECK(baron.getCoins() == 7);
}

TEST_CASE("Game handles 4 players in correct turn order") {
    Game game;
    Spy spy(game, "Spy");
    Governor gov(game, "Gov");
    Baron baron(game, "Baron");
    Judge judge(game, "Judge");

    game.setCurrentPlayer("Spy");
    spy.gather();
    game.setCurrentPlayer("Gov");
    gov.gather();
    game.setCurrentPlayer("Baron");
    baron.gather();
    game.setCurrentPlayer("Judge");
    judge.gather();

    CHECK(spy.getCoins() == 1);
    CHECK(gov.getCoins() == 1);
    CHECK(baron.getCoins() == 1);
    CHECK(judge.getCoins() == 1);
}

TEST_CASE("Game declares winner when only one player is alive") {
    Game game;
    vector<string> names = {"Gen", "Spy"};
    game.addPlayer(names);

    Player* gen = nullptr;
    Player* spy = nullptr;
    for (Player* p : game.getPlayers()) {
        if (p->getName() == "Gen") gen = p;
        else if (p->getName() == "Spy") spy = p;
    }
    REQUIRE(gen != nullptr);
    REQUIRE(spy != nullptr);

    spy->setIsAlive(false);
    CHECK(game.getIsGameOver() == true);
    CHECK(game.winner() == "Gen");
}

TEST_CASE("winner throws when game is not over") {
    Game game;
    Governor gov(game, "Gov");
    Spy spy(game, "Spy");

    CHECK_THROWS_WITH(game.winner(), "Game is not over yet");
}

// ── History ───────────────────────────────────────────────────────────────────

TEST_CASE("Game history tracks actions in order") {
    Game game;
    Spy spy(game, "Spy");

    game.setCurrentPlayer("Spy");
    for (int i = 0; i < 4; ++i) spy.gather();
    spy.bribe();
    spy.gather();
    spy.tax();

    auto history = game.getHistory();
    CHECK(history.size() >= 2);
    CHECK(history[0].action == "gather");
    CHECK(history.back().action == "tax");
}

TEST_CASE("Game history records bribe correctly") {
    Game game;
    Spy spy(game, "Spy");

    game.setCurrentPlayer("Spy");
    for (int i = 0; i < 4; ++i) spy.gather();
    spy.bribe();
    CHECK(game.getHistory().back().action == "bribe");
}

// ── Dead target guards ────────────────────────────────────────────────────────

TEST_CASE("Player can't arrest dead target") {
    Game game;
    General gen(game, "Gen");
    Spy spy(game, "Spy");

    spy.setIsAlive(false);
    game.setCurrentPlayer("Gen");
    CHECK_THROWS_WITH(gen.arrest(spy), "Target is not alive");
}

TEST_CASE("Player can't coup dead target") {
    Game game;
    Baron baron(game, "Baron");
    Spy spy(game, "Spy");

    spy.setIsAlive(false);
    game.setCurrentPlayer("Baron");
    for (int i = 0; i < 7; ++i) baron.gather();
    CHECK_THROWS_WITH(baron.coup(spy), "Target is not alive");
}

TEST_CASE("Player cannot arrest if target is arrest-blocked") {
    Game game;
    Spy spy(game, "Spy");
    General gen(game, "Gen");

    spy.blockArrest(gen);
    game.setCurrentPlayer("Spy");
    CHECK_THROWS_WITH(spy.arrest(gen), "Target is currently protected from arrest");
}