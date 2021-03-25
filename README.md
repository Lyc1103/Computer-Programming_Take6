# Take6 - Written In C Language

> Author : Ya Chen <br>
> Date : 2021 / 3 / 25 <br>
> List
>
> > <a href = "#description">Description</a><br> > > <a href = "#practicing">What I'm Practicing</a><br> > > <a href = "#rules">Take 6 Rules</a><br> > > <a href = "#reference">Reference</a><br>

<br>
<div id = "description">

## Description

<b>Take 6!</b> is a card game for 2-10 players designed by Wolfgang Kramer in 1994 and published by Amigo Spiele. In Taiwan, it has another famous name called 誰是⽜頭王. This game received the Deutscher Spiele Preis award in 1994. Now I want you to develop this game as your final project.
For your reference, you can find its rule in the following links:

1. https://en.wikipedia.org/wiki/6_Nimmt!
2. http://www.swanpanasia.com/products/take-6

</div>
<br>
<br>
<div id = "practicing">

## What I'm Practicing

Design a personal game.<br>
I want to design my own interface. The minimum requirement is a terminal-based game. Of course, if I want to use other libraries, like SDL, to develop a game with a graphical interface, it will be much better (, but I don't ☹ ). Since this is a personal game, it need to develop some computer agents to play with the human player.<br>

It should contains...

- Support 1 player and 1 computer agents
- Support multiple computer agents
  1
- Support computer agents with more than 1 level
- Interface design
- Any other interesting features

</div>
<br>
<br>
<div id = "rules">

## Take 6 Rules

In this game, there are 104 numbered cards from 1 to 104, and on each turn, players choose the most appropriate numbered card from their hand without knowing the numbered cards of other players. <br>
When all players have chosen their numbered cards for the round, the players open their cards at the same time. The game is played with four rows of cards, and each player will place the lowest number in all four rows of cards after each turn. Provided that the last card in each row is the last, the player must place the card he has played at the end of the row closest to the number but less than the number he has played. <br>
If the card is the sixth card in a row, the player must take the first five cards. If the player's number is less than the last number in any row, the player must choose a row and take all the numbers in that row. <br>
When all the cards in the hand are used up, each player adds up the penalty points ( the number of bulls on each card ) from the cards he took back, and the winner is the one with the least.

</div>
<br>
<br>
<div id = "reference">

## Reference : <a href = "https://drive.google.com/file/d/1VGUcrUOkHtLXIPLSu85LsNB6G2-sJMAH/view?usp=sharing">Take 6</a>

</div>
