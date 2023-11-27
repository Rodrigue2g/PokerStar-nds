
#ifndef GAME_H_
#define GAME_H_

typedef struct Game Game;
struct Game;

void shuffleDeck();
void burn();
void nextCard();

void playerPlay();
void nextPlayer();

void updatePot();
void updateTime();

void foldPlayer();
void bet();

void findBestHand();
void updatePlayersHand();

#endif /* GAME_H_ */