# "Agonia" Card Game  
This is a cpp terminal based game that is based around a university assignment.  
It will however be developed independently after all and integrate some very interesting mathematical models that would in theory be able to beat the player most of the time based only on the bot player's data.  
## Rules of the game:  
### Setup:
Each player is dealt 7 cards, and one card is placed face-up to start the discard pile.
### Gameplay:
Players must match the face-up card's number or suit, or play a special card.
### Drawing Cards:
If a player cannot play, they must draw one card from the deck. If it is playable, they may play it immediately, otherwise they pass.
### Special Cards (Common Variations):
**7**: The next player must draw two cards. If the next player has a 7, they can play it, forcing the following player to draw four cards, and so on.  
Ace: Allows the player to change the active suit.  
**8**: Skips the next player's turn.  
**9**: Reverses the order of play.  
### Ending the Game:
-The first person to empty their hand wins the round.  
-As per the instructions we were requested to integrate a point grading system at the end of each round.  
So at the end of each round the we count the players cards and assign a score as seen in the scoriung system.  
### Scoring system:
-All the numbered cards contribute their number.  
-All the non-numbered special cards contribute 10 points.
-The Ace contributes 11 points
### Winning:
At the begginning of the game a point threshold is defined.  
So after N rounds when one player has accumulated at least the minimum theshold the players compare their scores.  
The player with the lowest total score accross rounds wins!  

## IMPLEMENTATION:
### Class definitions and design choices:  
The developers of the program have agreed to create the following 4 class entities:   
**Deck**: The deck is the pile that holds all the shuffled cards that have yet to be drawn by a player.  
**Card**: The card entity this holds information about a card.  
**Game engine**:This class does all the heavy lifting! It calls the player to play, checks card validity executes special card actions and keeps track of the game in general(for further info on how the game engine connects to the whole system check the class UML diagramm).  
**Player**:This holds the info about the player and specifically their name, id, accumulative score and of cource the cards that they have in hand.  


## Future ideas...(bucket list)
1. Investigate mathematical models that could create a bot.(Markov chains, graph theory even using A*+heuristics!)    
2. Train a NN on player data and deploy it so that the Bot can mimic human psychological and behavioural characteristics.
3. Make graphics with raylib or a graph library.  
4. Create game-analytics so that the player can recognize their mistakes.  
**Warning: This project is still on its early development days. As you can notice no code has been written yet since we are on the critical design and UML phases of the project. As a developer and as a team and a community of devs that maybe executing the same project, but are collaborating extensively for the greater good, any notice-issue would be greatly appreciated!**  
Thanks!   


