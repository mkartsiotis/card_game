// =============================================================================
// main.cpp
// -----------------------------------------------------------------------------
// Entry point for the Agonia card game.
//
// This file wires together all the classes (Game, Player, Deck, Table, Hand,
// Card) and runs the full match loop.
//
// Current mode: AUTO-PLAY
//   All player decisions are made randomly using rand().
//   To add real human input, use the readIntInput() function from InputUtils.h
//   and replace the auto-selection logic in executePlayerTurn().
//
// Match structure:
//   - A match consists of multiple rounds.
//   - Each round ends when one player empties their hand.
//   - All other players add their remaining card points to their total score.
//   - The match ends when any player reaches or exceeds POINTS_LIMIT (200).
//   - The player with the LOWEST score wins.
//   - If there is a tie for the lowest score at the limit, an extra round is played.
// =============================================================================

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

#include "Card.h"
#include "Deck.h"
#include "Game.h"
#include "Hand.h"
#include "Player.h"
#include "Table.h"
Card **getValidCards(Game &g, Player *current, int &validCount);
// void handleChoice(Game &g, Player *current, Card **validCards, int validCount, int choice, bool &turnEnded,
//                  bool &gameRunning);
void handleChoice(Game &game, Player *current, Card **valid, int validCount, int choice, bool &turnEnded,
                  bool &gameRunning);
// ---------------------------------------------------------------------------
// hasPlayerReachedPointsLimit
// ---------------------------------------------------------------------------
// Returns true if at least one player has reached or exceeded the points limit.
// Used to determine when the match should end.
bool hasPlayerReachedPointsLimit(const Game &game, int pointsLimit)
{
    Player **players = game.getPlayers();
    for (int i = 0; i < game.getNoOfPlayers(); ++i)
    {
        if (players[i]->getPoints() >= pointsLimit)
        {
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------------
// hasTieForBestScore
// ---------------------------------------------------------------------------
// Returns true if two or more players share the lowest score.
// Used to decide whether to play an extra round when the limit is reached.
bool hasTieForBestScore(const Game &game)
{
    Player **players = game.getPlayers();

    // Find the lowest score among all players
    int bestScore = players[0]->getPoints();
    for (int i = 1; i < game.getNoOfPlayers(); ++i)
    {
        if (players[i]->getPoints() < bestScore)
        {
            bestScore = players[i]->getPoints();
        }
    }

    // Count how many players share that lowest score
    int bestCount = 0;
    for (int i = 0; i < game.getNoOfPlayers(); ++i)
    {
        if (players[i]->getPoints() == bestScore)
        {
            ++bestCount;
        }
    }

    return bestCount > 1; // Tie if more than one player has the best score
}

// ---------------------------------------------------------------------------
// printMatchResult
// ---------------------------------------------------------------------------
// Prints the final match summary: who won, their score, and how many rounds
// were played.
void printMatchResult(const Game &game, int pointsLimit, int roundsPlayed)
{
    Player **players = game.getPlayers();

    cout << "\n=== MATCH OVER ===\n";
    cout << "Points limit reached: " << pointsLimit << "\n";
    cout << "Rounds played: " << roundsPlayed << "\n";

    // Find the player with the lowest score (the winner)
    int winnerIndex = 0;
    int bestScore = players[0]->getPoints();
    for (int i = 1; i < game.getNoOfPlayers(); ++i)
    {
        if (players[i]->getPoints() < bestScore)
        {
            bestScore = players[i]->getPoints();
            winnerIndex = i;
        }
    }

    cout << "Winner:\n";
    cout << "- " << players[winnerIndex]->getName() << " with " << players[winnerIndex]->getPoints() << " points\n";
}

// ---------------------------------------------------------------------------
// handlePenaltyPhase
// ---------------------------------------------------------------------------
// Called at the start of a turn when a penalty stack is active (from Sevens).
// If the player has no Seven, they automatically absorb the penalty.
// If they do have a Seven, a warning is printed — the actual choice of whether
// to play it or absorb is handled later in the main turn loop.
void handlePenaltyPhase(Game &game, Player *current)
{
    if (game.getPenaltyStack() == 0)
        return;

    // Check if this player has a Seven in hand
    bool hasSeven = false;
    Card **pCards = current->getHand().getCards();
    int hSize = current->getHand().numberOfCards();
    for (int i = 0; i < hSize; i++)
    {
        if (pCards[i]->getValue() == 7)
        {
            hasSeven = true;
            break;
        }
    }

    if (!hasSeven)
    {
        // No Seven available — automatically absorb the penalty
        cout << "\nYou don't have a 7 to defend yourself! You automatically absorb the penalty.\n";
        game.applyPenalty(current);
    }
    else
    {
        // Player has a Seven — they can choose to play it or absorb in the main loop
        cout << "\nPenalty active! You have a 7 to defend. Play a 7, or enter -1 to absorb the penalty: " << endl;
    }
}

// ---------------------------------------------------------------------------
// executePlayerTurn
// ---------------------------------------------------------------------------
// Orchestrates one complete turn for the current player.
// Delegates the heavy lifting to three helpers:
//   handlePenaltyPhase() — checks and handles active penalty at turn start
//   getValidCards()      — computes which cards can be played right now
//   handleChoice()       — processes the player's chosen action
void executePlayerTurn(Game &game, bool &gameRunning)
{
    Player *current = game.getCurrentPlayer();

    // Make sure the deck has enough cards for a potential draw or penalty
    game.replenishDeckIfNeeded(game.getPenaltyStack() + 1);

    cout << "\n-----------------------------";
    cout << "\nTurn: " << current->getName();

    // Phase 1: handle any active penalty stack
    handlePenaltyPhase(game, current);

    // Phase 2: main turn loop — repeats until the player ends their turn
    bool turnEnded = false;
    while (!turnEnded)
    {
        // Get the cards the player can legally play right now
        int validCount = 0;
        Card **valid = getValidCards(game, current, validCount);

        // Print the player's full hand
        cout << "\n" << current->getName() << "'s Full Hand:\n";
        Card **currentCards = current->getHand().getCards();
        int handSize = current->getHand().numberOfCards();
        for (int i = 0; i < handSize; ++i)
        {
            cout << "  " << currentCards[i]->getValue() << "-" << suitToString(currentCards[i]->getSuit()) << "\n";
        }

        // If the player already drew and still has no valid cards, they must pass
        if (current->getHasDrawnCard() && validCount == 0)
        {
            cout << "No valid cards to play after drawing. Passing turn.\n";
            current->pass();
            turnEnded = true;
            delete[] valid;
            break;
        }

        // Print the valid card options
        cout << "\nValid Options:\n";
        if (validCount == 0)
        {
            cout << "  [None]\n";
        }
        else
        {
            for (int i = 0; i < validCount; ++i)
            {
                cout << "  " << i << ": " << valid[i]->getValue() << "-" << suitToString(valid[i]->getSuit()) << "\n";
            }
        }

        // AUTO-PLAY: randomly pick -1 (draw/pass) or one of the valid card indices
        // In a human-input version, this would be replaced by readIntInput()
        int choice = (rand() % (validCount + 1)) - 1;
        cout << "Auto-selected action: " << choice << "\n";

        handleChoice(game, current, valid, validCount, choice, turnEnded, gameRunning);

        delete[] valid; // Free the array allocated by getValidCards()
    }

    // Print the game state after this player's turn
    game.printState();

    // Advance to the next player (unless the round just ended)
    if (gameRunning)
    {
        game.advanceTurn();
    }
}

// =============================================================================
// main
// =============================================================================
// Sets up the game, creates players, and runs the outer match loop.
// Each iteration of the outer loop is one complete round.
int main()
{
    // Use a fixed seed so the game plays out the same way every run.
    // Change to srand(time(0)) for a different game each time.
    srand(42);

    const int POINTS_LIMIT = 200; // Match ends when any player reaches this score

    cout << "Starting Agonia Game Setup...\n";

    // Create the four players (stack-allocated — Game stores pointers, not copies)
    Player p1("Alice");
    Player p2("Bob");
    Player p3("Charlie");
    Player p4("Diana");

    // Create the game and register all players in one step.
    // Only Game(p1,p2) or Game(p1,p2,p3,p4) are valid — the compiler enforces this.
    Game game(&p1, &p2, &p3, &p4);

    int roundNumber = 1;

    // Outer loop: each iteration is one round
    while (true)
    {
        cout << "\n========== ROUND " << roundNumber << " ==========\n";

        game.startRound();
        game.printState();

        // Inner loop: each iteration is one player's turn
        bool gameRunning = true;
        while (gameRunning)
        {
            executePlayerTurn(game, gameRunning);
        }

        game.endRound();

        // Check if the match should end
        if (hasPlayerReachedPointsLimit(game, POINTS_LIMIT))
        {
            if (hasTieForBestScore(game))
            {
                // Tie at the limit — play one more round to break it
                cout << "Tie for best score at points limit. Playing an extra round...\n";
            }
            else
            {
                // Clear winner — print results and exit
                printMatchResult(game, POINTS_LIMIT, roundNumber);
                break;
            }
        }

        ++roundNumber;
    }

    return 0;
}

Card **getValidCards(Game &g, Player *current, int &validCount)
{
    validCount = 0; // Πολύ σημαντικό να αρχικοποιείται!

    if (g.getPenaltyStack() == 0)
    {
        // Ζητάμε την topCard απευθείας από το table
        return current->getHand().validRoundCards(g.getTable().topCard(), g.getTable().getHasDeclaredSuit(),
                                                  g.getTable().getDeclaredSuit(), validCount);
    }
    else
    {
        Card **array_of_v_cards = new Card *[52];
        Card **handCards = current->getHand().getCards();
        int handSize = current->getHand().numberOfCards();

        for (int i = 0; i < handSize; i++)
        {
            if (handCards[i]->getValue() == 7)
            {
                array_of_v_cards[validCount] = handCards[i];
                validCount++;
            }
        }
        return array_of_v_cards;
    }
}
void handleChoice(Game &game, Player *current, Card **valid, int validCount, int choice, bool &turnEnded,
                  bool &gameRunning)
{
    if (choice == -1)
    {
        // Αν δεν έχει τραβήξει κάρτα και δεν υπάρχει ποινή
        if (!current->getHasDrawnCard() && game.getPenaltyStack() == 0)
        {
            cout << current->getName() << " draws 1 card(s).\n";
            game.replenishDeckIfNeeded(1);
            current->drawCard(game.getDeck().deal());
        }
        // Αν δεν έχει τραβήξει κάρτα και υπάρχει ενεργή ποινή
        else if (!current->getHasDrawnCard() && game.getPenaltyStack() > 0)
        {
            game.applyPenalty(current);
        }
        // Αν έχει ήδη τραβήξει φύλλο
        else
        {
            cout << current->getName() << " passes.\n";
            current->pass();
            turnEnded = true;
        }
    }
    else if (choice >= 0 && choice < validCount)
    {
        Card *playedCard = valid[choice];

        // Αφαίρεση της κάρτας από το χέρι
        if (current->playCard(playedCard))
        {

            // Μήνυμα απόκρουσης ποινής
            if (game.getPenaltyStack() > 0 && playedCard->getValue() == 7)
            {
                cout << current->getName() << " successfully bounces the penalty with another 7!\n";
            }

            // Τοποθέτηση κάρτας στο τραπέζι και καθαρισμός δηλωμένης φιγούρας
            game.getTable().addCard(playedCard);
            game.getTable().resetDeclaredSuit();

            // Εφαρμογή των effect των ειδικών καρτών
            CardEffect effect = playedCard->getEffect();
            if (effect == DRAW_TWO)
            {
                cout << current->getName() << " raised the penalty stack!\n";
                game.addPenaltyStack(2);
            }
            else if (effect == PLAY_AGAIN)
            {
                cout << current->getName() << " gets to play another card!!\n";
                game.setMustPlayAgain(true);
            }
            else if (effect == SKIP_NEXT)
            {
                cout << "Nine played! Next player loses their turn.\n";
                game.setSkipNext(true);
            }
            else if (effect == CHANGE_SUIT)
            {
                CardSuit newSuit = current->chooseSuit();
                game.getTable().setDeclaredSuit(newSuit);
                cout << current->getName() << " declares suit: " << suitToString(newSuit) << "\n";
            }

            // Έλεγχος τέλους γύρου (ο παίκτης έμεινε από κάρτες)
            if (!current->hasCards())
            {
                cout << current->getName() << " has no more cards. Round ends!\n";
                gameRunning = false;
            }

            turnEnded = true;
        }
        else
        {
            cout << "Error: Could not play the card. Please try again.\n";
        }
    }
    else
    {
        cout << "Invalid choice. Please pick a number from 0 to " << validCount - 1 << " or -1.\n";
    }
}

/*
void handleChoice(Game &g, Player *current, Card **validCards, int validCount, int choice, bool &turnEnded,
                  bool &gameRunning)
{
    string playerName = current->getName();

    if (choice == -1)
    {
        if (!current->getHasDrawnCard() && g.getPenaltyStack() == 0)
        {
            cout << playerName << " draws 1 card(s).\n";
            g.replenishDeckIfNeeded(1);
            current->drawCard(g.getDeck().deal());
        }
        else if (!current->getHasDrawnCard() && g.getPenaltyStack() > 0)
        {
            g.applyPenalty(current);
        }
        else if (current->getHasDrawnCard())
        {
            current->pass();
            turnEnded = true;
            cout << playerName << " passes.\n";
        }
    }
    else if (choice >= 0 && choice < validCount)
    {
        Card *chosenCard = validCards[choice];

        if (current->playCard(chosenCard))
        {
            if (g.getPenaltyStack() > 0 && chosenCard->getValue() == 7)
            {
                cout << playerName << " successfully bounces the penalty with another 7!\n";
            }
            g.getTable().addCard(chosenCard);
            g.getTable().resetDeclaredSuit();

            switch (chosenCard->getEffect())
            {
            case DRAW_TWO:
                g.addPenaltyStack(2);
                cout << playerName << " raised the penalty stack!\n";
                break;
            case PLAY_AGAIN:
                g.setMustPlayAgain(true);
                cout << playerName << " gets to play another card!\n";
                break;
            case SKIP_NEXT:
                g.setSkipNext(true);
                cout << "Nine played! Next player loses their turn.\n";
                break;
            case CHANGE_SUIT: {
                CardSuit newSuit = current->chooseSuit();
                g.getTable().setDeclaredSuit(newSuit);
                cout << playerName << " declares suit: " << suitToString(newSuit) << "\n";
            }
            break;
            case NONE:
            default:
                break;
            }

            if (current->getHand().numberOfCards() == 0)
            {
                cout << playerName << " has no more cards. Round ends!\n";
                gameRunning = false;
            }
            turnEnded = true;
        }
        else
        {
            cout << "Error: Could not play the card. Please try again.\n";
        }
    }
    else
    {
        cout << "Invalid choice. Please pick a number from 0 to " << validCount - 1 << " or -1.\n";
    }
}
*/
