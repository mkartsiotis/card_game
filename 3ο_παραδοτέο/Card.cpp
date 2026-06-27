// =============================================================================
// Card.cpp
// -----------------------------------------------------------------------------
// Implements the Card abstract base class and all its concrete subclasses:
//   Simple, Seven, Eight, Nine, Ace
//
// Also implements the suitToString() helper function.
// =============================================================================

#include "Card.h"
#include <iostream>

// ---------------------------------------------------------------------------
// suitToString
// Converts a CardSuit enum value to a human-readable string.
// Used when printing card information to the console.
// ---------------------------------------------------------------------------
string suitToString(CardSuit suit) {
    switch (suit) {
        case HEART:   return "Heart";
        case DIAMOND: return "Diamond";
        case CLUB:    return "Club";
        case SPADE:   return "Spade";
        default:      return "Unknown";
    }
}


// =============================================================================
// Card – Base Class Implementation
// =============================================================================

// Constructor: initializes the card's suit, value, and point cost.
// Called from every subclass constructor via the initializer list.
Card::Card(CardSuit s, int v, int p) {
    suit   = s;
    value  = v;
    points = p;
}

// Default destructor (virtual, so polymorphic deletion works correctly)
Card::~Card() = default;

// Returns the suit of this card
CardSuit Card::getSuit() const {
    return suit;
}

// Returns the numeric value of this card (1 = Ace, 11 = Jack, etc.)
int Card::getValue() const {
    return value;
}

// Returns the penalty points this card adds to a player's score
// if it is still in their hand at the end of a round
int Card::getPoints() const {
    return points;
}


// =============================================================================
// Simple – Normal card, no special effect
// =============================================================================

Simple::Simple(CardSuit s, int v, int p) : Card(s, v, p) {}

// Simple cards have no effect
CardEffect Simple::getEffect() const {
    return NONE;
}

// Simple cards are not special
bool Simple::isSpecial() const {
    return false;
}


// =============================================================================
// Seven – Draw-two card
// =============================================================================

Seven::Seven(CardSuit s, int v, int p) : Card(s, v, p) {}

// Playing a 7 forces the next player to draw 2 cards (or stack another 7)
CardEffect Seven::getEffect() const {
    return DRAW_TWO;
}

bool Seven::isSpecial() const {
    return true;
}


// =============================================================================
// Eight – Play-again card
// =============================================================================

Eight::Eight(CardSuit s, int v, int p) : Card(s, v, p) {}

// Playing an 8 gives the current player an extra turn
CardEffect Eight::getEffect() const {
    return PLAY_AGAIN;
}

bool Eight::isSpecial() const {
    return true;
}


// =============================================================================
// Nine – Skip card
// =============================================================================

Nine::Nine(CardSuit s, int v, int p) : Card(s, v, p) {}

// Playing a 9 causes the next player to miss their turn
CardEffect Nine::getEffect() const {
    return SKIP_NEXT;
}

bool Nine::isSpecial() const {
    return true;
}


// =============================================================================
// Ace – Suit-change card
// =============================================================================

Ace::Ace(CardSuit s, int v, int p) : Card(s, v, p) {}

// Playing an Ace lets the current player declare any suit
CardEffect Ace::getEffect() const {
    return CHANGE_SUIT;
}

bool Ace::isSpecial() const {
    return true;
}
