#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "take6.h"

int32_t cmp(const void *a, const void *b)
{
	return (((sCard *)a)->num) - (((sCard *)b)->num);
}

int32_t countCattleHead(int32_t num)
{
	int32_t count;

	if (num == 55)
	{
		return 7;
	}
	else if (num % 10 == 5)
	{
		return 2;
	}
	else if (num % 10 == 0)
	{
		return 3;
	}
	else if (num % 11 == 0)
	{
		return 5;
	}
	else
	{
		return 1;
	}

	return 0;
}

void setup(sCard deck[])
{
	for (int32_t i = 0; i < DECK_SIZE; i++)
	{
		deck[i].num = i + 1;
		deck[i].used = false;
		deck[i].owner = 0;
		deck[i].cattle_head = countCattleHead(i + 1);
	}

	return;
}

//洗牌, 發牌
void shuffle(sCard deck[], sCard tableCard[][MAX_TABLE_CARD], sCard playerCard[][MAX_HAND_CARD], int32_t hand_card_amount, int32_t player_amount)
{
	printf("%d, %d\n", player_amount, hand_card_amount);
	srand(time(0));
	for (int32_t n = 0; n < 10000; n++)
	{
		int32_t i = rand() % 104;
		int32_t j = rand() % 104;
		sCard tmp;
		tmp.num = deck[i].num;
		deck[i].num = deck[j].num;
		deck[j].num = tmp.num;

		tmp.cattle_head = deck[i].cattle_head;
		deck[i].cattle_head = deck[j].cattle_head;
		deck[j].cattle_head = tmp.cattle_head;
	}

	for (int32_t i = 0; i < 4; i++)
	{
		tableCard[i][0].num = deck[i].num;
		tableCard[i][0].cattle_head = deck[i].cattle_head;
		deck[i].used = true;
		deck[i].owner = -1;

		for (int32_t j = 1; j < MAX_TABLE_CARD; j++)
		{
			tableCard[i][j].num = 0;
			tableCard[i][j].cattle_head = 0;
		}
	}

	for (int32_t i = 0; i < player_amount; i++)
	{
		for (int32_t j = 0; j < hand_card_amount; j++)
		{
			playerCard[i][j].num = deck[4 + i * hand_card_amount + j].num;
			playerCard[i][j].cattle_head = deck[4 + i * hand_card_amount + j].cattle_head;
			deck[4 + i * hand_card_amount + j].owner = true;
			deck[4 + i * hand_card_amount + j].owner = i;
		}
	}

	return;
}
void delPlayerCard(sCard playerCard[][MAX_HAND_CARD], int32_t owner, int32_t *target, int32_t *hand_card_amount)
{
	int32_t flag = 0, input;
	while (flag == 0)
	{
		for (int32_t i = 0; i < *hand_card_amount; i++)
		{
			if (playerCard[owner][i].num == *target)
			{
				flag = 1;
				playerCard[owner][i].num = 105;
				qsort(playerCard[owner], *hand_card_amount, sizeof(sCard), cmp);
				*hand_card_amount -= 1;
			}
		}
		if (flag == 0)
		{
			puts(RED "Wrong input! The card you choose wasn't your card." RESET);
			printf(MAG "Please re-choose a card of your cards. ...>" RESET);
			scanf("%d", &input);
			*target = input;
		}
	}
}
void addToTable(sCard tableCard[][MAX_TABLE_CARD], sCard countCH[], int32_t owner, int32_t new, int32_t _mode)
{
	//printf( "mode = %d\n", _mode );
	int32_t row, col, min_close = 0, input, mode = _mode;
	for (int32_t i = 0; i < 4; i++)
	{
		for (int32_t j = 0; j < MAX_TABLE_CARD; j++)
		{
			int32_t now_num = tableCard[i][j].num;
			if (now_num != 0 && now_num < new &&now_num > min_close)
			{
				min_close = now_num;
				row = i;
				col = j;
			}
		}
	}

	if ((min_close == 0) || (tableCard[row][col + 1].num != 0))
	{
		if (owner == 0)
		{
			printf("\n-------\n");
			puts(RED "Your card can not be put in any row!" RESET);
			printf(MAG "Please choose a row to take cattle heads. ...> " RESET);
			scanf("%d", &input);
			while (!(input == 1 || input == 2 || input == 3 || input == 4))
			{
				printf(RED "Wrong input! " RESET);
				printf(MAG "Please input the row in range 1 ~ 4 ...> " RESET);
				scanf("%d", &input);
			}
			row = input - 1;
		}
		else
		{
			int32_t tmp, min = 105, max = 0;
			for (int32_t i = 0; i < 4; i++)
			{
				tmp = 0;
				for (int32_t j = 0; j < 6; j++)
				{
					tmp += tableCard[i][j].cattle_head;
				}
				//printf( "mode = %d\n", mode );
				if (min > tmp && mode > 1)
				{
					min = tmp;
					row = i;
				}
				//*
				if (max < tmp && mode == 1)
				{
					max = tmp;
					row = i;
				}
				//*/
			}

			if (mode == 1)
				printf(BLU "COM%d eat the row %d, and get %d cattles.\n" RESET, owner, row, max);
			else
				printf(BLU "COM%d eat the row %d, and get %d cattles.\n" RESET, owner, row, min);
		}

		for (int32_t i = 0; i < 6; i++)
		{
			countCH[owner].cattle_head += tableCard[row][i].cattle_head;
			tableCard[row][i].num = 0;
			tableCard[row][i].cattle_head = 0;
		}
		tableCard[row][0].num = new;
		tableCard[row][0].cattle_head = countCattleHead(new);
	}
	else
	{
		if (col == 4)
		{
			if (owner == 0)
				printf(BLU "You eat the row" RESET);
			else
				printf(BLU "COM%d eat the row" RESET, owner);
			printf(YEL " %d " RESET, row + 1);
			printf(BLU "!\n" RESET);

			for (int32_t i = 0; i < 6; i++)
			{
				countCH[owner].cattle_head += tableCard[row][i].cattle_head;
				tableCard[row][i].num = 0;
				tableCard[row][i].cattle_head = 0;
			}
			tableCard[row][0].num = new;
			tableCard[row][0].cattle_head = countCattleHead(new);
		}
		else
		{
			tableCard[row][col + 1].num = new;
			tableCard[row][col + 1].cattle_head = countCattleHead(new);
		}
	}
}

void game(sCard deck[], const int32_t _com_num, const int32_t _hand_card_amount, const int32_t _mode, int32_t easterEgg, int32_t os)
{
	//printf( "mode = %d\n", _mode );
	//PAUSE; CLEAR_BUFFER;

	int32_t player_amount = _com_num + 1;
	int32_t hand_card_amount = _hand_card_amount;
	int32_t mode = _mode;
	sCard playerCard[player_amount][MAX_HAND_CARD];
	sCard tableCard[4][6];
	shuffle(deck, tableCard, playerCard, hand_card_amount, player_amount);

	sCard countCH[player_amount];
	for (int32_t i = 0; i < player_amount; i++)
	{
		countCH[i].owner = i;
		countCH[i].cattle_head = 0;
	}
	//*
	while (hand_card_amount)
	{
		if (os == 1)
		{
			system("cls"); //Windows system
		}
		else if (os == 2)
		{
			system("clear"); // Linux system
		}
		puts(YEL "\t\t\t\t~~ Let's play take6 ! ~~" RESET);

		//顯示player牛頭數
		printCattleCount(countCH, player_amount);
		//顯示桌面
		printTable(tableCard);
		//顯示手牌
		for (int32_t i = 0; i < player_amount; i++)
		{
			qsort(playerCard[i], hand_card_amount, sizeof(sCard), cmp);
		}
		printPlayerCard(playerCard, hand_card_amount, easterEgg);

		putchar('\n');

		sCard put_cards[player_amount];
		for (int32_t i = 0; i < player_amount; i++)
		{
			put_cards[i].num = 0;
		}

		if (hand_card_amount > 1)
		{
			//電腦選牌
			if (easterEgg == 666 || easterEgg == 777)
				printComCard(playerCard, hand_card_amount, player_amount);
			srand(time(0));
			for (int32_t i = 1; i < player_amount; i++)
			{
				int32_t rand_card = rand() % hand_card_amount;
				put_cards[i].num = playerCard[i][rand_card].num;
				put_cards[i].cattle_head = countCattleHead(put_cards[i].num);
				put_cards[i].owner = i;
				delPlayerCard(playerCard, i, &(put_cards[i].num), &hand_card_amount);
				hand_card_amount += 1;
			}
			if (easterEgg == 5487 || easterEgg == 666 || easterEgg == 777)
			{
				qsort(put_cards, player_amount, sizeof(sCard), cmp);
				printPutCards(put_cards, player_amount, easterEgg);
			}

			//玩家選牌
			int32_t input, input_2, input_3, flag = 0;
			if (easterEgg == 777)
			{
				puts(MAG "You can put any card which card is\nnot on the table and not in other players' card.( 1 ~ 104 )");
				scanf("%d", &input);
				input_2 = input;
				while (flag == 0)
				{
					if (input <= 1 || input >= 104)
					{
						puts(RED "Wrong input!" RESET);
						printf(MAG "You also need to choose a card within 1 ~ 104. ...> ");
						scanf("%d", &input);
					}
					else
					{
						while (flag != 1)
						{
							for (int32_t i = 0; i < 4; i++)
								for (int32_t j = 0; j < 6; j++)
									if (input == tableCard[i][j].num)
									{
										puts(RED "Wrong input! Your choice is on the table." RESET);
										printf(MAG "Please re-choose a card within 1 ~ 104. ...> ");
										scanf("%d", &input_2);
									}
							if (input == input_2)
							{
								flag = 1;
							}
							else
								input = input_2;
						}
					}
				}
				put_cards[0].num = input;
				put_cards[0].cattle_head = countCattleHead(input);
				put_cards[0].owner = 0;
			}
			else
			{
				puts(MAG "Which card do you want to choose ?" RESET);
				printf(MAG "Enter ...> " RESET);
				scanf("%d", &input);
				put_cards[0].num = input;
				put_cards[0].cattle_head = countCattleHead(input);
				put_cards[0].owner = 0;
			}

			delPlayerCard(playerCard, 0, &input, &hand_card_amount);
		}
		else
		{
			for (int32_t i = 0; i < player_amount; i++)
			{
				for (int32_t j = 0; j < _hand_card_amount; j++)
				{
					if (playerCard[i][j].num != 0 && playerCard[i][j].num != 105)
					{
						put_cards[i].num = playerCard[i][j].num;
						put_cards[i].cattle_head = playerCard[i][j].cattle_head;
						put_cards[i].owner = i;
						hand_card_amount += 1;
						delPlayerCard(playerCard, i, &(put_cards[i].num), &hand_card_amount);
					}
				}
			}
			hand_card_amount = 0;
		}

		qsort(put_cards, player_amount, sizeof(sCard), cmp);
		int32_t counter = 0;
		while (counter < player_amount)
		{
			if (os == 1)
			{
				system("cls"); //Windows system
			}
			else if (os == 2)
			{
				system("clear"); // Linux system
			};
			printCattleCount(countCH, player_amount);
			printPutCards(put_cards, player_amount, 0);
			printTable(tableCard);
			//printf( "mode = %d\n", mode );
			//PAUSE; CLEAR_BUFFER;
			addToTable(tableCard, countCH, put_cards[counter].owner, put_cards[counter].num, mode);
			put_cards[counter].num = 0;
			put_cards[counter].cattle_head = 0;
			put_cards[counter].owner = 0;
			//*
			if (counter == (player_amount - 1))
			{
				sleep(1);
				if (os == 1)
				{
					system("cls"); //Windows system
				}
				else if (os == 2)
				{
					system("clear"); // Linux system
				};
				printCattleCount(countCH, player_amount);
				printPutCards(put_cards, player_amount, 0);
				printTable(tableCard);
				sleep(1);
				PAUSE;
				CLEAR_BUFFER;
			}
			//*/
			else
				sleep(1);

			counter++;
		}
	}
	//CLEAR_BUFFER;
	//PAUSE;
	//*/
	if (os == 1)
	{
		system("cls"); //Windows system
	}
	else if (os == 2)
	{
		system("clear"); // Linux system
	};
	printCattleCount(countCH, player_amount);
	putchar('\n');
	puts("So, the winner is ...");
	sleep(1);

	puts(".");
	sleep(1);
	puts(".");
	sleep(1);
	puts(".");

	int32_t winner_id = 0;
	for (int32_t i = 1; i < player_amount; i++)
	{
		if (countCH[i].cattle_head < countCH[winner_id].cattle_head)
		{
			winner_id = i;
		}
	}

	if (winner_id == 0)
	{
		puts(YEL "\tYOU !!!\n ~~ Congradulation ~~" RESET);
	}
	else
	{
		printf(YEL "Computer %d\n" RESET, winner_id);
		puts(MAG "So sad QQ... But you can try again !" RESET);
	}
	PAUSE;
	CLEAR_BUFFER;

	return;
}

void menu(int32_t *vs, int32_t *com_num, int32_t *hand_card_amount, int32_t *mode, int32_t *easterEgg, int32_t os)
{
	if (os == 1)
	{
		system("cls"); //Windows system
	}
	else if (os == 2)
	{
		system("clear"); // Linux system
	};
	puts(YEL "----------------------------------- " RESET);
	puts(YEL "      ~~ Wellcome to Take6 ~~       " RESET);
	puts(YEL "----------------------------------- " RESET);
	putchar('\n');

	puts(BLU " ----------------------------------" RESET);
	puts(BLU " Please choose the challenge mode :" RESET);
	puts(BLU " ----------------------------------" RESET);
	puts(BLU "          (1)  1 v.s.    1    " RESET);
	puts(BLU "          (2)  1 v.s. Muitiple" RESET);
	putchar('\n');

	int32_t input_1, input_2, input_3, input_4;
	puts(MAG "Which one do you want to choose ?" RESET);
	printf(MAG "Enter ...> " RESET);
	scanf("%d", &input_1);
	while (!(input_1 == 1 || input_1 == 2))
	{
		puts(RED "Wrong input! " RESET);
		printf(MAG "Please choose 1 or 2 mode. ...> " RESET);
		scanf("%d", &input_1);
	}
	*vs = input_1;

	printf("\n\n");

	*com_num = 1;
	if (input_1 == 2)
	{
		puts(BLU "--------------------------------------------------------------" RESET);
		puts(BLU "Please enter the number of computer you want to fight( 2 ~ 9 )" RESET);
		puts(BLU "--------------------------------------------------------------" RESET);
		printf(MAG "Enter ...> " RESET);
		scanf("%d", &input_2);
		while (!(input_2 == 2 || input_2 == 3 || input_2 == 4 || input_2 == 5 || input_2 == 6 || input_2 == 7 || input_2 == 8 || input_2 == 9))
		{
			puts(RED "Wrong input! " RESET);
			printf(MAG "Please input the number in range 2 ~ 9 ...> " RESET);
			scanf("%d", &input_2);
		}
		*com_num = input_2;

		printf("\n\n");
	}

	puts(BLU "---------------------------------------------------------------------" RESET);
	puts(BLU "Please enter the number of cards you want in the beginning.( 5 ~ 10 )" RESET);
	puts(BLU "---------------------------------------------------------------------" RESET);
	printf(MAG "Enter ...> " RESET);
	scanf("%d", &input_3);
	while (!(input_3 == 5 || input_3 == 6 || input_3 == 7 || input_3 == 8 || input_3 == 9 || input_3 == 10))
	{
		puts(RED "\t\t\tWrong input! " RESET);
		printf(MAG "\t\t\tPlease choose the number in range 5 ~ 10. ...> " RESET);
		scanf("%d", &input_3);
	}
	*hand_card_amount = input_3;

	printf("\n\n");

	puts(BLU "----------------------------------------" RESET);
	puts(BLU "Please chose the mode you want to play :" RESET);
	puts(BLU "----------------------------------------" RESET);
	puts(BLU "          (1) Easy mode" RESET);
	puts(BLU "          (2) Normal mode" RESET);
	puts(BLU "          (3) Hard mode" RESET);
	printf(MAG "Enter ...> " RESET);
	scanf("%d", &input_4);
	while (input_4 != 1 && input_4 != 2 && input_4 != 3)
	{
		if (input_4 == 666)
		{
			putchar('\n');
			puts(YEL "=================================" RESET);
			puts(YEL "You change to the Engineer mode !" RESET);
			puts(YEL "=================================" RESET);
			sleep(2);
			putchar('\n');
			*easterEgg = 666;
			puts(BLU "Then what Computer mode do you want to choose ?");
			sleep(1);
		}
		else if (input_4 == 5487)
		{
			putchar('\n');
			puts(YEL "=====================================" RESET);
			puts(YEL "OK~ Fine~ You are really an idoit.^_^" RESET);
			puts(YEL "I wil show you others player's cards." RESET);
			puts(YEL "=====================================" RESET);
			*easterEgg = 5487;
			putchar('\n');
			sleep(2);
			puts(BLU "Then what Computer mode do you want to choose ?" RESET);
			sleep(1);
		}
		/*
			else if( input_4 == 777 )
			{
				puts( YEL "You change to the \"GOD\" mode !" RESET );
				*easterEgg = 777;
				puts( BLU "Then what Computer mode do you want to choose ?" );
			}
			*/
		else
		{
			puts(RED "Wrong input! " RESET);
		}
		printf(MAG "Please choose the mode in above ( 1 ~ 3 ). ...> " RESET);
		scanf("%d", &input_4);
	}
	if (input_4 == 3)
	{
		puts(YEL "Oh!" RESET);
		sleep(1);
		puts(YEL "You choose the Hard mode!" RESET);
		sleep(2);
		puts(YEL "How brave you are~." RESET);
		sleep(2);
		puts(RED "So you will get nothing help, and I will challenge you!" RESET);
		sleep(3);
		*easterEgg = 0;
	}
	*mode = input_4;

	sleep(1);
	PAUSE;
	CLEAR_BUFFER;
	return;
}

void printCattleCount(sCard countCH[], int32_t player_amount)
{
	puts(GRN "-----------------------------" RESET);
	puts(GRN "Cattle Heads of each player :" RESET);
	puts(GRN "-----------------------------" RESET);
	printf(BLU "Player name :" RESET);
	printf("\tYOUR");
	for (int32_t i = 1; i < player_amount; i++)
	{
		printf("\tCOM%d", i);
	}
	putchar('\n');
	printf(BLU "Cattle Head :" RESET);
	for (int32_t i = 0; i < player_amount; i++)
	{
		printf("\t(%02d)", countCH[i].cattle_head);
	}
	putchar('\n');
	putchar('\n');
}

void printPutCards(sCard put_cards[], int32_t card_amount, int32_t easterEgg)
{
	if (easterEgg)
	{
		puts(YEL "===============================================" RESET);
		puts(YEL "The cards thet the others player hvae choosed :" RESET);
		puts(YEL "===============================================" RESET);
	}
	else
	{
		puts(GRN "----------------------------------------------" RESET);
		puts(GRN "The cards being going to be put to the table :" RESET);
		puts(GRN "----------------------------------------------" RESET);
	}
	for (int32_t i = 0; i < card_amount; i++)
	{
		if (put_cards[i].num != 0)
		{
			printf(YEL "\t%3d" RESET, put_cards[i].num);
			printf(CYAN "(%d)" RESET, put_cards[i].cattle_head);
		}
	}
	putchar('\n');
	putchar('\n');
}

void printTable(sCard tableCard[][MAX_TABLE_CARD])
{
	puts(GRN "--------------------" RESET);
	puts(GRN "Cards on the table :" RESET);
	puts(GRN "--------------------" RESET);
	for (int32_t i = 0; i < 4; i++)
	{
		printf(BLU "Row %d :" RESET, i + 1);
		for (int32_t j = 0; j < 6; j++)
		{
			if (tableCard[i][j].num != 0)
			{
				printf("\t%3d", tableCard[i][j].num);
				printf(CYAN "(%d)" RESET, tableCard[i][j].cattle_head);
			}
		}
		putchar('\n');
	}
	putchar('\n');
}

void printPlayerCard(sCard playerCard[][MAX_HAND_CARD], int32_t hand_card_amount, int32_t easterEgg)
{
	puts(GRN "------------------------" RESET);
	puts(GRN "Look at your ugly hand :" RESET);
	puts(GRN "------------------------" RESET);
	printf(BLU " Your  card :" RESET);
	if (easterEgg == 777)
	{
		puts(YEL "\tYou can take any card which is not on the table." RESET);
	}
	else
	{
		for (int32_t i = 0; i < hand_card_amount; i++)
		{
			if (playerCard[0][i].num != 0)
			{
				printf("\t%3d", playerCard[0][i].num);
				printf(CYAN "(%d)" RESET, playerCard[0][i].cattle_head);
			}
		}
	}

	putchar('\n');
	putchar('\n');
}

void printComCard(sCard playerCard[][MAX_HAND_CARD], int32_t hand_card_amount, int32_t player_amount)
{
	puts(YEL "=============" RESET);
	puts(YEL "NPC's cards :" RESET);
	puts(YEL "=============" RESET);
	for (int32_t id = 1; id < player_amount; id++)
	{
		printf(BLU " Com%d  card :" RESET, id);
		for (int32_t i = 0; i < hand_card_amount; i++)
		{
			if (playerCard[id][i].num != 0)
			{
				printf(MAG "\t%3d" RESET, playerCard[id][i].num);
				printf(MAG "(%d)" RESET, playerCard[id][i].cattle_head);
			}
		}
		putchar('\n');
	}
	putchar('\n');
}

void rule(int32_t os)
{
	if (os == 1)
	{
		system("cls"); //Windows system
	}
	else if (os == 2)
	{
		system("clear"); // Linux system
	};
	puts(GRN "-------" RESET);
	puts(GRN "RULES :" RESET);
	puts(GRN "-------" RESET);
	puts(MAG "In this game, there are 104 numbered cards from 1 to 104, and on each turn, players choose the most appropriate numbered card from their hand without knowing the numbered cards of other players. \n \
	When all players have chosen their numbered cards for the round, the players open their cards at the same time. The game is played with four rows of cards, and each player will place the lowest number in all four rows of cards after each turn. Provided that the last card in each row is the last, the player must place the card he has played at the end of the row closest to the number but less than the number he has played. \n \
	If the card is the sixth card in a row, the player must take the first five cards. If the player's number is less than the last number in any row, the player must choose a row and take all the numbers in that row. \n \
	When all the cards in the hand are used up, each player adds up the penalty points (the number of bulls on each card) from the cards he took back, and the winner is the one with the least." RESET);
	printf("\n");
	puts(GRN "----------------------------------" RESET);
	puts(GRN "Cattle Heads of each Card Number :" RESET);
	puts(GRN "----------------------------------" RESET);
	puts(BLU "76 1-cow license plates" RESET);
	puts(BLU " 9 2-cow license plates ( 5, 15, 25, 35, 45, 65, 75, 85, 95 )" RESET);
	puts(BLU "10 3-cow license plates ( 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 )" RESET);
	puts(BLU " 8 5-cow license plates ( 11, 22, 33, 44, 66, 77, 88, 99 )" RESET);
	puts(BLU " 1 7-cow license plates ( 55 )" RESET);
	putchar('\n');
	PAUSE;
	CLEAR_BUFFER;
	return;
}
