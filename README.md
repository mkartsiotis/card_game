# "Agonia" Card Game  
This is a cpp terminal based game that is based around a university assignment.  
It will however be developed independently after all and integrate some very interesting mathematical models that would in theory be able to beat the player most of the time based only on the bot player's data.  
![Playing_Cards_Image](https://github.com/mkartsiotis/card_game/blob/main/cards.jpeg)
## Contents: 
1. [Rules of the game](#rules-of-the-game)
2. [Class definitions and design choices](#class-definitions-and-design-choices)
3. [Card Encoding](#card-encoding)
4. [Design review and logic explanation](#design-review-and-logic-explanation)
5. [UML Relationships](#uml-relationships)
6. [Future ideas](#future-ideas...)

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
At the beginning of the game a point threshold is defined.  
So after N rounds when one player has accumulated at least the minimum threshold the players compare their scores.  
The player with the lowest total score across rounds wins!  

## IMPLEMENTATION:
### Class definitions and design choices
***Initial Thoughts***  
The developers of the program have agreed to create the following 4 class entities:   
**Deck**: The deck is the pile that holds all the shuffled cards that have yet to be drawn by a player.  
**Card**: The card entity this holds information about a card.  
**Game engine**:This class does all the heavy lifting! It calls the player to play, checks card validity executes special card actions and keeps track of the game in general(for further info on how the game engine connects to the whole system check the class UML diagramm).  
**Player**:This holds the info about the player and specifically their name, id, accumulative score and the cards that they have in hand.  
***Refined approach***  
If you have noticed one thing that would have certainly been the absurd amount of responsibilities of the game-engine class.  
Thus, that class will be split and the rule verification and scoring will be handled independently by the *Rulebook* and *Scoreboard* classes respectively as seen in the UML diagram.  
### Card Encoding
Card encoding will be done via global enumerations and specifically:  
`enum suit  
{ 
  Spades, Hearts, Diamonds, Clubs
};`  
`enum card  
{ 
  Ace, 2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K 
};`  
### Design review and logic explanation
Based on all the objectives set the following logic was set:  
#### How the game would proceed
1. At the beginning of the game the number of players is defined.  
2. A game-engine object is created and in its constructor an array of player objects is created.  
3. The user is called to enter the player names and those names are set on the player object names that make the player array  
4. Deck is initialized.  
5. Deck is shuffled.  
6. The main loop is initialized.  
7. Whose turn is it?This is decided by a counter variable inside the game-engine.  
8. Then the player that matches the player id is called to play.  
9. When the player decides the move a validity check is implemented.  
10. If the move is valid then the player card is removed from the player's card array and is transferred to the discard pile array in the game engine.  
    Else the move is rejected and the player is called to play again.  
11. Then the next player is called and so on.  
***We also need to investigate the following scenarios***  
>*What happens if a player drops a special card(7, 8, 9, ACE)?*  
- At that case the penalty point integer counter of the game engine comes in hand. So if a player drops a 7 that int becomes 2, then with the method check_and_penalize the system asks the next player: Do you have a 7?
If the player decides to play(say yes) then the system automatically checks their cards, finds if there is a 7 and then if there is executes the move as clarified above.  
If not then the next player receives the 2 cards(drawn from the pile of the deck), penalty int is set back to 0 and then the player is asked to play.
- If the special card is 8 then the simply the counter is not updated at the end of the game loop and thus the same player is asked to play.  
- Similarly if the last played card is 9 then the index is updated by +2.  
- If the special card is the ACE then the player needs to decide the next suit. So the player is asked to select the suit and simply the proposed suit is set as a game-engine rule parameter.  
>***There is also another thing that needs to be addressed and that is scoring!***  
- For scoring each player has an accumulative score parameter. At the end of each round the player cards are counted and based on that score is assigned and updated.  
It is also evident that this significantly transforms the game over logic.
>***Game over logic***  
- When does the game end? When the accumulative score one player surpasses the threshold set at the beginning of the game.  
So we need two functions.  
A round_over function and a game_over function.  
The first will access the players' cards and see if any player has a zero card vector length at the end of each card play.  
The second will be activated at the end of each round and check the players' accumulative score. If the score surpasses the threshold then it is GAME OVER.  

## UML Relationships
This is arguably the most complicated things since they will determine the projects architecture.  
The Relationships have been determined based on the following thoughts:  
1. The main entity that handles everything is the game engine.  
Thus the game engine should have access to the players, cards, deck.  
2. Each player holds every time a number of cards.  
The player *OWNS* the cards. So their relationship is at least *aggregation*.  
Is it *composition*?  
To determine the answer to that question we need to think if the cards are owned just by the player(so they are objects of the player) or they can exist by their own and the player just holds those cards.  
It could be interpreted both ways but for this game the stronger relationship of composition was selected since the player needs to take the decision and no extra information is stored in the game engine apart from the cards that have already been played.  
The same goes for the deck-card relationship.  
The game-engine-card relationship is weaker since the cards do not "live" into the game-engine they are just held there temporarily.  
3. We need to ask ourselves another question. *Can there be a player without a game-engine?*  
No! So the relationship of the game-engine and the player is *composition*.
4. The rulebook and the scoreboard classes are owned by the game-engine class and cannot exist independently. Therefore, their relationship with the game-engine is composition.  

## Development Schedule:  
1. Finish UML class diagrams by Friday.  
2. Get started with code on Saturday.  
3. Finish basic concept coding before the end of next week.  
4. Full stack production release after Easter.  
5. Time permitting mathematical features will be added along the way.  
6. Then the training of the NN and the gathering of the data.  
Note that all of these are still conceptual and release times may vary depending on schedule and workload fluctuation.  
This is not the only ongoing project after all(Assembly - Verilog - TSNE embeddings and vector databases)  


## Future ideas...
1. Investigate mathematical models that could create a bot.(Markov chains, graph theory even using A*+heuristics!)    
2. Train a NN on player data and deploy it so that the Bot can mimic human psychological and behavioural characteristics.
3. Make graphics with raylib or a graph library.  
4. Create game-analytics so that the player can recognize their mistakes.  
**Warning: This project is still on its early development days. As you can notice no code has been written yet since we are on the critical design and UML phases of the project. As a developer and as a team and a community of devs that maybe executing the same project, but are collaborating extensively for the greater good, any notice-issue would be greatly appreciated!**  
Thanks!  
