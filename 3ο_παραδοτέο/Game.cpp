// =============================================================================
// Game.cpp
// -----------------------------------------------------------------------------
// Implements the Game class: round lifecycle, turn management, scoring,
// and deck replenishment.
// =============================================================================

#include "Game.h"
#include "Card.h" // Provides suitToString() — no need for extern
#include "Player.h"
#include <iostream>
// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
// Deletes the players pointer array.
// The Player objects themselves are NOT deleted — they are stack-allocated in main.
Game::~Game()
{
    delete[] players;
}

// ---------------------------------------------------------------------------
// Getters
// ---------------------------------------------------------------------------

int Game::getNoOfPlayers() const
{
    return noOfPlayers;
}

Deck &Game::getDeck()
{
    return deck;
}

Table &Game::getTable()
{
    return table;
}

Player **Game::getPlayers() const
{
    return players;
}

int Game::getCurrentPlayerIndex() const
{
    return currentPlayerIndex;
}

void Game::setCurrentPlayerIndex(int idx)
{
    currentPlayerIndex = idx;
}

bool Game::getSkipNext() const
{
    return skipNext;
}

void Game::setSkipNext(bool b)
{
    skipNext = b;
}

bool Game::getMustPlayAgain() const
{
    return mustPlayAgain;
}

void Game::setMustPlayAgain(bool b)
{
    mustPlayAgain = b;
}

int Game::getPenaltyStack() const
{
    return penaltyStack;
}

void Game::setPenaltyStack(int p)
{
    penaltyStack = p;
}

// Increases the penalty stack by p (used when a Seven is stacked on another Seven)
void Game::addPenaltyStack(int p)
{
    penaltyStack += p;
}

// ---------------------------------------------------------------------------
// endRound
// ---------------------------------------------------------------------------
// Computes scores and prints a summary of the round results.
void Game::endRound()
{
    computeScore();

    for (int i = 0; i < noOfPlayers; ++i)
    {
        Player *p = players[i];
        cout << p->getName() << " score points this round: " << p->getHand().calculatePoints() << endl;
        cout << "Total Game Points: " << p->getPoints() << endl;
    }
}

// ---------------------------------------------------------------------------
// computeScore
// ---------------------------------------------------------------------------
// Adds each player's remaining hand points to their cumulative total.
// Called at the end of every round.
void Game::computeScore()
{
    for (int i = 0; i < noOfPlayers; ++i)
    {
        players[i]->addPoints(players[i]->getHand().calculatePoints());
    }
}

// ---------------------------------------------------------------------------
// printState
// ---------------------------------------------------------------------------
// Prints the current state of the game: each player's card count, score,
// and status, plus the top card on the table and the penalty stack.
void Game::printState() const
{
    cout << "\n--- PLAYERS ---" << endl;
    for (int i = 0; i < noOfPlayers; ++i)
    {
        Player *p = players[i];
        cout << p->getName() << " (Score: " << p->getPoints() << ") - " << p->getHand().numberOfCards()
             << " cards left. "
             << "Status: " << statusToString(p->getStatus()) << "\n";
    }

    if (const Card *top = table.topCard())
    {
        cout << "--- TABLE ---" << endl;
        cout << "Top Card: " << top->getValue() << " of " << suitToString(top->getSuit());

        if (table.getHasDeclaredSuit())
        {
            cout << " (Declared Suit: " << suitToString(table.getDeclaredSuit()) << ")";
        }

        cout << "\nPenalty Stack: " << penaltyStack << endl;
    }

    cout << "===============\n";
}

// ---------------------------------------------------------------------------
// getCurrentPlayer
// ---------------------------------------------------------------------------
// Returns the player whose turn it currently is.
Player *Game::getCurrentPlayer() const
{
    if (noOfPlayers > 0)
    {
        return players[currentPlayerIndex];
    }
    return nullptr;
}

Game::Game(Player *p1, Player *p2)
{
    currentPlayerIndex = 0;
    skipNext = false;
    penaltyStack = 0;
    players = new Player *[noOfPlayers];
    players[0] = p1;
    players[1] = p2;
}
Game::Game(Player *p1, Player *p2, Player *p3, Player *p4)
{
    currentPlayerIndex = 0;
    skipNext = false;
    penaltyStack = 0;
    players = new Player *[noOfPlayers];
    players[0] = p1;
    players[1] = p2;
    players[2] = p3;
    players[3] = p4;
}

void Game::startRound()
{
    for (int i = 0; i < noOfPlayers; i++)
    {
        players[i]->clearHand();
        players[i]->setHasDrawnCard(false);
        players[i]->setStatus(HASNTPLAYED);
        table.clearPile();
        deck.reset();
        deck.shuffle();
        for (int i = 0; i < noOfPlayers; i++)
            for (int k = 0; i < 7; k++)
                players[i]->drawCard(deck.deal());

        table.addCard(deck.deal());
        penaltyStack = 0;
        skipNext = false;
        mustPlayAgain = false;
        currentPlayerIndex = 0;
    }
}

void Game::advanceTurn()
{
    if (mustPlayAgain == true)
    {
        mustPlayAgain = false;
        return;
    }
    if (skipNext == true)
    {
        cout << "\n" << players[currentPlayerIndex]->getName() << "misses their turn!" << endl;
        players[currentPlayerIndex]->setStatus(MISSEDTURN);
        skipNext = false;
        this->advanceTurn();
    }

    if (currentPlayerIndex < noOfPlayers - 1)
    {
        currentPlayerIndex++;
    }
    else
    {
        for (int i = 0; i < noOfPlayers; i++)
            if (players[i]->getStatus() == PASSED || players[i]->getStatus() == PLAYED ||
                players[i]->getStatus() == MISSEDTURN)
                players[i]->setStatus(HASNTPLAYED);
        currentPlayerIndex = 0;
    }
}

void Game::replenishDeckIfNeeded(int neededCards)
{
    if (deck.getCurrentSize() < neededCards)
    {
        Card *topCard = table.getPile()[table.getPileSize() - 1];
        deck.rebuildFromDiscard(table.getPile(), table.getPileSize());
        table.clearPile();
        table.addCard(topCard);
    }
}

void Game::applyPenalty(Player *p)
{
    for (int i = 0; i < penaltyStack; i++)
        p->drawCard(deck.deal());
    cout << p->getName() << " absorbs " << penaltyStack << " penalty card(s)!\n";
    penaltyStack = 0;
    p->setHasDrawnCard(false);
}
