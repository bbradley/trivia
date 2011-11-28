#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Player {
  char* name;
  struct Player* next;
} Player;
static Player* players = NULL;

typedef struct Question {
  char* question;
  struct Question* next;
} Question;
static Question* popQuestions = NULL;
static Question* scienceQuestions = NULL;
static Question* sportsQuestions = NULL;
static Question* rockQuestions = NULL;

static int places[6];
static int purses[6];
static bool inPenaltyBox[6];

static int currentPlayer = 0;
static bool isGettingOutOfPenaltyBox;

static char* createRockQuestion(int index)
{
  char question[128];
  sprintf(question, "Rock Question %d", index);
  return strdup(question);
}

void Game_New()
{
  Question* lastPopQuestion = popQuestions = malloc(sizeof(*lastPopQuestion));
  Question* lastScienceQuestion = scienceQuestions = malloc(sizeof(*lastScienceQuestion));
  Question* lastSportsQuestion = sportsQuestions = malloc(sizeof(*lastSportsQuestion));
  Question* lastRockQuestion = rockQuestions = malloc(sizeof(*lastRockQuestion));
  int i;
  for (i = 0; i < 50; i++) {
    char question[128];
    sprintf(question, "Pop Question %d", i);
    lastPopQuestion->question = strdup(question);
    if (i == 50)
      lastPopQuestion->next = NULL;
    else
      lastPopQuestion = lastPopQuestion->next = malloc(sizeof(*lastPopQuestion));
    sprintf(question, "Science Question %d", i);
    lastScienceQuestion->question = strdup(question);
    if (i == 50)
      lastScienceQuestion->next = NULL;
    else
      lastScienceQuestion = lastScienceQuestion->next = malloc(sizeof(*lastScienceQuestion));
    sprintf(question, "Sports Question %d", i);
    lastSportsQuestion->question = strdup(question);
    if (i == 50)
      lastSportsQuestion->next = NULL;
    else
      lastSportsQuestion = lastSportsQuestion->next = malloc(sizeof(*lastSportsQuestion));
    lastRockQuestion->question = createRockQuestion(i);
    if (i == 50)
      lastRockQuestion->next = NULL;
    else
      lastRockQuestion = lastRockQuestion->next = malloc(sizeof(*lastRockQuestion));
  }
}

void Game_Delete()
{
  while (popQuestions)
  {
    Question* next = popQuestions->next;
    free(popQuestions->question);
    free(popQuestions);
    popQuestions = next;
  }
  while (scienceQuestions)
  {
    Question* next = scienceQuestions->next;
    free(scienceQuestions->question);
    free(scienceQuestions);
    scienceQuestions = next;
  }
  while (sportsQuestions)
  {
    Question* next = sportsQuestions->next;
    free(sportsQuestions->question);
    free(sportsQuestions);
    sportsQuestions = next;
  }
  while (rockQuestions)
  {
    Question* next = rockQuestions->next;
    free(rockQuestions->question);
    free(rockQuestions);
    rockQuestions = next;
  }
  while (players)
  {
    Player* next = players->next;
    free(players->name);
    free(players);
    players = next;
  }
}

static int howManyPlayers()
{
  int playerCount = 0;

  Player* currentPlayer = players;
  while (currentPlayer)
  {
    playerCount += 1;
    currentPlayer = currentPlayer->next;
  }

  return playerCount;
}

bool Game_Add(const char* playerName)
{
  Player* nextPlayer = malloc(sizeof(*nextPlayer));
  nextPlayer->name = strdup(playerName);
  nextPlayer->next = NULL;

  if (!players)
  {
    players = nextPlayer;
  }
  else
  {
    Player* lastPlayer = players;
    while (lastPlayer->next)
      lastPlayer = lastPlayer->next;
    lastPlayer->next = nextPlayer;
  }

  places[howManyPlayers()] = 0;
  purses[howManyPlayers()] = 0;
  inPenaltyBox[howManyPlayers()] = false;

  printf("%s was added\n", playerName);
  printf("They are player number %d\n", howManyPlayers());

  return true;
}

static Player* getPlayer(int number)
{
  Player *player = players;
  while (number--)
    player = player->next;
  return player;
}

static char* currentCategory()
{
  if (places[currentPlayer] == 0) return "Pop";
  if (places[currentPlayer] == 4) return "Pop";
  if (places[currentPlayer] == 8) return "Pop";
  if (places[currentPlayer] == 1) return "Science";
  if (places[currentPlayer] == 5) return "Science";
  if (places[currentPlayer] == 9) return "Science";
  if (places[currentPlayer] == 2) return "Sports";
  if (places[currentPlayer] == 6) return "Sports";
  if (places[currentPlayer] == 10) return "Sports";
  return "Rock";
}

static void askQuestion()
{
  if (!strcmp(currentCategory(), "Pop"))
  {
    printf("%s\n", popQuestions->question);
    Question* tmp = popQuestions;
    popQuestions = popQuestions->next;
    free(tmp->question);
    free(tmp);
  }
  if (!strcmp(currentCategory(), "Science"))
  {
    printf("%s\n", scienceQuestions->question);
    Question* tmp = scienceQuestions;
    scienceQuestions = scienceQuestions->next;
    free(tmp->question);
    free(tmp);
  }
  if (!strcmp(currentCategory(), "Sports"))
  {
    printf("%s\n", sportsQuestions->question);
    Question* tmp = sportsQuestions;
    sportsQuestions = sportsQuestions->next;
    free(tmp->question);
    free(tmp);
  }
  if (!strcmp(currentCategory(), "Rock"))
  {
    printf("%s\n", rockQuestions->question);
    Question* tmp = rockQuestions;
    rockQuestions = rockQuestions->next;
    free(tmp->question);
    free(tmp);
  }
}

void Game_Roll(int roll)
{
  Player *player = getPlayer(currentPlayer);
	printf("%s is the current player\n", player->name);
	printf("They have rolled a %d\n", roll);

  if (inPenaltyBox[currentPlayer])
  {
    if (roll % 2 != 0) {
      isGettingOutOfPenaltyBox = true;

      printf("%s is getting out of the penalty box\n", player->name);
      places[currentPlayer] = places[currentPlayer] + roll;
      if (places[currentPlayer] > 11) places[currentPlayer] = places[currentPlayer] - 12;

      printf("%s's new location is %d\n", player->name, places[currentPlayer]);
      printf("The category is %s\n", currentCategory());
      askQuestion();
    } else {
      printf("%s is not getting out of the penalty box\n", player->name);
      isGettingOutOfPenaltyBox = false;
    }
  } else {
    places[currentPlayer] = places[currentPlayer] + roll;
    if (places[currentPlayer] > 11) places[currentPlayer] = places[currentPlayer] - 12;

    printf("%s's new location is %d\n", player->name, + places[currentPlayer]);
    printf("The category is %s\n", currentCategory());
    askQuestion();
  }
}

bool Game_WrongAnswer()
{
  Player *player = getPlayer(currentPlayer);
  printf("Question was incorrectly answered\n");
  printf("%s was sent to the penalty box\n", player->name);
  inPenaltyBox[currentPlayer] = true;

  currentPlayer++;
  if (currentPlayer == howManyPlayers()) currentPlayer = 0;
  return true;
}

static bool didPlayerWin()
{
  return !(purses[currentPlayer] == 6);
}

bool Game_WasCorrectlyAnswered()
{
  Player *player = getPlayer(currentPlayer);
  if (inPenaltyBox[currentPlayer])
  {
    if (isGettingOutOfPenaltyBox)
    {
      printf("Answer was correct!!!!\n");
      purses[currentPlayer]++;
      printf("%s now has %d Gold Coins.\n", player->name, purses[currentPlayer]);

      bool winner = didPlayerWin();
      currentPlayer++;
      if (currentPlayer == howManyPlayers()) currentPlayer = 0;

      return winner;
    } else {
      currentPlayer++;
      if (currentPlayer == howManyPlayers()) currentPlayer = 0;
      return true;
    }
  } else {
    printf("Answer was correct!!!!\n");
    purses[currentPlayer]++;
    printf("%s now has %d Gold Coins.\n", player->name, purses[currentPlayer]);

    bool winner = didPlayerWin();
    currentPlayer++;
    if (currentPlayer == howManyPlayers()) currentPlayer = 0;

    return winner;
  }
}
