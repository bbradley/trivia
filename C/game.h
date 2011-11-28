#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

void Game_New();
void Game_Delete();
bool Game_Add(const char* playerName);
void Game_Roll(int roll);
bool Game_WrongAnswer();
bool Game_WasCorrectlyAnswered();

#endif
