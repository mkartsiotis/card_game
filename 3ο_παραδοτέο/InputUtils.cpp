// =============================================================================
// InputUtils.cpp
// -----------------------------------------------------------------------------
// Implements safe integer input reading from the console.
//
// This is a utility used for interactive (human) play. In the current
// auto-play version of main.cpp, decisions are made via rand() instead.
// =============================================================================

#include "InputUtils.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// ---------------------------------------------------------------------------
// readIntInput
// ---------------------------------------------------------------------------
// Reads a full line from standard input and attempts to parse exactly one
// integer from it, with no extra characters.
//
// Why read a full line instead of just `cin >> value`?
//   Using cin >> directly can leave leftover characters in the input buffer,
//   causing problems on the next read. Reading a full line and then parsing
//   it with istringstream is safer and more predictable.
//
// Parameters:
//   value – OUTPUT: the parsed integer (only set if the function returns true)
//
// Returns:
//   true  – input was a valid integer (and nothing else)
//   false – input was invalid; an error message is printed
bool readIntInput(int& value) {
    string inputLine;
    getline(cin >> ws, inputLine);  // Read a full line (skipping leading whitespace)

    istringstream iss(inputLine);
    char extra = '\0';

    // Try to parse an integer, then check there are no extra characters after it
    if ((iss >> value) && !(iss >> extra)) {
        return true;  // Successfully parsed exactly one integer
    }

    cout << "Invalid input. Please enter a valid number.\n";
    return false;
}
