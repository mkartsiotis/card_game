// =============================================================================
// Hand.cpp
// -----------------------------------------------------------------------------
// Implements the Hand class: managing a player's cards and determining
// which cards are legally playable on a given turn.
// =============================================================================

#include "Hand.h"

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
// Allocates the card pointer array. Capacity is set to 52 because
// in the worst case a player could hold the entire deck.
Hand::Hand() {
    currentSize = 0;
    capacity    = 52;
    cards       = new Card*[capacity];
}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
// Deletes the array of pointers, but NOT the Card objects themselves.
// Card objects are owned by the Deck and deleted there.
Hand::~Hand() {
    delete[] cards;
}

// ---------------------------------------------------------------------------
// getCards
// ---------------------------------------------------------------------------
// Returns the raw pointer to the internal card array.
Card** Hand::getCards() const {
    return cards;
}

// ---------------------------------------------------------------------------
// numberOfCards
// ---------------------------------------------------------------------------
int Hand::numberOfCards() const {
    return currentSize;
}

// ---------------------------------------------------------------------------
// validRoundCards
// ---------------------------------------------------------------------------
// Returns a newly allocated array of cards that can legally be played right now.
// The caller MUST call delete[] on the returned pointer when done with it.
//
// Playability rules (checked in order):
//   1. If there is no top card (empty table), all cards are playable.
//   2. Aces are always playable — UNLESS the top card is also an Ace.
//   3. If a suit was declared (after an Ace was played), only cards of that
//      declared suit are playable (Aces still bypass this).
//   4. Otherwise, a card is playable if it matches the top card's suit OR value.
//   5. Special rule: a player's LAST card cannot be a special card
//      (you cannot end the round by triggering an effect on other players).
Card** Hand::validRoundCards(Card* topCard, bool hasDeclaredSuit, CardSuit declaredSuit, int& outCount) const {
    // Allocate a result array large enough to hold all cards (worst case: all are valid)
    Card** validCards = new Card*[capacity];
    outCount = 0;

    for (int i = 0; i < currentSize; ++i) {
        Card* card = cards[i];
        bool isPlayable = false;

        if (!topCard) {
            // Rule 1: No card on the table yet — anything goes
            isPlayable = true;

        } else if (dynamic_cast<Ace*>(card) != nullptr) {
            // Rule 2: Aces can be played on anything EXCEPT another Ace
            if (dynamic_cast<Ace*>(topCard) == nullptr) {
                isPlayable = true;
            }

        } else if (hasDeclaredSuit && card->getSuit() == declaredSuit) {
            // Rule 3: A suit was declared — must match that suit
            isPlayable = true;

        } else if (!hasDeclaredSuit && card->getSuit() == topCard->getSuit()) {
            // Rule 4a: No declared suit — matching suit is always valid
            isPlayable = true;

        } else if (!hasDeclaredSuit && card->getValue() == topCard->getValue()) {
            // Rule 4b: No declared suit — matching value is also valid
            isPlayable = true;
        }

        // Rule 5: You cannot play a special card as your very last card
        // (it would trigger an effect on other players after the round ends)
        if (isPlayable && currentSize == 1 && card->isSpecial()) {
            isPlayable = false;
        }

        if (isPlayable) {
            validCards[outCount++] = card;
        }
    }

    return validCards;
}

// ---------------------------------------------------------------------------
// calculatePoints
// ---------------------------------------------------------------------------
// Sums the penalty points of all cards currently in the hand.
int Hand::calculatePoints() const {
    int total = 0;
    for (int i = 0; i < currentSize; ++i) {
        total += cards[i]->getPoints();
    }
    return total;
}

// ---------------------------------------------------------------------------
// addCard
// ---------------------------------------------------------------------------
// Appends a card to the end of the hand.
void Hand::addCard(Card* card) {
    if (card && currentSize < capacity) {
        cards[currentSize++] = card;
    }
}

// ---------------------------------------------------------------------------
// removeCard
// ---------------------------------------------------------------------------
// Finds the given card by pointer and removes it from the hand.
// Shifts all subsequent cards one position to the left to fill the gap.
// Returns true if found and removed, false if the card was not in this hand.
bool Hand::removeCard(Card* card) {
    for (int i = 0; i < currentSize; ++i) {
        if (cards[i] == card) {
            // Shift cards left to fill the removed slot
            for (int j = i; j < currentSize - 1; ++j) {
                cards[j] = cards[j + 1];
            }
            currentSize--;
            return true;
        }
    }
    return false;  // Card was not found in this hand
}

// ---------------------------------------------------------------------------
// clear
// ---------------------------------------------------------------------------
// Empties the hand by resetting the size counter.
// The Card objects themselves are NOT deleted — the Deck owns them.
void Hand::clear() {
    currentSize = 0;
}
