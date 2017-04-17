#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "crossfireOperations.h"

int main(void)
{

	int row, column;

	struct slot* currSlot = NULL;
	struct slot *foundSlots;
	bool explored[BOARD_SIZE][BOARD_SIZE];
	int count = 0;

	struct slot *upLeft;			//pointer to slot (0,0)

	struct slot *upRight;				//pointer to slot (0,BOARD_SIZE -1)

	struct slot *downLeft;				//pointer to slot (boardSize - 1, 0)

	struct slot *downRight;				//pointer to slot (boardSize - 1, boardSize -1)


	//Creates the board
	createBoard(BOARD_SIZE, &upLeft, &upRight, &downLeft, &downRight);

	int numPlayers, activePlayers;

	printf("How many players would you like in the game? (max 6): ");		//get num players
	scanf("%d", &numPlayers);

	while(numPlayers < 2 || numPlayers > 6)		
	{
		printf("\n\nNumber of players must be at least 2, but at most 6: ");		//checks for a valid number of players
		scanf("%d", &numPlayers);
	}

	activePlayers = numPlayers; 				//active players is started at numPlayers, but will be decremented when players are removed

	struct player playerArray[numPlayers];			//array of player structs

	for(int i = 0; i < numPlayers; i++)	//making the appropriate number of player structs
	{
		getPlayer(&playerArray[i], i);
	}

	boardSlots(numPlayers);		//allocate board slot type

	playerSlots(numPlayers, playerArray);	//alloctate players to slots randomly

	bool live = true;			//will set to false when activePlayers <= 1, and loop will break
	int round = 1;				//round number starts at 1 and is incremented at the end of each round (just after thefor loop)

	while(live)				//the while loop acts as the game loop, each iteration is a single round
	{
		for(int loop = 0; loop < numPlayers; loop++)		//goes through all the players left in a game to give them their turn
		{
			if(playerArray[loop].type != 8)		//if the player number is not 8 (i.e. not dead or hasn't quit the game) then they get a turn
			{
				int menuChoice, moveChoice, attackChoice;		//the choices for
				bool up = false, down = false, left = false, right = false;		//set to true if a player can move in that direction
				bool canNearAttack = false, canDistanceAttack = false, canMagicAttack = false; 		//set true if capabilities allow player to atack, then check if players available for attack and adjust 

				checkIfMove(&playerArray[loop], &up, &down, &left, &right);		//where can the player move?

				numMagic = 0, numNear = 0, numDistance = 0;					//how many players are available for each attack?

				checkMagicCapabilities(&playerArray[loop], &canMagicAttack);	//can a player perform magic?

				int attackNear[activePlayers - 1], attackDistance[activePlayers - 1], attackMagic[activePlayers - 1];		//lists of attackable players

				checkPlayersToAttack(playerArray[loop], attackNear, attackDistance, attackMagic, &canNearAttack, &canDistanceAttack, &canMagicAttack, numPlayers, loop);  //check attack capabilities

				printf("\n");

				printf("Round %d\n\n", round);		//prints the round

				for(int i = 0; i < BOARD_SIZE; i++)			//the following will print the board out (varied spacing has been used to keep a standard size rectangular shape)
				{
					for(int j = 0; j < BOARD_SIZE; j++)
					{
						if(board[i][j].slotType == 1)
						{
							printf("Level   ");
						}					
						else if(board[i][j].slotType == 2)			//printing slot type and appropriate spacing
						{
							printf("Hill    ");
						}
						else 
						{
							printf("City    ");
						}
					}

					printf("\n");

					for(int j = 0; j < BOARD_SIZE; j++)
					{
						for(int c = 0; c < numPlayers; c++)
						{
							if(board[i][j].playerIndexInArray[c] != 8)
							{
								printf("%d", board[i][j].playerIndexInArray[c] + 1);	//print player number or spacing
							}
							else
							{
								printf(" ");
							}
						}

						for(int x = 0; x < 2 + (6 - numPlayers); x++)
						{
							printf(" ");										//print appropriate spacing
						}
					}

					printf("\n");
				}

				printf("\n");

				for(int i = 0; i < numPlayers; i++)
				{
					if(playerArray[i].type != 8)
					{
						char playType[7];

						if(playerArray[i].type == 1)
						{
						 	strcpy(playType,"Elf");
						}
						else if(playerArray[i].type == 2)			//gives playType the player's type in text
						{
							strcpy(playType,"Human");
						}
						else if(playerArray[i].type ==  3)
						{
							strcpy(playType,"Ogre");
						}
						else
						{
							strcpy(playType,"Wizard");
						}

						//print's all live players' information: name, type, life points, coordinates etc.
						printf("Player %d:%s %s(%d) [%d,%d] Smartness: %d Strength: %d Magic: %d Luck: %d Dexterity: %d", i + 1, playerArray[i].name, playType, playerArray[i].life_points, playerArray[i].row, playerArray[i].column, playerArray[i].smartness, playerArray[i].strength, playerArray[i].magicSkill, playerArray[i].luck, playerArray[i].dexterity);
						printf("\n");
					}
				}

				printf("\nPlayer %d's Turn: \n", loop + 1);		//prints out which player's turn it is

				printf("\nWould you like to :");		//printing out the menu of choices
				printf("\n1) Move");

				if(canMagicAttack || canNearAttack || canDistanceAttack)
				{
					printf("\n2) Attack");					//if any attacks are possible, the attack option is printed
				}

				printf("\n3) Forfeit Move");
				printf("\n4) Quit Games\n ?");		

				scanf("%d", &menuChoice);

				while((!(canMagicAttack || canNearAttack || canDistanceAttack) && menuChoice == 2) || menuChoice < 1 || menuChoice > 4)
				{
					printf("\nPlease enter a valid selection :");		//if they enter an impossible choice, they are asked to reinput
					scanf("%d", &menuChoice);
				}

				if(menuChoice == 1)
				{
					printf("\n\n");
					if(up)
					{
						printf("\n1) Move up");				//prints posible choices for movement direction
					}

					if(down)
					{
						printf("\n2) Move down");
					}

					if(left)
					{
						printf("\n3) Move left");
					}

					if(right)
					{
						printf("\n4) Move right");
					}
					
					printf("\n ?");
					scanf("%d", &moveChoice);

					if((moveChoice == 1 && !(up)) || (moveChoice == 2 && !(down)) || (moveChoice == 3 && !(left)) || (moveChoice == 4 && !(right)) || (moveChoice < 1 || moveChoice > 4))
					{
						printf("\nEnter a valid choice");		//if choice is not valid, user must resubmit
						scanf("%d", &moveChoice); 
					}

					movePlayer(&playerArray[loop], moveChoice, loop);
				}
				else if(menuChoice == 2)
				{
					int i = 0, playerAttack;

					printf("\n\nWhat kind of attack: ");		//gets user to input desired attack 
					if(canMagicAttack)
					{
						printf("\n1) Magic");
					}

					if(canDistanceAttack)
					{
						printf("\n2) Distance");
					}

					if(canNearAttack)
					{
						printf("\n3) Near");
					}

					printf("\n ?");

					scanf("%d", &attackChoice);

					while((!canMagicAttack && attackChoice == 1) || (!canDistanceAttack && attackChoice == 2) || (!canNearAttack && attackChoice == 3) || (attackChoice < 1 || attackChoice > 3))
					{
						printf("\nPlease enter a valid choice: ");		//if choice is invalid, ask for input again
						scanf("%d", &attackChoice);
					}

					if(attackChoice == 1)		//magic attack
					{
						printf("\n\nChoose a player to attack:");

						while(i < numMagic)
						{
							if(attackMagic[i] >= 0 && attackMagic[i] <= numPlayers - 1)
							{
								printf("\n%d) Player %d", attackMagic[i] + 1, attackMagic[i] + 1);		//prints possible players to attack
								i += 1;
							}
						}

						printf("\n ?");
						scanf("%d", &playerAttack);				

						playerAttack -= 1;

						while(!(inputChecker(playerAttack, attackMagic, numMagic)) || playerAttack == loop)		//if player is not in the list of attackables, then repeat
						{
							printf("\nPlease enter a valid choice: ");
							scanf("%d", &playerAttack);

							playerAttack -= 1;
						}

						magicAttack(&playerArray[loop], &playerArray[playerAttack]);
					}
					else if(attackChoice == 2)		//distance attack
					{
						printf("\n\nChoose a player to attack:");

						while(i < numDistance)
						{
							if(attackDistance[i] >= 0 && attackDistance[i] <= numPlayers - 1)
							{
								printf("\n%d) Player %d", attackDistance[i] + 1, attackDistance[i] + 1);	//prints possible players to attack
								i += 1;
							}
						}

						printf("\n ?");
						scanf("%d", &playerAttack);

						playerAttack -= 1;

						while(!(inputChecker(playerAttack, attackDistance, numDistance)) || playerAttack == loop)	//if player is not in the list of attackables, then repeat
						{
							printf("\nPlease enter a valid choice: ");
							scanf("%d", &playerAttack);

							playerAttack -= 1;
						}

						distanceAttack(&playerArray[loop], &playerArray[playerAttack]);
					}
					if(attackChoice == 3)		
					{
						printf("\n\nChoose a player to attack:");

						while(i < numNear)
						{
							if(attackNear[i] >= 0 && attackNear[i] <= numPlayers - 1)
							{
								printf("\n%d) Player %d", attackNear[i] + 1, attackNear[i] + 1);		//prints possible players to attack
								i += 1;
							}
						}

						printf("\n ?");
						scanf("%d", &playerAttack);

						playerAttack -= 1;

						while(!(inputChecker(playerAttack, attackNear, numNear)) || playerAttack == loop)		//if player is not in the list of attackables, then repeat
						{
							printf("\nPlease enter a valid choice: ");
							scanf("%d", &playerAttack);

							playerAttack -= 1;
						}

						nearAttack(&playerArray[loop], &playerArray[playerAttack]);
					}
				}//attacking goes here		>> if it can be printed
				else if(menuChoice == 3)
				{
					printf("\n\nYou have forfeited your move.");		//do nothing, simply continue 
				}//forfeit move 
				else if(menuChoice == 4)
				{
					printf("\n\nYou have been removed from the game");

					removePlayer(playerArray, loop, &activePlayers);		//removes quit player from game
				}//quit game -> run remove player function
			}

			if(!(activePlayers <= 1)) //run find dead player function here >> run remove player function inside if player has lifepoints <= 0		//only if numPlayers > 1
			{
				findDead(playerArray, numPlayers, &activePlayers);
			}
			else
			{
				break;		//if only 1 (or less players) remain, then break the for loop to set live = false
			}
		}

		if(activePlayers <= 1)
		{
			live = false;		//set live = false to break while loop
		}

		round++;			//increment the round counter
	}

	for(int i = 0; i < numPlayers; i++)
	{
		if(playerArray[i].type != 8)			//find the remaining player and congratulate them on their victory
		{
			printf("\n\nCongratulations Player %d, %s, you are the winner!", i + 1, playerArray[i].name);		
		}
	}
}
