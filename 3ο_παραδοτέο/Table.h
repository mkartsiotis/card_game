// =============================================================================
// Table.h
// -----------------------------------------------------------------------------
// Declares the Table class, which represents the shared discard pile at the
// center of the game.
//
// All players play cards onto the Table. The top card of the pile determines
// what cards are legal to play next.
//
// Special case – Declared Suit:
//   When a player plays an Ace, they declare any suit they choose.
//   The Table stores this declared suit and signals to the Hand's
//   validRoundCards() that only that suit is acceptable (until the next
//   non-Ace card is played and the declared suit is reset).
//
// Ownership note: Table does NOT own the Card objects — it only stores pointers.
// Cards are created and destroyed by the Deck.
// =============================================================================

#ifndef TABLE_H
#define TABLE_H

#include "Card.h"

// =============================================================================
// Table – The shared discard pile
// =============================================================================
class Table {
private:
    Card** pile;          // Array of pointers to cards on the table (NOT owned)
    int pileSize;         // Number of cards currently on the table
    int capacity;         // Maximum pile size (52)

    bool hasDeclaredSuit; // True when the last Ace played declared a suit
    CardSuit declaredSuit; // The suit declared by the most recent Ace (only valid if hasDeclaredSuit == true)

public:
    // Constructor: allocates the pile array, initializes state
    Table();

    // Destructor: deletes the array (NOT the Card objects — Deck owns those)
    ~Table();

    // --- Declared Suit ---

    // Returns true if a suit is currently declared (i.e., an Ace was just played)
    bool getHasDeclaredSuit() const;

    // Returns the currently declared suit (only meaningful if hasDeclaredSuit is true)
    CardSuit getDeclaredSuit() const;

    // setDeclaredSuit(): records the suit chosen after an Ace is played
    void setDeclaredSuit(CardSuit suit);

    // resetDeclaredSuit(): clears the declared suit (called when any non-Ace card is played)
    void resetDeclaredSuit();

    // --- Pile access ---

    // getPile(): returns the raw pile array (used by Game to rebuild the deck)
    Card** getPile() const;

    // getPileSize(): returns how many cards are on the table
    int getPileSize() const;

    // clearPile(): resets the pile to empty.
    // Does NOT delete Card objects — those belong to the Deck.
    void clearPile();

    // addCard(): places a card on top of the pile
    void addCard(Card* card);

    // topCard(): returns the top card of the pile, or nullptr if the pile is empty
    Card* topCard() const;

    // isEmpty(): returns true if no cards are on the table
    bool isEmpty() const;
};

#endif // TABLE_H
