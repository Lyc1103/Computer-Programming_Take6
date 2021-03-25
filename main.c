#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "take6.h"

int main()
{
	while (true)
	{
		int32_t os;
		printf(YEL "Which OS system do you use ? ");
		printf(BLU "(1) Windows (2) Linux \n");
		printf(RED "( Please enter 1 or 2 to continue ): ");
		scanf("%d", &os);
		while (os != 1 && os != 2)
		{
			puts(RED "Wrong input !" RESET);
			printf(MAG "Please take the option within 1 and 2 ...> ");
			scanf("%d", &os);
		}
		if (os == 1)
		{
			system("cls"); //Windows system
		}
		else if (os == 2)
		{
			system("clear"); // Linux system
		}
		puts(YEL "\t\t\t------------------------------------" RESET);
		puts(YEL "\t\t\t      ~~ Wellcome to Take6 ~~       " RESET);
		puts(YEL "\t\t\t------------------------------------" RESET);
		putchar('\n');
		puts(GRN "\t\t\tWhilch option do you wnat to choose :" RESET);
		puts(BLU "\t\t\t         (1) Play game !" RESET);
		puts(BLU "\t\t\t         (2) How to play ?" RESET);
		puts(BLU "\t\t\t         (3) Exit" RESET);
		putchar('\n');
		int32_t input, easterEgg = 0;
		printf(MAG "\t\t\tPlease take the option. ...> " RESET);
		scanf("%d", &input);
		while (input != 1 && input != 2 && input != 3)
		{
			puts(RED "\t\t\tWrong input !" RESET);
			printf(MAG "\t\t\tPlease take the option within 1 ~ 3 ...> " RESET);
			scanf("%d", &input);
		}
		if (input == 1)
		{
			sCard deck[DECK_SIZE] = {0};
			setup(deck);

			int32_t vs, com_num, hand_card_amount, mode;
			menu(&vs, &com_num, &hand_card_amount, &mode, &easterEgg, os);

			//printf( "mode = %d\n", mode );
			//PAUSE; CLEAR_BUFFER;
			game(deck, com_num, hand_card_amount, mode, easterEgg, os);
		}
		else if (input == 2)
		{
			rule(os);
		}
		else
		{
			puts(YEL "\t\t\tGoodbye ~~" RESET);
			printf("\t\t\t");
			PAUSE;
			CLEAR_BUFFER;
			break;
		}
	}

	return 0;
}
