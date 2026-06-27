// =============================================================================
// Game.h
// -----------------------------------------------------------------------------
// Declares the Game class, which is the central controller of the Agonia game.
//
// The Game owns:
//   - The Deck (the draw pile)
//   - The Table (the discard pile)
//   - An array of Player pointers (players are NOT owned — they live in main)
//
// The Game manages:
//   - Round lifecycle (startRound, endRound, computeScore)
//   - Turn flow (getCurrentPlayer, advanceTurn)
//   - Special effect state (skipNext, mustPlayAgain, penaltyStack)
//   - Deck replenishment when cards run out
// =============================================================================

#ifndef GAME_H
#define GAME_H

#include "Deck.h"
#include "Player.h"
#include "Table.h"

// =============================================================================
// Game – The central game controller
// =============================================================================
class Game
{
  private:
    int noOfPlayers; // Total number of players in the match
    Deck deck;       // The draw pile (owned by Game)
    Table table;     // The discard pile (owned by Game)

    Player **players; // Array of Player pointers (NOT owned — players live in main)

    // --- Turn state ---
    int currentPlayerIndex; // Index into players[] of the player whose turn it is
    bool skipNext;          // If true, the next player's turn will be skipped (Nine effect)
    bool mustPlayAgain;     // If true, the current player gets another turn (Eight effect)
    int penaltyStack;       // Accumulated draw-two penalty (grows when Sevens are stacked)

  public:
    // Constructors: the only valid player counts are 2 or 4.
    // The compiler enforces this — no other overload exists.
    Game(Player *p1, Player *p2);
    Game(Player *p1, Player *p2, Player *p3, Player *p4);

    // Destructor: deletes the players array (NOT the Player objects themselves)
    ~Game();

    // --- Getters ---
    int getNoOfPlayers() const;
    Deck &getDeck();
    Table &getTable();
    Player **getPlayers() const;

    // --- Turn state getters/setters ---
    int getCurrentPlayerIndex() const;
    void setCurrentPlayerIndex(int idx);

    bool getSkipNext() const;
    void setSkipNext(bool b);

    bool getMustPlayAgain() const;
    void setMustPlayAgain(bool b);

    int getPenaltyStack() const;
    void setPenaltyStack(int p);
    void addPenaltyStack(int p); // Adds p to the current penalty stack (for stacking Sevens)

    // --- Player management ---

    // getCurrentPlayer(): returns the player whose turn it currently is
    Player *getCurrentPlayer() const;

    // --- Round lifecycle ---

    // startRound(): resets all hands and the table, rebuilds and shuffles the deck,
    // deals 7 cards to each player, and places the first card on the table.
    void startRound();

    // endRound(): calls computeScore() and prints each player's round/total score.
    void endRound();

    // computeScore(): adds each player's remaining hand points to their total.
    void computeScore();

    // printState(): prints all player info (cards, score, status) and the table state.
    void printState() const;

    // --- Turn flow ---

    // advanceTurn(): moves to the next player, handling Eight (play again) and
    // Nine (skip next) effects. Also resets player statuses when a full orbit completes.
    void advanceTurn();

    // --- Deck management ---

    // replenishDeckIfNeeded(): if the deck has fewer than `neededCards` cards,
    // recycles the discard pile (all but the top card) back into the deck and shuffles.
    void replenishDeckIfNeeded(int neededCards);

    // applyPenalty(): forces player `p` to draw `penaltyStack` cards, then resets the stack.
    void applyPenalty(Player *p);
};

#endif // GAME_H
