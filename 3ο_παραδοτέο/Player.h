// =============================================================================
// Player.h
// -----------------------------------------------------------------------------
// Declares the Player class, which represents one participant in the game.
//
// Each Player has:
//   - A name and index (their position at the table)
//   - A Hand (the cards they currently hold)
//   - A status (what happened on their last turn)
//   - A cumulative points total (penalty points across all rounds)
//   - A flag tracking whether they have already drawn a card this turn
// =============================================================================

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
using namespace std;

#include "Hand.h"
#include "Card.h"

// ---------------------------------------------------------------------------
// PlayerStatus: describes what a player did on their most recent turn.
// Used by the Game to track turn flow and decide when a round ends.
// ---------------------------------------------------------------------------
enum PlayerStatus {
    HASNTPLAYED,     // Player has not yet taken a turn in this orbit
    PASSED,          // Player drew a card but had nothing to play, so they passed
    PLAYED,          // Player successfully played a card
    HASNOCARDSLEFT,  // Player emptied their hand (wins the round)
    WONTHETRICK,     // Player won the trick (reserved for future use)
    MISSEDTURN       // Player's turn was skipped by a Nine card
};

// statusToString: converts a PlayerStatus to a human-readable string
string statusToString(PlayerStatus status);


// =============================================================================
// Player – A game participant
// =============================================================================
class Player {
private:
    string name;         // Display name (e.g., "Alice")
    PlayerStatus status; // What happened on this player's last turn
    int points;          // Cumulative penalty points across all rounds (lower is better)
    bool hasDrawnCard;   // True if the player has already drawn a card this turn
    Hand hand;           // The cards this player is currently holding

public:
    // Constructor: sets up the player with a name
    Player(string n);

    // --- Identity getter ---
    string getName() const;

    // --- Status ---
    PlayerStatus getStatus() const;
    void setStatus(PlayerStatus s);

    // --- Points ---
    int getPoints() const;
    void setPoints(int p);
    void addPoints(int p);   // Adds p to the current total (used at end of round)

    // --- Draw flag ---
    bool getHasDrawnCard() const;
    void setHasDrawnCard(bool b);

    // --- Hand access ---
    Hand& getHand();             // Non-const: allows modifying the hand
    const Hand& getHand() const; // Const: for read-only access

    // --- Actions ---

    // playCard(): removes the given card from the hand and updates status to PLAYED.
    // Returns true if the card was found and removed, false otherwise.
    bool playCard(Card* card);

    // drawCard(): adds the given card to the hand and sets hasDrawnCard = true.
    void drawCard(Card* card);

    // pass(): sets status to PASSED (player drew but could not play).
    void pass();

    // chooseSuit(): randomly selects and returns a CardSuit.
    // Used by the AI when an Ace is played (no human input in auto-play mode).
    CardSuit chooseSuit();

    // --- Utility ---

    // hasCards(): returns true if the player has at least one card in hand
    bool hasCards() const;

    // clearHand(): removes all cards from the hand (does NOT delete them)
    void clearHand();
};

#endif // PLAYER_H
