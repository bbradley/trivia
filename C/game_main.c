#include "game.h"
#include <stdlib.h>

int main()
{
  bool notAWinner;

  Game_New();

  Game_Add("Chet");
  Game_Add("Pat");
  Game_Add("Sue");

  do
  {
    Game_Roll((rand() % 5) + 1);
    if ((rand() % 9) == 7)
      notAWinner = Game_WrongAnswer();
    else
      notAWinner = Game_WasCorrectlyAnswered();
  } while (notAWinner);

  Game_Delete();

  return 0;
}
