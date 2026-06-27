// =============================================================================
// Hand.h
// -----------------------------------------------------------------------------
// Declares the Hand class, which represents the set of cards held by one player.
//
// Ownership note: Hand does NOT own the Card objects — it only stores pointers.
// Cards are created and destroyed by the Deck.
// When clear() is called, the card pointers are discarded but the Card objects
// themselves are NOT deleted.
// =============================================================================

#ifndef HAND_H
#define HAND_H

#include "Card.h"

// =============================================================================
// Hand – A player's set of held cards
// =============================================================================
class Hand {
private:
    Card** cards;     // Array of pointers to Card objects (NOT owned by Hand)
    int currentSize;  // Number of cards currently in the hand
    int capacity;     // Maximum number of cards the hand can hold (52)

public:
    // Constructor: allocates the cards array
    Hand();

    // Destructor: deletes the array itself (NOT the Card objects — Deck owns those)
    ~Hand();

    // getCards(): returns the raw array of card pointers
    // Useful for iterating over the hand
    Card** getCards() const;

    // numberOfCards(): returns how many cards are currently in the hand
    int numberOfCards() const;

    // validRoundCards(): determines which cards in this hand can legally be played
    // given the current top card on the table.
    //
    // Playability rules:
    //   1. Aces can always be played (except on top of another Ace)
    //   2. If a suit was declared (after an Ace), only cards of that suit are valid
    //   3. Otherwise, cards matching the top card's suit OR value are valid
    //   4. Special cards CANNOT be played if they are the player's last card
    //
    // Parameters:
    //   topCard        – the current top card on the table
    //   hasDeclaredSuit – true if an Ace was played and a suit was declared
    //   declaredSuit   – the suit that was declared (only used if hasDeclaredSuit is true)
    //   outCount       – OUTPUT: set to the number of valid cards found
    //
    // Returns: a newly allocated array of valid Card pointers.
    // IMPORTANT: The caller is responsible for calling delete[] on the returned array.
    Card** validRoundCards(Card* topCard, bool hasDeclaredSuit, CardSuit declaredSuit, int& outCount) const;

    // calculatePoints(): sums the penalty points of all cards in the hand.
    // Used at the end of a round to compute each player's score.
    int calculatePoints() const;

    // addCard(): adds a card to the end of the hand
    void addCard(Card* card);

    // removeCard(): finds and removes a specific card from the hand by pointer.
    // Shifts remaining cards left to fill the gap.
    // Returns true if the card was found and removed, false otherwise.
    bool removeCard(Card* card);

    // clear(): resets the hand to empty (currentSize = 0).
    // Does NOT delete the Card objects — those belong to the Deck.
    void clear();
};

#endif // HAND_H
