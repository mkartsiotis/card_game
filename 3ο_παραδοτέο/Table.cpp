// =============================================================================
// Table.cpp
// -----------------------------------------------------------------------------
// Implements the Table class: managing the discard pile and the declared suit
// state that results from an Ace being played.
// =============================================================================

#include "Table.h"

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
// Allocates the pile array and sets initial state.
// No suit is declared at the start; the pile is empty.
Table::Table() {
    capacity        = 52;
    pileSize        = 0;
    pile            = new Card*[capacity];
    hasDeclaredSuit = false;
    declaredSuit    = HEART;  // Default value; only meaningful when hasDeclaredSuit is true
}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
// Deletes the pile array, but NOT the Card objects inside it.
// Cards are owned by the Deck.
Table::~Table() {
    delete[] pile;
}


// ---------------------------------------------------------------------------
// Declared Suit
// ---------------------------------------------------------------------------

// Returns true if a suit is currently in effect (i.e., an Ace was just played)
bool Table::getHasDeclaredSuit() const {
    return hasDeclaredSuit;
}

// Returns the declared suit (only valid when hasDeclaredSuit == true)
CardSuit Table::getDeclaredSuit() const {
    return declaredSuit;
}

// Records the suit chosen by a player after playing an Ace
void Table::setDeclaredSuit(CardSuit suit) {
    declaredSuit    = suit;
    hasDeclaredSuit = true;
}

// Clears the declared suit (called when any non-Ace card is played)
void Table::resetDeclaredSuit() {
    hasDeclaredSuit = false;
}


// ---------------------------------------------------------------------------
// Pile access
// ---------------------------------------------------------------------------

// Returns the raw pile array (used by Game::replenishDeckIfNeeded)
Card** Table::getPile() const {
    return pile;
}

// Returns the number of cards currently on the table
int Table::getPileSize() const {
    return pileSize;
}

// Resets the pile to empty without deleting card objects.
// Called when the discard pile is recycled back into the deck.
void Table::clearPile() {
    pileSize = 0;
}

// Returns true if no cards are on the table yet
bool Table::isEmpty() const {
    return pileSize == 0;
}

// Adds a card to the top of the pile
void Table::addCard(Card* card) {
    if (card && pileSize < capacity) {
        pile[pileSize++] = card;
    }
}

// Returns the top card of the pile (the card currently in play),
// or nullptr if the pile is empty
Card* Table::topCard() const {
    if (pileSize > 0) return pile[pileSize - 1];
    return nullptr;
}
