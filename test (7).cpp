//vanunuraz@gmail.com
// Comprehensive and extended tests for Coup game and all role actions using doctest framework

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Game.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"

using namespace coup;

//Verifies basic flow of gather/tax and coin accumulation
TEST_CASE("Basic game flow"){
    //Initialize game and players
    Game game; 
    Governor gov(game, "Gov");
    Spy spy(game, "Spy");
    Baron baron(game, "Baron");
    //
    CHECK(gov.getCoins()== 0);
    gov.gather();
    CHECK(gov.getCoins()== 1);
    spy.gather();
    spy.tax();
    CHECK(spy.getCoins()== 3);
    baron.gather();
    baron.tax();
    CHECK(baron.getCoins()== 3);
}

TEST_CASE("Governor undoTax") {
    Game game;
    Governor gov(game, "Gov");
    Spy spy(game, "Spy");

    spy.gather();
    spy.tax();
    CHECK(spy.getCoins() == 3);
    gov.undoTax(spy);
    CHECK(spy.getCoins() == 1);
}

TEST_CASE("Governor can't undo tax if target not taxed") {
    Game game;
    Governor gov(game, "Gov");
    Spy spy(game, "Spy");
    spy.gather();
    CHECK_THROWS(gov.undoTax(spy));
}

TEST_CASE("Spy blockArrest protects target") {
    Game game;
    Spy spy(game, "Spy");
    General gen(game, "Gen");
    spy.blockArrest(gen);
    CHECK_THROWS(spy.arrest(gen));
}

TEST_CASE("Spy watchCoins returns correct amount") {
    Game game;
    Spy spy(game, "Spy");
    Baron baron(game, "Baron");
    for (int i = 0; i < 3; ++i) baron.gather();
    CHECK(spy.watchCoins(baron) == 3);
}

TEST_CASE("Spy bribe and skipTurn") {
    Game game;
    Spy spy(game, "Spy");
    for (int i = 0; i < 4; ++i) spy.gather();
    spy.bribe();
    CHECK(spy.getCoins() == 0);
    CHECK(game.getSkipTurn());
}

TEST_CASE("Spy cannot bribe with less than 4 coins") {
    Game game;
    Spy spy(game, "Spy");
    CHECK_THROWS(spy.bribe());
}

TEST_CASE("Baron invest and coup") {
    Game game;
    Baron baron(game, "Baron");
    Governor gov(game, "Gov");

    for (int i = 0; i < 3; ++i) baron.gather();
    baron.tax();
    baron.invest();
    for (int i = 0; i < 4; ++i) baron.gather();

    CHECK(baron.getCoins() == 10);
    baron.coup(gov);
    CHECK(!gov.getIsAlive());
}

TEST_CASE("Baron coup fails if not enough coins") {
    Game game;
    Baron baron(game, "Baron");
    Governor gov(game, "Gov");
    CHECK_THROWS(baron.coup(gov));
}

TEST_CASE("General arrest and double arrest error") {
    Game game;
    General gen(game, "Gen");
    Spy spy(game, "Spy");
    gen.gather();
    spy.gather();
    gen.arrest(spy);
    CHECK(spy.getCoins() == 0);
    CHECK_THROWS(gen.arrest(spy));
}

TEST_CASE("General coup removes player") {
    Game game;
    General gen(game, "Gen");
    Spy spy(game, "Spy");
    for (int i = 0; i < 7; ++i) gen.gather();
    gen.coup(spy);
    CHECK(!spy.getIsAlive());
}

TEST_CASE("General can't sanction without enough coins") {
    Game game;
    General gen(game, "Gen");
    Spy spy(game, "Spy");
    CHECK_THROWS(gen.sanction(spy));
}

TEST_CASE("Judge undo bribe and addToCashDesk") {
    Game game;
    Spy spy(game, "Spy");
    Judge judge(game, "Judge");
    for (int i = 0; i < 4; ++i) spy.gather();
    spy.bribe();
    spy.setIsAlive(false);
    judge.undoBribe(spy);
    CHECK_NOTHROW(game.setSkipTurn(false));
    spy.setIsAlive(true);
    for (int i = 0; i < 3; ++i) spy.gather();
    spy.sanction(judge);
    judge.addToCashDesk();
    CHECK(judge.getCoins() == 1);
}

TEST_CASE("Undo bribe works if bribe is last action") {
    Game game;
    Spy spy(game, "Spy");
    Judge judge(game, "Judge");
    for (int i = 0; i < 4; ++i) spy.gather();
    spy.bribe();
    CHECK_NOTHROW(judge.undoBribe(spy)); // מותר כי bribe אחרון
}

TEST_CASE("Undo bribe fails if bribe is not last action") {
    Game game;
    Spy spy(game, "Spy");
    Judge judge(game, "Judge");
    for (int i = 0; i < 5; ++i) spy.gather(); // 5 = 4 + עוד אחת אחרי bribe
    spy.bribe();
    spy.gather();  // פעולה נוספת
    CHECK_THROWS_WITH(judge.undoBribe(spy), "Last action was not bribe, cannot undo");
}


TEST_CASE("Judge addToCashDesk fails when not sanctioned") {
    Game game;
    Judge judge(game, "Judge");
    CHECK_THROWS(judge.addToCashDesk());
}

TEST_CASE("Merchant gather only") {
    Game game;
    Merchant merch(game, "Merch");
    for (int i = 0; i < 3; ++i) merch.gather();
    CHECK(merch.getCoins() == 3);
}

TEST_CASE("Sanction prevents one turn") {
    Game game;
    Spy spy(game, "Spy");
    General gen(game, "Gen");
    for (int i = 0; i < 3; ++i) gen.gather();
    gen.sanction(spy);
    CHECK_THROWS(spy.tax());
    spy.clearSanction();
    spy.gather();
    CHECK(spy.getCoins() == 1);
}

TEST_CASE("Player can act with 10+ coins if allowed") {
    Game game;
    Governor gov(game, "Gov");
    for (int i = 0; i < 10; ++i) gov.gather();
    CHECK_NOTHROW(gov.gather());
}

TEST_CASE("Player can't arrest dead target") {
    Game game;
    General gen(game, "Gen");
    Spy spy(game, "Spy");
    spy.setIsAlive(false);
    CHECK_THROWS(gen.arrest(spy));
}

TEST_CASE("Player can't coup dead target") {
    Game game;
    Baron baron(game, "Baron");
    Spy spy(game, "Spy");
    spy.setIsAlive(false);
    for (int i = 0; i < 7; ++i) baron.gather();
    CHECK_THROWS(baron.coup(spy));
}

TEST_CASE("Player cannot arrest if blocked") {
    Game game;
    Spy spy(game, "Spy");
    General gen(game, "Gen");
    spy.blockArrest(gen);
    CHECK_THROWS(spy.arrest(gen));
}

TEST_CASE("Game history tracks actions") {
    Game game;
    Spy spy(game, "Spy");
    for (int i = 0; i < 4; ++i) spy.gather();
    spy.bribe();
    CHECK(game.getHistory().size() >= 1);
    CHECK(game.getHistory().back().action == "bribe");
}

TEST_CASE("Sanction followed by coup") {
    Game game;
    General gen(game, "Gen");
    Spy spy(game, "Spy");

    for (int i = 0; i < 9; ++i) gen.gather();  // 9 coins
    gen.sanction(spy);                         // -3 → 6
    gen.gather();                              // 7
    gen.gather();                              // 8
    gen.gather();                              // 9
    gen.gather();                              // 10
    gen.coup(spy);                             // -7 → spy מת
    CHECK(!spy.getIsAlive());
}


TEST_CASE("Arrest right after sanction") {
    Game game;
    General gen(game, "Gen");
    Spy spy(game, "Spy");
    for (int i = 0; i < 4; ++i) gen.gather();
    gen.sanction(spy);
    spy.clearSanction();
    spy.gather();
    gen.arrest(spy);
    CHECK(spy.getCoins() == 0);
    CHECK(spy.getIsAlive());
}

TEST_CASE("Spy bribes, Judge undoes, Governor tries to undo non-existent tax") {
    Game game;
    Spy spy(game, "Spy");
    Judge judge(game, "Judge");
    Governor gov(game, "Gov");

    for (int i = 0; i < 4; ++i) spy.gather();   // spy עם 4 מטבעות
    spy.bribe();                                // הפסיד את כל המטבעות
    judge.undoBribe(spy);                       // ביטול brib – אבל לא מחזיר לו מטבעות
    spy.setIsAlive(true);                       // מחזירים לחיים

    CHECK_THROWS(gov.undoTax(spy));             // לא היה tax – מצפים לשגיאה

    CHECK(game.getSkipTurn() == false);         // ביטול bribe גם ביטל את דילוג התור
    CHECK(spy.getCoins() == 0);                 // לא קיבל חזרה את ה־4 מטבעות!
}

TEST_CASE("Baron invests, then is sanctioned and still continues") {
    Game game;
    Baron baron(game, "Baron");
    General gen(game, "Gen");

    for (int i = 0; i < 3; ++i) baron.gather();
    baron.invest(); // אמור להביא 6
    CHECK(baron.getCoins() == 6);

    for (int i = 0; i < 3; ++i) gen.gather();
    gen.sanction(baron);
    baron.clearSanction();
    baron.gather();
    CHECK(baron.getCoins() == 7);
}

TEST_CASE("Game handles 4+ players and correct turn rotation") {
    Game game;
    Spy spy(game, "Spy");
    Governor gov(game, "Gov");
    Baron baron(game, "Baron");
    Judge judge(game, "Judge");

    spy.gather();    // 1
    gov.gather();    // 2
    baron.gather();  // 3
    judge.gather();  // 4

    CHECK(spy.getCoins() == 1);
    CHECK(judge.getCoins() == 1);
}
TEST_CASE("Game declares winner when only one alive using addPlayer") {
    Game game;
    vector<string> names = {"Gen", "Spy"};
    game.addPlayer(names);

    Player* gen = nullptr;
    Player* spy = nullptr;

    // נאתר מי קיבל איזה תפקיד
    for (Player* p : game.getPlayers()) {
        if (p->getName() == "Gen") {
            gen = p;
        } else if (p->getName() == "Spy") {
            spy = p;
        }
    }

    REQUIRE(gen != nullptr);
    REQUIRE(spy != nullptr);

    // נהרוג את Spy
    spy->setIsAlive(false);
    gen->setIsAlive(true);

    CHECK(game.getIsGameOver() == true);
    CHECK(game.winner() == "Gen");
}




TEST_CASE("Multiple actions recorded in history") {
    Game game;
    Spy spy(game, "Spy");
    for (int i = 0; i < 4; ++i) spy.gather();
    spy.bribe();
    spy.gather();
    spy.tax();

    auto history = game.getHistory();
    CHECK(history.size() >= 2);
    CHECK(history[0].action == "gather");
    CHECK(history.back().action == "tax");
}


