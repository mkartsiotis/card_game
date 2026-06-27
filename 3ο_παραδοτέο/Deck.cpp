// =============================================================================
// Deck.cpp
// -----------------------------------------------------------------------------
// Implements the Deck class: construction, card dealing, shuffling, and
// rebuilding the deck from the discard pile.
// =============================================================================

#include "Deck.h"
#include "Card.h"
#include <cstdlib>
// ---------------rand------------------------------------------------------------
// shuffle
// ---------------------------------------------------------------------------
// Randomly reorders the cards in the deck using the Fisher-Yates algorithm.
//
// How Fisher-Yates works:
//   For each position i from the end down to 1:
//     Pick a random position j between 0 and i (inclusive)
//     Swap cards[i] and cards[j]
// This guarantees every possible ordering is equally likely.
void Deck::shuffle()
{
    for (int i = currentSize - 1; i > 0; i--)
    {
        // Pick a random index between 0 and i (inclusive)
        int j = rand() % (i + 1);

        // Swap the cards at positions i and j
        Card *temp = cards[i];
        cards[i] = cards[j];
        cards[j] = temp;
    }
}

// ---------------------------------------------------------------------------
// isEmpty
// ---------------------------------------------------------------------------
bool Deck::isEmpty() const
{
    return currentSize == 0;
}

// ---------------------------------------------------------------------------
// getCurrentSize
// ---------------------------------------------------------------------------
int Deck::getCurrentSize() const
{
    return currentSize;
}

Deck::Deck() : currentSize(0), capacity(52)
{
    cards = new Card *[capacity];
    for (int i = 0; i < capacity; i++)
    {
        cards[i] = nullptr;
    }
    for (int i = 0; i < 4; i++)
    {
        CardSuit currentSuit = static_cast<CardSuit>(i);
        cards[(i * 13)] = new Ace(currentSuit, 1, 11);
        for (int k = 2; k <= 6; k++)
            cards[(i * 13) + (k - 1)] = new Simple(currentSuit, k, k);
        cards[(i * 13) + 6] = new Seven(currentSuit, 7, 7);
        cards[(13 * i) + 7] = new Eight(currentSuit, 8, 8);
        cards[(13 * i) + 8] = new Nine(currentSuit, 9, 9);
        for (int k = 10; k <= 13; k++)
            cards[(i * 13) + (k - 1)] = new Simple(currentSuit, k, 10);
    }
    currentSize = 52;
}

Deck::~Deck()
{
    for (int i = 0; i < 52; i++)
        delete cards[i];
    delete[] cards;
}

void Deck::reset()
{
    for (int i = 0; i < 52; i++)
        delete cards[i];
    delete[] cards;
    cards = new Card *[capacity];
    currentSize = 52;
    for (int i = 0; i < 4; i++)
    {
        CardSuit currentSuit = static_cast<CardSuit>(i);
        cards[(i * 13)] = new Ace(currentSuit, 1, 11);
        for (int k = 2; k <= 6; k++)
            cards[(i * 13) + (k - 1)] = new Simple(currentSuit, k, k);
        cards[(i * 13) + 6] = new Seven(currentSuit, 7, 10);
        cards[(13 * i) + 7] = new Eight(currentSuit, 8, 10);
        cards[(13 * i) + 8] = new Nine(currentSuit, 9, 10);
        for (int k = 10; k <= 13; k++)
            cards[(i * 13) + (k - 1)] = new Simple(currentSuit, k, 10);
    }
}

Card *Deck::deal()
{
    if (this->isEmpty() == true)
        return nullptr;
    else
    {
        Card *returnCard = cards[(currentSize - 1)];
        cards[(currentSize - 1)] = nullptr;
        currentSize--;
        return returnCard;
    }
}

void Deck::rebuildFromDiscard(Card **discardPile, int discardSize)
{
    for (int i = 0; i < discardSize - 1; i++)
        cards[i] = discardPile[i];
    this->currentSize = discardSize - 1;
    this->shuffle();
}
