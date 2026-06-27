// =============================================================================
// Deck.h
// -----------------------------------------------------------------------------
// Declares the Deck class, which represents the draw pile of cards.
//
// The Deck is the sole OWNER of all Card objects it contains.
// It creates them with `new` and destroys them with `delete`.
// Hand and Table only borrow pointers — they never delete cards.
//
// A standard deck has 52 cards: 4 suits x 13 values (Ace through King).
// =============================================================================

#ifndef DECK_H
#define DECK_H

#include "Card.h"

// =============================================================================
// Deck – The draw pile
// =============================================================================
class Deck
{
  private:
    Card **cards;    // Dynamic array of pointers to Card objects (owned by Deck)
    int currentSize; // How many cards are currently in the deck
    int capacity;    // Maximum number of cards (52)

  public:
    // Constructor: allocates the card array and builds all 52 cards
    Deck();

    // Destructor: deletes all remaining Card objects and the array itself
    ~Deck();

    // deal(): removes and returns the top card of the deck.
    // Returns nullptr if the deck is empty.
    // The caller takes ownership of the returned Card pointer.
    Card *deal();

    // shuffle(): randomly reorders the cards currently in the deck.
    // Uses the Fisher-Yates algorithm.
    void shuffle();

    // reset(): deletes all current cards and rebuilds a fresh 52-card deck.
    // Used at the start of each round.
    void reset();

    // rebuildFromDiscard(): takes cards from the discard pile (excluding the top card)
    // and adds them back into the deck, then shuffles.
    // Used when the deck runs out of cards mid-round.
    // Parameters:
    //   discardPile – pointer array of cards on the table
    //   discardSize – total number of cards on the table
    void rebuildFromDiscard(Card **discardPile, int discardSize);

    // isEmpty(): returns true if there are no cards left in the deck
    bool isEmpty() const;

    // getCurrentSize(): returns how many cards are currently in the deck
    int getCurrentSize() const;
};

#endif // DECK_H
