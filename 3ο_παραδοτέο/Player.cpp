// =============================================================================
// Player.cpp
// -----------------------------------------------------------------------------
// Implements the Player class: player actions, state management, and the
// statusToString helper.
// =============================================================================

#include "Player.h"
#include <cstdlib>   // for rand()

// ---------------------------------------------------------------------------
// statusToString
// ---------------------------------------------------------------------------
// Converts a PlayerStatus enum value to a human-readable string.
string statusToString(PlayerStatus status) {
    switch (status) {
        case HASNTPLAYED:    return "Hasn't Played";
        case PASSED:         return "Passed";
        case PLAYED:         return "Played";
        case HASNOCARDSLEFT: return "Has No Cards Left";
        case WONTHETRICK:    return "Won the Trick";
        case MISSEDTURN:     return "Missed Turn";
        default:             return "Unknown";
    }
}


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
// Initializes the player with a name.
// Status starts as HASNTPLAYED, points start at 0, hasDrawnCard starts false.
Player::Player(string n) {
    name         = move(n);   // move avoids an extra string copy
    status       = HASNTPLAYED;
    points       = 0;
    hasDrawnCard = false;
}


// ---------------------------------------------------------------------------
// Identity getter
// ---------------------------------------------------------------------------

string Player::getName() const {
    return name;
}


// ---------------------------------------------------------------------------
// Status
// ---------------------------------------------------------------------------

PlayerStatus Player::getStatus() const {
    return status;
}

void Player::setStatus(PlayerStatus s) {
    status = s;
}


// ---------------------------------------------------------------------------
// Points
// ---------------------------------------------------------------------------

int Player::getPoints() const {
    return points;
}

void Player::setPoints(int p) {
    points = p;
}

// Adds p penalty points to this player's running total
void Player::addPoints(int p) {
    points += p;
}


// ---------------------------------------------------------------------------
// Draw flag
// ---------------------------------------------------------------------------

bool Player::getHasDrawnCard() const {
    return hasDrawnCard;
}

void Player::setHasDrawnCard(bool b) {
    hasDrawnCard = b;
}


// ---------------------------------------------------------------------------
// Hand access
// ---------------------------------------------------------------------------

Hand& Player::getHand() {
    return hand;
}

const Hand& Player::getHand() const {
    return hand;
}


// ---------------------------------------------------------------------------
// hasCards
// ---------------------------------------------------------------------------
// Returns true if the player still has at least one card in their hand.
bool Player::hasCards() const {
    return hand.numberOfCards() > 0;
}


// ---------------------------------------------------------------------------
// clearHand
// ---------------------------------------------------------------------------
// Empties the hand without deleting the Card objects (Deck owns them).
void Player::clearHand() {
    hand.clear();
}


// ---------------------------------------------------------------------------
// playCard
// ---------------------------------------------------------------------------
// Attempts to remove `card` from the player's hand.
// If successful: sets status to PLAYED and resets hasDrawnCard.
// Returns true on success, false if the card was not found.
bool Player::playCard(Card* card) {
    if (hand.removeCard(card)) {
        status       = PLAYED;
        hasDrawnCard = false;
        return true;
    }
    return false;
}


// ---------------------------------------------------------------------------
// drawCard
// ---------------------------------------------------------------------------
// Adds `card` to the player's hand and marks that they have drawn this turn.
void Player::drawCard(Card* c) {
    if (c) {
        hand.addCard(c);
    }
    hasDrawnCard = true;
}


// ---------------------------------------------------------------------------
// pass
// ---------------------------------------------------------------------------
// Marks the player as having passed their turn (drew a card but could not play).
void Player::pass() {
    status       = PASSED;
    hasDrawnCard = false;
}


// ---------------------------------------------------------------------------
// chooseSuit
// ---------------------------------------------------------------------------
// Randomly picks one of the four suits and returns it.
// This is the AI behavior when an Ace is played (no human input in auto-play mode).
CardSuit Player::chooseSuit() {
    int choice = rand() % 4;
    return static_cast<CardSuit>(choice);
}
