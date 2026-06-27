// =============================================================================
// InputUtils.h
// -----------------------------------------------------------------------------
// Declares a utility function for safe integer input from the user.
//
// NOTE: This function is not used in the current auto-play version of main.cpp
// (where all decisions are made randomly via rand()). It is provided here for
// use if you want to add real human input to the game.
// =============================================================================

#ifndef INPUTUTILS_H
#define INPUTUTILS_H

// readIntInput(): reads a single integer from standard input (cin).
// Returns true if the input was a valid integer with nothing extra on the line.
// Returns false and prints an error message if the input was invalid.
//
// Usage example:
//   int choice;
//   if (readIntInput(choice)) {
//       // use choice
//   }
bool readIntInput(int& value);

#endif // INPUTUTILS_H
