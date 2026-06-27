// =============================================================================
// Card.h
// -----------------------------------------------------------------------------
// Defines the Card class hierarchy for the Agonia card game.
//
// Card is an abstract base class. Each concrete subclass represents a specific
// type of card (by value) and defines the effect that card has when played.
//
// Class hierarchy:
//   Card  (abstract)
//    ├── Simple  – normal cards (2-6, 10, J, Q, K) → no special effect
//    ├── Seven   – draw-two card → next player must draw 2 (or stack another 7)
//    ├── Eight   – play-again card → current player gets an extra turn
//    ├── Nine    – skip card → next player loses their turn
//    └── Ace     – suit-change card → current player declares any suit
// =============================================================================

#ifndef CARD_H
#define CARD_H

#include <string>
using namespace std;

// ---------------------------------------------------------------------------
// CardSuit: the four suits of a standard deck
// ---------------------------------------------------------------------------
enum CardSuit {
    HEART,    // ♥
    DIAMOND,  // ♦
    CLUB,     // ♣
    SPADE     // ♠
};

// ---------------------------------------------------------------------------
// CardEffect: the special effect a card triggers when played
// ---------------------------------------------------------------------------
enum CardEffect {
    NONE,         // No special effect (Simple cards)
    DRAW_TWO,     // Next player must draw 2 cards (Seven)
    PLAY_AGAIN,   // Current player plays another card immediately (Eight)
    SKIP_NEXT,    // Next player misses their turn (Nine)
    CHANGE_SUIT   // Current player declares a new suit (Ace)
};

// ---------------------------------------------------------------------------
// suitToString: converts a CardSuit enum value to a human-readable string.
// Defined in Card.cpp. Declared here so any file that includes Card.h can use it.
// ---------------------------------------------------------------------------
string suitToString(CardSuit suit);


// =============================================================================
// Card – Abstract Base Class
// =============================================================================
// Every card in the deck is a Card. Subclasses must implement:
//   - getEffect()  → what happens when this card is played
//   - isSpecial()  → whether this card has a non-NONE effect
//
// Ownership note: Card objects are created by the Deck and deleted by the Deck.
// Hand and Table only store pointers — they do NOT delete cards.
// =============================================================================
class Card {
private:
    CardSuit suit;   // The suit of this card (HEART, DIAMOND, CLUB, SPADE)
    int value;       // Numeric value: 1 (Ace) through 13 (King)
    int points;      // Penalty points this card contributes at end of round

public:
    // Constructor: called by every subclass constructor via the initializer list
    Card(CardSuit s, int v, int p);

    // Virtual destructor: needed so that deleting a Card* calls the right destructor
    virtual ~Card();

    // --- Getters ---
    CardSuit getSuit() const;
    int getValue() const;
    int getPoints() const;

    // --- Pure Virtual Methods (must be overridden by every subclass) ---

    // Returns the effect this card triggers when played
    virtual CardEffect getEffect() const = 0;

    // Returns true if this card has a special effect (i.e., effect != NONE)
    virtual bool isSpecial() const = 0;
};


// =============================================================================
// Simple – A normal card with no special effect
// Values: 2, 3, 4, 5, 6, 10, Jack (11), Queen (12), King (13)
// Effect: NONE
// =============================================================================
class Simple : public Card {
public:
    Simple(CardSuit s, int v, int p);
    CardEffect getEffect() const override;
    bool isSpecial() const override;
};


// =============================================================================
// Seven – Forces the next player to draw 2 cards (or stack another 7)
// Value: 7
// Effect: DRAW_TWO
// =============================================================================
class Seven : public Card {
public:
    Seven(CardSuit s, int v, int p);
    CardEffect getEffect() const override;
    bool isSpecial() const override;
};


// =============================================================================
// Eight – Gives the current player an extra turn
// Value: 8
// Effect: PLAY_AGAIN
// =============================================================================
class Eight : public Card {
public:
    Eight(CardSuit s, int v, int p);
    CardEffect getEffect() const override;
    bool isSpecial() const override;
};


// =============================================================================
// Nine – Skips the next player's turn
// Value: 9
// Effect: SKIP_NEXT
// =============================================================================
class Nine : public Card {
public:
    Nine(CardSuit s, int v, int p);
    CardEffect getEffect() const override;
    bool isSpecial() const override;
};


// =============================================================================
// Ace – Lets the current player declare any suit
// Value: 1
// Effect: CHANGE_SUIT
// =============================================================================
class Ace : public Card {
public:
    Ace(CardSuit s, int v, int p);
    CardEffect getEffect() const override;
    bool isSpecial() const override;
};

#endif // CARD_H
