#ifndef CROSSFIREOPERATIONS_H_
#define CROSSFIREOPERATIONS_H_
#endif 

#define BOARD_SIZE 7
#define REQ_DISTANCE 5

struct slot ** board;										//board stucture created globally
int numMagic, numDistance, numNear;							//the number of players found that each of these attacks can be preformed on

struct player{
	char name[20];
	int num;											//players number 
	int type;											//player type : set to 8 if remove from game OR dead
	int slotType;									//stores slot type of player
	int row;											//player's row
	int column;										//player's column
	int life_points;			//player's life points
	int smartness;
	int strength;						//player's stats
	int magicSkill;
	int luck;
	int dexterity;

	int originalStrength;
	int originalDexterity;				//these hold the original values if the stats are reduced below zero
} player_struct;

struct slot{
													
	int row;						//row number

	int column;						//column number

	int slotType;			//type of slot 1 level ground, 2 hill, 3 for city

	int playerIndexInArray[6];		//holds player's location in player playerIndexInArray     //use calloc to replace [6]

	struct slot *left;			// adjacent left slot		

	struct slot *right;				// adjacent right slot

	struct slot *up; 				// adjacent up slot

	struct slot *down;					// adjacent down slot
}slot;

void createBoard(int boardSize, struct slot **upLeft, struct slot **upRight, struct slot **downLeft, struct slot **downRight);	//creates board

void getPlayer(struct player *playerGet, int playerNum);			//gets player type, name and assigns num and life points, calls assign stat function
		
void assignStats(struct player *playerStat);							//assigns stats according to player type

void boardSlots(int numPlayers);									//assigns each slot a type [1,3] level, hill, city

void playerSlots(int numPlayers, struct player playerArray[]);			//assigns players to a slot at random, calls setstat function

void setStat(struct player *slotPlayer, int slotType);						//alters stats accoring to slot type

void checkIfMove(struct player *currPlayer, bool *up, bool *down, bool *left, bool *right);		//checks how the player can move

void resetStat(struct player *slotPlayer);											//resets the players stats to normal before movement takes place

void movePlayer(struct player *player, int moveChoice, int playerNum);				//moves the player, calls resetstat and checkslot functions

void checkSlot(struct player *slotPlayer);										//checks the players slot and calls setstat function

void findDead(struct player players[], int numPlayers, int *activePlayers);			//checks to see if any player is dead (life_points <= 0), calls removeplayer function

void removePlayer(struct player player[], int i, int *activePlayers);			//removes a player from the game(either because of death or by quitting)

void checkMagicCapabilities(struct player *player, bool *canMagicAttack);			//checks if they are able to perform a magic attack

//checks what players are available to atttack, and also what attacks are possible to be perfomed
void checkPlayersToAttack(struct player player, int nearAttack[], int distanceAttack[], int magicAttack[], bool *canNearAttack, bool *canDistanceAttack, bool *canMagicAttack, int numPlayers, int loop);

void nearAttack(struct player *attacker, struct player *defender);		//performs near attacke

void distanceAttack(struct player *attacker, struct player *defender);			//performs distance attack

void magicAttack(struct player *attacker, struct player *defender);				//performs magic attack

bool inputChecker(int value, int array[], int arraySize);					//checks to see if a correct player number has been inputted for attack

void createBoard(int boardSize, struct slot **upLeft, struct slot **upRight, struct slot **downLeft, struct slot **downRight)
{
	board = malloc(boardSize * sizeof(struct slot *));

	for(int i =0; i< boardSize; i++)
	{

		board[i] = malloc(boardSize * sizeof(struct slot));

		for(int j=0;j < boardSize; j++)
		{
			board[i][j].row = i;
			board[i][j].column = j;
		}
	}

	for(int i =1; i< boardSize-1; i++)
	{
		for(int j=1;j < boardSize-1; j++)
		{
			board[i][j].up = &board[i-1][j];
			board[i][j].right = &board[i][j+1];
			board[i][j].down = &board[i+1][j];
			board[i][j].left = &board[i][j-1];
		}
	}


	for(int j = 1; j < boardSize -1; j++)
	{
		board[0][j].right = &board[0][j+1];
		board[0][j].left = &board[0][j-1];
		board[0][j].down = &board[1][j];
		board[0][j].up = NULL;

		board[boardSize - 1][j].right = &board[boardSize - 1][j+1];
		board[boardSize -1][j].left = &board[boardSize - 1][j-1];
		board[boardSize - 1][j].up = &board[boardSize - 2][j];
		board[boardSize - 1][j].down = NULL;
	}

	for(int i = 1; i < boardSize -1; i++)
	{
		board[i][0].right = &board[i][1];
		board[i][0].up = &board[i-1][0];
		board[i][0].down = &board[i+1][0];
		board[i][0].left = NULL;

		board[i][boardSize-1].left = &board[i][boardSize-2];
		board[i][boardSize -1].up = &board[i -1][boardSize-1];
		board[i][boardSize -1].down = &board[i+1][boardSize -1];
		board[i][boardSize -1].right = NULL;
	}

		board[0][0].right = &board[0][1];
		board[0][0].down = &board[1][0];

		board[0][boardSize-1].left = &board[0][boardSize-2];
		board[0][boardSize -1].down = &board[1][boardSize -1];

		board[boardSize -1][0].right = &board[boardSize -1][1];
		board[boardSize -1][0].up = &board[boardSize -2][0];

		board[boardSize - 1][boardSize-1].up = &board[boardSize-2][boardSize-1];
		board[boardSize - 1][boardSize -1].left = &board[boardSize -1][boardSize -2];

	*upLeft = &board[0][0];

	*upRight = &board[0][boardSize -1];

	*downLeft = &board[boardSize -1][0];

	*downRight = &board[boardSize -1][boardSize -1];
}

void getPlayer(struct player *playerGet, int playerNum)
{
	char text[20];

	printf("\n\nPlease select a type for player %d by entering a number:\n", playerNum + 1);		//get player type
	printf("(1) Elf\n(2) Human\n(3) Ogre\n(4) Wizard\nEnter the choice: ");
	scanf("%d", &playerGet->type);


	while(playerGet->type < 1 || playerGet->type > 4)
	{
		printf("\n\nYou must enter a valid choice(1-4): ");
		scanf("%d", &playerGet->type);
	}

	
	playerGet->life_points = 100;
	playerGet->num = playerNum;		//see struct for info

	printf("\n\nPlease enter a name for the player: ");			//mget name of player
	scanf("%s", &text);

	strcpy(playerGet->name, text);

	assignStats(playerGet);			//cal assignstats 
}

void assignStats(struct player *playerStat)
{
	time_t t;
	srand((unsigned) time(&t));	

	if(playerStat->type == 1) // if type is elf
	{
		playerStat->luck = rand() % 42 + 59;	//[100,60]
		playerStat->smartness = rand() % 32 + 69;	//[100,70]
		playerStat->strength = rand() % 50 + 1;	//[50,1]
		playerStat->magicSkill = rand() % 20 + 50;	//(50,80)
	    playerStat->dexterity = rand() % 100 + 1;	//[100,1] 					rand() % (max - min + 1) + min
	}
	else if(playerStat->type == 2)
	{
		time_t t;						//call stat change for tile detection thinger CODE
		srand((unsigned) time(&t));
		
		int sum = 300;
		
		playerStat->life_points = 100;

		while(sum >= 300) // for human
		{
			playerStat->smartness = rand() % 100 + 1;
			playerStat->luck = rand() % 100 + 1;
			playerStat->strength = rand() % 100 + 1;	// stat for human
			playerStat->magicSkill = rand() % 100 + 1;
	   		playerStat->dexterity = rand() % 100 + 1;
	   		
	   		sum = (playerStat->smartness) + (playerStat->strength) + (playerStat->magicSkill) + (playerStat->luck) + (playerStat->dexterity);
		}

	}
	else if(playerStat->type == 3) // ogre
	{
		int sum = 51;
		
		playerStat->life_points = 100;
		playerStat->magicSkill = 0;
		playerStat->strength = rand() % 22 + 79;
		playerStat->dexterity = rand() % 22 + 79;
		   
		while(sum >= 50)
		{
			playerStat->smartness = rand() % 21;
			playerStat->luck = rand() % 50;	//stats for ogre
						
			sum = (playerStat->luck) + (playerStat->smartness);
		}
	}
	else if(playerStat->type == 4) // wizard
	{
		playerStat->life_points = 100;
		playerStat->luck = rand() % 52 + 49;		//stats for wizard
		playerStat->smartness = rand() % 12 + 89;
		playerStat->strength = rand() % 21 + 1;
		playerStat->magicSkill = rand() % 22 + 79;
	    playerStat->dexterity = rand() % 100 + 1;
	}
}

void boardSlots(int numPlayers)
{
	time_t t;
	srand((unsigned) time(&t));	//fill slot type into slot board randomly (1 - 3); 1 level ground, 2 hill, 3 city

	for(int i = 0; i < BOARD_SIZE; i++)
	{
		for(int j = 0; j < BOARD_SIZE; j++)
		{
			board[i][j].slotType = rand() % 3 + 1;		//random numbers [1,3]
			
			for(int c = 0; c < numPlayers; c++)
			{
				board[i][j].playerIndexInArray[c] = 8;		//initialises all stored players to 8 (empty)
			}
		}
	}
}

void playerSlots(int numPlayers, struct player playerArray[])
{ 
	time_t t;						
	srand((unsigned) time(&t));

	int playerIndex[numPlayers], i, j, tmp;

	for(i = 0; i < numPlayers; i++)
	{
		playerIndex[i] = i;			//create list of player array index that are in play
	}

	for(int c = 0; c < numPlayers; c++)
	{
		int x = 0;

		i = rand() % BOARD_SIZE;
		j = rand() % BOARD_SIZE;

		while(board[i][j].playerIndexInArray[x] != 8)		//if a players is already in a slot, it will cycle through the slot's space to fit them into it
		{
			++x;
		}

		board[i][j].playerIndexInArray[x] = c;			//puts player's location into the board

		playerArray[c].row = i;
		playerArray[c].column = j;							//sets the players coordinates
		playerArray[c].slotType = board[i][j].slotType;

		setStat(&playerArray[c], playerArray[c].slotType);		//calls the setstat function
	}
}

void setStat(struct player *slotPlayer, int slotType)	//cant't go below 0 (ie  not negative)
{
	if(slotType == 2)						//if their last slot type is a hill, adjust accordingly
	{
		if(slotPlayer->dexterity < 50 )
		{
			if(slotPlayer->strength - 10 <= 0)
			{
				slotPlayer->originalStrength = slotPlayer->strength;        //if reduced below zero, store original
			}

			slotPlayer->strength -= 10;
		}
		else if(slotPlayer->dexterity >= 60)
		{
			slotPlayer->strength += 10;
		}
	}
	else if(slotType == 3)						//if their slot type is a city
	{
		if(slotPlayer->smartness > 60)
		{
			slotPlayer->magicSkill += 10;
		}
		else if(slotPlayer->smartness <= 50)
		{
 	        if(slotPlayer->dexterity - 10 <= 0)
			{
				slotPlayer->originalDexterity = slotPlayer->dexterity;        //if reduced below zero, store original value
			}

			slotPlayer->dexterity -= 10;
		}
	}

	if(slotPlayer->strength < 0)
	{
		slotPlayer->strength = 0;				//if any stat becomes less than zero, set it to zero
	}

	if(slotPlayer->dexterity < 0)
	{
		slotPlayer->dexterity = 0;
	}
	
	if(slotPlayer->magicSkill < 0)
	{
		slotPlayer->magicSkill = 0;
	}

}

void checkIfMove(struct player *currPlayer, bool *up, bool *down, bool *left, bool *right)
{
	if(!(currPlayer->row >= BOARD_SIZE - 1))
	{
		*down = true;								//if they can move in a direction, then the directional boolean 
	}

	if(!(currPlayer->row <= 0))
	{
		*up = true;
	}

	if(!(currPlayer->column >= BOARD_SIZE - 1))
	{
		*right = true;
	}

	if(!(currPlayer->column <= 0))
	{
		*left = true;
	}
}

void movePlayer(struct player *player, int moveChoice, int playerNum)
{
	resetStat(player);

	int x = 0, i = player->row, j = player->column;

	while(board[i][j].playerIndexInArray[x] != playerNum)
	{
		x++;
	}

	board[i][j].playerIndexInArray[x] = 8;

	int r = 0;														//1 up 2 down 3 left 4 right

	if(moveChoice == 1)
	{
		player->row -= 1;

		while(board[i - 1][j].playerIndexInArray[r] != 8)		//each one will change the position in the board accordingly and also the player's coordinates
		{
			++r;
		}

		board[i - 1][j].playerIndexInArray[r] = playerNum;
	}

	if(moveChoice == 2)
	{
		player->row += 1;

		while(board[i + 1][j].playerIndexInArray[r] != 8)
		{
			++r;
		}

		board[i + 1][j].playerIndexInArray[r] = playerNum;
	}

	if(moveChoice == 3)
	{
		player->column -= 1;

		while(board[i][j - 1].playerIndexInArray[r] != 8)
		{
			++r;
		}

		board[i][j - 1].playerIndexInArray[r] = playerNum;
	}

	if(moveChoice == 4)
	{
		player->column += 1;

		while(board[i][j + 1].playerIndexInArray[r] != 8)
		{
			++r;
		}

		board[i][j + 1].playerIndexInArray[r] = playerNum;
	}

	checkSlot(player);					//checks or the new slot
}

void resetStat(struct player *slotPlayer)
{
	int slotType = slotPlayer->slotType;			//resetting their stats based on whether they were in a certain slot, or if their stat had been reduced past 0

	if(slotType == 2)
	{
		if(slotPlayer->dexterity < 50 )
		{
			if(slotPlayer->strength <= 0)
			{
				slotPlayer->strength = slotPlayer->originalStrength;
			}
			else
			{
				slotPlayer->strength += 10;				//if was less than or equal to 0; set stat based on originalStrength
			}
		}
		else if(slotPlayer->dexterity >= 60)
		{
			slotPlayer->strength -= 10;
		}
	}
	else if(slotType == 3)
	{
		if(slotPlayer->smartness > 60)
		{
			slotPlayer->magicSkill -= 10;
		}
		else if(slotPlayer->smartness <= 50)
		{
			if(slotPlayer->dexterity <= 0)
			{
				slotPlayer->dexterity = slotPlayer->originalDexterity;
			}
			else
			{
				slotPlayer->dexterity += 10;		//if was less than or equal to 0; set stat based on originalDexterity
			}
		}
	}
}

void checkSlot(struct player *slotPlayer)
{
	slotPlayer->slotType = board[slotPlayer->row][slotPlayer->column].slotType;			//set player's new sotType

	if(slotPlayer->slotType == 1)
	{
		//level ground
	}
	else if(slotPlayer->slotType == 2)
	{
		setStat(slotPlayer, slotPlayer->slotType);		//hill
	}
	else if(slotPlayer->slotType == 3)
	{
		setStat(slotPlayer, slotPlayer->slotType);	//city
	}
}

void findDead(struct player players[], int numPlayers, int *activePlayers)
{
	for(int i = 0; i < numPlayers; i++)
	{
		if((players[i].life_points <= 0) && (players[i].type != 8))			//if an active player is dead (life_points <= 0) they are removed
		{
			removePlayer(players, i, activePlayers);
		}
	}
}

void removePlayer(struct player player[], int i, int *activePlayers)
{
	player[i].type = 8;			//set player type to 8

	int x = 0;

	while(board[player[i].row][player[i].column].playerIndexInArray[x] != i)		//player to remove is located
	{ 
		x++;
	}
	
	board[player[i].row][player[i].column].playerIndexInArray[x] = 8;		//set player reference in board to 8

	*activePlayers = *activePlayers - 1;					//decrement the number of active players
}

void checkMagicCapabilities(struct player *player, bool *canMagicAttack)
{
	if(player->magicSkill + player->smartness > 150)
	{
		*canMagicAttack = true;							//if they can perform magic, this is set to true
	}
}

void checkPlayersToAttack(struct player player, int nearAttack[], int distanceAttack[], int magicAttack[], bool *canNearAttack, bool *canDistanceAttack, bool *canMagicAttack, int numPlayers, int loop)
{
	int i, j, c, x;

	if(*canMagicAttack)
	{
		x = 0;

		for(i = 0; i < BOARD_SIZE; i++)
		{
			for(j = 0; j < BOARD_SIZE; j++)
			{
				for(c = 0; c < numPlayers; c++)
				{
					if(board[i][j].playerIndexInArray[c] != 8 && board[i][j].playerIndexInArray[c] != loop)
					{
						magicAttack[x] = board[i][j].playerIndexInArray[c];		//finds attackable players for magic attack
						x++;

						numMagic++;
					}
				}
			}
		}

		if(x == 0)
		{
			*canMagicAttack = false;
		}
	}

	x = 0;								//checking for distance attack

	for(i = 0; i < BOARD_SIZE; i++)
	{
		for(j = 0; j < BOARD_SIZE; j++)
		{
			int I = i - player.row;
			int J = j - player.column;

			if(I < 0)
			{
				I = -I;
			}

			if(J < 0)
			{
			 	J = -J;
			}

			if( !( (I + J) >= REQ_DISTANCE || (I + J) <= 1))			//if the absolute value of their coordinates differences are added together, this value tells us if they are in range
			{																	//they are in range if this value is < 5 but > 1
				for(c = 0; c < numPlayers; c++)
				{
					if(board[i][j].playerIndexInArray[c] != 8 && board[i][j].playerIndexInArray[c] != loop)
					{
						distanceAttack[x] = board[i][j].playerIndexInArray[c];			//finds  players to distance attack
						x++;

						numDistance++;

						*canDistanceAttack = true;
					}
				}
			}
		}
	}

	x = 0;								//checking for near attack

	int I, J;

	if(!(player.row == 0))					//attack up
	{
		I = player.row - 1;					//each of the following will check for a player to attack in the up, down, left, right and same slot directions
		J = player.column;

		for(c = 0; c < numPlayers; c++)
		{
			if(board[I][J].playerIndexInArray[c] != 8 && board[I][J].playerIndexInArray[c] != loop)
			{
				nearAttack[x] = board[I][J].playerIndexInArray[c];			//finds players to near attack
				x++; 

				numNear++;

				*canNearAttack = true;
			}
		}
	}

	if(!(player.row == BOARD_SIZE - 1))					//attack down
	{
		I = player.row + 1;
		J = player.column;

		for(c = 0; c < numPlayers; c++)
		{
			if(board[I][J].playerIndexInArray[c] != 8 && board[I][J].playerIndexInArray[c] != loop)
			{
				nearAttack[x] = board[I][J].playerIndexInArray[c];
				x++; 

				numNear++;

				*canNearAttack = true;
			}
		}
	}

	if(!(player.column == 0))					//attack left
	{
		I = player.row;
		J = player.column - 1;

		for(c = 0; c < numPlayers; c++)
		{
			if(board[I][J].playerIndexInArray[c] != 8 && board[I][J].playerIndexInArray[c] != loop)
			{
				nearAttack[x] = board[I][J].playerIndexInArray[c];
				x++; 

				numNear++;

				*canNearAttack = true;
			}
		}
	}

	if(!(player.column == BOARD_SIZE - 1))					//attack right
	{
		I = player.row;
		J = player.column + 1;

		for(c = 0; c < numPlayers; c++)
		{
			if(board[I][J].playerIndexInArray[c] != 8 && board[I][J].playerIndexInArray[c] != loop)
			{
				nearAttack[x] = board[I][J].playerIndexInArray[c];
				x++; 

				numNear++;

				*canNearAttack = true;
			}
		}
	}

	I = player.row;
	J = player.column;				//attack same slot

	for(c = 0; c < numPlayers; c++)
	{
		if(board[I][J].playerIndexInArray[c] != 8 && board[I][J].playerIndexInArray[c] != loop)
		{
			nearAttack[x] = board[I][J].playerIndexInArray[c];
			x++; 

			numNear++;

			*canNearAttack = true;
		}
	}
	
}

void nearAttack(struct player *attacker, struct player *defender)
{
	if(defender->strength <= 70)
	{
		defender->life_points -= (0.5 * attacker->strength); 			//performs near attack based on criteria
	}
	else
	{
		attacker->life_points -= (0.3 * defender->life_points);
	}
}

void distanceAttack(struct player *attacker, struct player *defender)
{
	if(attacker->dexterity > defender->dexterity)
	{
		defender->life_points -= (0.3 * attacker->strength);	//adjust if dexterity is greater than defense dexterity	
	}
}

void magicAttack(struct player *attacker, struct player *defender)
{
	defender->life_points -= ((0.5 * attacker->magicSkill) + (0.2 * attacker->smartness));		//performs magic attack on desired player
}

bool inputChecker(int value, int array[], int arraySize)			//checks to see if correct input was entered
{
	bool check = false;
	
	for(int i = 0; i < arraySize; i++)
	{
		if(array[i] == value)
		{
			check = true;					//if the player number entered is on the attackable list, then the function returns true
		}
	}

	return check;
} 
