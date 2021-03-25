#include <stdint.h>
#include <stdbool.h>

#define DECK_SIZE 104
#define MAX_HAND_CARD 10
#define MAX_TABLE_CARD 6
#define CLEAR_BUFFER                                   \
	do                                                 \
	{                                                  \
		int32_t c = 0;                                 \
		while ((c = fgetc(stdin)) != '\n' && c != EOF) \
		{                                              \
		}                                              \
	} while (0)
int32_t ch;
#define PAUSE                             \
	printf("Press Enter to continue..."); \
	ch = getchar()

#define RED "\x1b[;31;1m"
#define GRN "\x1b[;32;1m"
#define YEL "\x1b[;33;1m"
#define BLU "\x1b[;34;1m"
#define MAG "\x1b[;35;1m"
#define CYAN "\x1b[;36;1m"
#define RESET "\x1b[0;m"

typedef struct _sCard
{
	int32_t num;
	bool used;
	int32_t owner;
	int32_t cattle_head;
} sCard;

int32_t cmp(const void *, const void *);
int32_t countCattleHead(int32_t);
void setup(sCard deck[]);
void shuffle(sCard deck[], sCard tableCard[][MAX_TABLE_CARD], sCard playerCard[][MAX_HAND_CARD], int32_t, int32_t);
void delPlayerCard(sCard playerCard[][MAX_HAND_CARD], int32_t, int32_t *, int32_t *);
void addToTable(sCard tableCard[][MAX_TABLE_CARD], sCard countCH[], int32_t, int32_t, int32_t);
void game(sCard deck[], const int32_t, const int32_t, const int32_t, int32_t, int32_t);
void menu(int32_t *, int32_t *, int32_t *, int32_t *, int32_t *, int32_t);
void printCattleCount(sCard count_CH[], int32_t);
void printPutCards(sCard put_cards[], int32_t, int32_t);
void printTable(sCard tableCard[][MAX_TABLE_CARD]);
void printPlayerCard(sCard playerCard[][MAX_HAND_CARD], int32_t, int32_t);
void printComCard(sCard playerCard[][MAX_HAND_CARD], int32_t, int32_t);
void rule(int32_t os);
