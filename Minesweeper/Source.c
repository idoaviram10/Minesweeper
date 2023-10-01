//This program simulates the famous video game Minesweeper.
//Ido Aviram, 01 December 2020

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

// Algorithm:
// Input: Select the board size from the main menu and then in each turn, the user selects a new slot that he wants to reveal.
// Output: The board status after each turn and at the end a message of loss or victory
// 1. Ask the user to choose the board size from the menu.
// 2. The game begins and the user needs to reveal a new slot in each turn, and try not to reveal a slot with a mine.
// 3. Print out the the board status after each turn.
// 4. At the end, the program will print out whether the user has won or lost.


#define MAXSIZE 22 //The maximus number of rows and columns in the array
#define VALUESAVE 1 //The size of the array that will retain value that counts moves made in recursion.


//All the functions we will use in this program:
void menu();
void emptyGameBoard(char gameBoard[][MAXSIZE], int rows, int cols);
void rndMines(char gameBoard[][MAXSIZE], int rows, int cols);
int minesAroud(char gameBoard[][MAXSIZE], int i, int j, int maxRow, int maxCol);
void initGameBoard(char gameBoard[][MAXSIZE], int rows, int cols);
void printGameBoard(char gameBoard[][MAXSIZE], int rows, int cols);
void initUserBoard(char userBoard[][MAXSIZE], int rows, int cols);
void printUserBoard(char userBoard[][MAXSIZE], int rows, int cols);
int revealX(char gameBoard[][MAXSIZE], char userBoard[][MAXSIZE], int rows, int cols, int chooseCol);
void winOrLose(char userBoard[][MAXSIZE], char gameBoard[][MAXSIZE], int mone, int chooseRow, int chooseCol, int rows, int cols, int maxMoves);
void eachTurn(char userBoard[][MAXSIZE], char gameBoard[][MAXSIZE], int rows, int cols, int maxMoves, int arrMone[]);
void openFreeSpaces(char userBoard[][MAXSIZE], char gameBoard[][MAXSIZE], int maxRow, int maxCol, int i, int j, int arrMone[]);
int maxMovesBoardSize(int rows, int cols);


void main()
{
	//Declaration of variables:
	int choose; //Input by user for select board size.
	char gameBoard[MAXSIZE][MAXSIZE]; //This is the array that contains the information about the location of the mines.
	char userBoard[MAXSIZE][MAXSIZE]; //This is the array that the user sees after each turn.
	int arrMone[VALUESAVE]; //This is the one-place array, which will keep the number of squares revealed in the recursion.
	int rows, cols; //Each menu selection will have a different board size. A certain number of rows and a certain number of columns.
	int maxMoves; //For each board size selected, the maximum number of moves in each game is different. It is the variable that will contain this number.

	menu(); //A function that prints the game menu.

	//Getting input from user:
	scanf("%d", &choose);

	//Check if the choice entered by user is valid, if not, the user has to choose another option.
	while (choose < 0 || choose > 4)
	{
		printf("\nPlease enter a valid number from the menu!\n");
		//The menu of the possible board sizes:
		menu();
		//Getting input from user:
		scanf("%d", &choose);
	}

	if (choose == 0)
		printf("\nGoodbye!\n\n");

	else
	{
		if (choose == 1)
		{
			rows = 8;
			cols = 8;
		}
		else if (choose == 2)
		{
			rows = 12;
			cols = 12;
		}
		else if (choose == 3)
		{
			rows = 15;
			cols = 15;
		}
		else
		{
			printf("\nPlease type the size of the board you want, The maximum size is 22X22: ");
			scanf("%d %d", &rows, &cols);
			while (rows < 1 || rows > 22 || cols < 1 || cols > 22)
			{
				printf("\nInvalid size! Please choose again, The maximum size is 22X22: ");
				scanf("%d %d", &rows, &cols);
			}
		}

		emptyGameBoard(gameBoard, rows, cols); //A function that fills the array that contains information on mines, with spaces.
		rndMines(gameBoard, rows, cols); //A function that selects random places in the array to place mines in them.
		initGameBoard(gameBoard, rows, cols); //This function initializes the array with information about the number of mines around.
		initUserBoard(userBoard, rows, cols); //A function that fills the array that the user sees after each turn, with the char 'X'.
		maxMoves = maxMovesBoardSize(rows, cols); //A function that calculates the maximum number of moves per board size. 
												  //With the help of this function we will eventually know whether the user has won or lost.
		eachTurn(userBoard, gameBoard, rows, cols, maxMoves, arrMone); //A function that performs each turn in the game with user instructions.
	}
}

void menu()
{
	//A function that prints the game menu.

	//The menu of the possible board sizes:
	printf("Welcome to Minesweeper!\n\n\n");
	printf("Please choose one of the following options and enter it's number:\n\n");
	printf("1 - for size 8X8 \n\n2 - for size 12X12 \n\n3 - for size 15X15 \n\n4 - for custom size \n\n0 - Exit \n\n");
}

int maxMovesBoardSize(int rows, int cols)
{
	//A function that calculates the maximum number of moves per board size. 
	//With the help of this function we will eventually know whether the user has won or lost.

	//Declaration of variables:
	int maxMoves; //The variable to be returned to the Main function.
	int multRowCols = rows * cols; //A variable that will contain the result of multiplication between the number of rows and number of columns for each board size.
	int sqrtMult = 0; //A variable that will contain the square root value of the multiplication.

	if (rows == 1 && cols == 1) //If the board size is 1X1, the maximum number of moves is 1.
		maxMoves = 1;
	else
	{
		sqrtMult = sqrt(multRowCols);
		maxMoves = (multRowCols)-sqrtMult;
	}
	return maxMoves;
}

void emptyGameBoard(char gameBoard[][MAXSIZE], int rows, int cols)
{
	//A function that fills the array that contains information on mines, with spaces.

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			gameBoard[i][j] = ' ';
}

void rndMines(char gameBoard[][MAXSIZE], int rows, int cols)
{
	//A function that selects random places in the array to place mines in them.

	//Declaration of variables:
	int randRow, randCol; //Variables that will contain a random row and column value.
	int multRowCols = rows * cols; //A variable that will contain the result of multiplication between the number of rows and number of columns for each board size.
	int numOfMines = sqrt(multRowCols); //The number of mines in each game board is the square root value of the multiplication result between the rows and columns.
	int mone = 0; //Count how many random places have already been selected.

	srand(time(NULL));

	while (mone < numOfMines) //calculates the random place of each mine on the game board.
	{
		randRow = rand() % rows; //we want a random number between 0 to the number of rows-1.
		randCol = rand() % cols; //we want a random number between 0 to the number of columns-1.

		//Checks if the place has not been selected before.
		if (gameBoard[randRow][randCol] != '*')
		{
			gameBoard[randRow][randCol] = '*';
			mone++;
		}
	}
}

void initGameBoard(char gameBoard[][MAXSIZE], int rows, int cols)
{
	//This function initializes the array with information about the number of mines around.

	//Declaration of variable:
	int numOfMines; //This variable will contain how many mines around.

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{
			if (gameBoard[i][j] == ' ')
			{
				numOfMines = minesAroud(gameBoard, i, j, rows, cols);
				if (numOfMines > 0)
				{
					gameBoard[i][j] = numOfMines + 48;
					//If the value is greater than 0, we will add to the returned value 48 because we want to convert the value from int to char.
				}
			}
		}
}

int minesAroud(char gameBoard[][MAXSIZE], int i, int j, int maxRow, int maxCol)
{
	//This function checks how many mines there are around each slot.

	//Declaration of variable:
	int mone = 0; //Count how many mines around each slot.

	if (gameBoard[i - 1][j - 1] == '*' && i - 1 >= 0 && j - 1 >= 0)
		mone++;
	if (gameBoard[i - 1][j] == '*' && i - 1 >= 0)
		mone++;
	if (gameBoard[i - 1][j + 1] == '*' && i - 1 >= 0 && j + 1 < maxCol)
		mone++;
	if (gameBoard[i][j - 1] == '*' && j - 1 >= 0)
		mone++;
	if (gameBoard[i][j + 1] == '*' && j + 1 < maxCol)
		mone++;
	if (gameBoard[i + 1][j - 1] == '*' && i + 1 < maxRow && j - 1 >= 0)
		mone++;
	if (gameBoard[i + 1][j] == '*' && i + 1 < maxRow)
		mone++;
	if (gameBoard[i + 1][j + 1] == '*' && i + 1 < maxRow && j + 1 < maxCol)
		mone++;
	return mone;
}

void printGameBoard(char gameBoard[][MAXSIZE], int rows, int cols)
{
	//A function that prints the contents of the array that contains the information about the location of the mines.

	printf("     ");
	for (int k = 0; k < cols; k++)
	{
		if (k < 10)
			printf("%d   ", k);
		else
			printf("%d  ", k);
	}
	printf("\n");
	printf("   ");
	for (int k = 0; k < 2 * cols; k++)
		printf("__");
	printf("\n");

	for (int i = 0; i < rows; i++)
	{
		if (i < 10)
			printf("%d  |", i);
		else
			printf("%d |", i);
		for (int j = 0; j < cols; j++)
			printf(" %c |", gameBoard[i][j]);
		printf("\n");
	}
	printf("   ");
	for (int k = 0; k < 2 * cols; k++)
		printf("__");
	printf("\n");
}

void initUserBoard(char userBoard[][MAXSIZE], int rows, int cols)
{
	//A function that fills the array that the user sees after each turn, with the char 'X'.

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			userBoard[i][j] = 'X';
}

void printUserBoard(char userBoard[][MAXSIZE], int rows, int cols)
{
	//A function that prints the contents of the array that the user sees after each turn.

	printf("     ");
	for (int k = 0; k < cols; k++)
	{
		if (k < 10)
			printf("%d   ", k);
		else
			printf("%d  ", k);
	}
	printf("\n");
	printf("   ");
	for (int k = 0; k < 2 * cols; k++)
		printf("__");
	printf("\n");

	for (int i = 0; i < rows; i++)
	{
		if (i < 10)
			printf("%d  |", i);
		else
			printf("%d |", i);
		for (int j = 0; j < cols; j++)
			printf(" %c |", userBoard[i][j]);
		printf("\n");
	}

	printf("   ");
	for (int k = 0; k < 2 * cols; k++)
		printf("__");
	printf("\n");
}

int revealX(char gameBoard[][MAXSIZE], char userBoard[][MAXSIZE], int rows, int cols, int chooseCol)
{
	//Addition of automatic testing to the game (Progress of X steps at user request):

	//Declaration of variables:
	int i = 0; //Variable for the loop
	int j = 0; //Variable for the loop
	int mone = 0; //Indicates progress according to the number of steps the user requested

	for (i = 0; i < rows && mone < chooseCol; i++)
	{
		for (j = 0; j < cols && mone < chooseCol; j++)
			if ((userBoard[i][j] == 'X' || userBoard[i][j] == 'f' || userBoard[i][j] == 'F') && (gameBoard[i][j] != '*'))
			{
				userBoard[i][j] = gameBoard[i][j];
				mone++;
			}
	}
	return mone;
}

void winOrLose(char userBoard[][MAXSIZE], char gameBoard[][MAXSIZE], int mone, int chooseRow, int chooseCol, int rows, int cols, int maxMoves)
{
	//This function Checks if the user has won or lost:

	if (mone < maxMoves)
	{
		printf("\n--------------------------------\n");
		printf("  You've hit a bomb! Game over!  ");
		printf("\n--------------------------------\n\n");
		printGameBoard(gameBoard, rows, cols);
		printf("\n\----------------------\n");
		printf("  LOOOOOOOOOOOOSER!  ");
		printf("\n----------------------\n\n");
	}
	else
	{
		userBoard[chooseRow][chooseCol] = gameBoard[chooseRow][chooseCol];
		printf("\n--------------------------------\n");
		printf("  You win! VICTORY!!!   ");
		printf("\n--------------------------------\n\n");
		printGameBoard(gameBoard, rows, cols);
	}
}

void eachTurn(char userBoard[][MAXSIZE], char gameBoard[][MAXSIZE], int rows, int cols, int maxMoves, int arrMone[])
{
	//A function that performs each turn in the game with user instructions.

	//Declaration of variables:
	int chooseRow, chooseCol; //Variables that contain the user's choice in each new turn.
	int mone = 0; //Indicates that the user has won in the end.
	char markOrReveal; //In each turn the user decides whether to reveal a square or just mark it. This variable will contain the user's decision.


	printUserBoard(userBoard, rows, cols);

	while (mone < maxMoves)
	{
		//Getting input from user (row, column and the letter 'O' or 'F'):
		printf("\nPlease enter your move - row and column and the letter O or F (O to reveal and F to mark):   ");
		//fseek(stdin, 0, SEEK_END);
		scanf(" %d %d", &chooseRow, &chooseCol);
		printf("\n");

		//Addition of automatic testing to the game (Progress of X steps at user request):
		if (chooseRow == -1 && chooseCol > 0)
		{
			mone = mone + revealX(gameBoard, userBoard, rows, cols, chooseCol);
			printUserBoard(userBoard, rows, cols);
		}

		else
		{
			scanf(" %c", &markOrReveal);
			printf("\n");

			if (chooseRow >= 0 && chooseRow < rows && chooseCol >= 0 && chooseCol < cols)
			{
				if (markOrReveal == 'O' || markOrReveal == 'o')
				{

					if (gameBoard[chooseRow][chooseCol] != '*')
					{

						//Check if the choice entered by user is valid, if not, the user has to choose another option.
						if (userBoard[chooseRow][chooseCol] == 'X' || userBoard[chooseRow][chooseCol] == 'F')
						{
							if (gameBoard[chooseRow][chooseCol] == ' ')
							{
								arrMone[0] = 0;
								openFreeSpaces(userBoard, gameBoard, rows, cols, chooseRow, chooseCol, arrMone);
								mone = mone + arrMone[0];
							}
							else
							{
								userBoard[chooseRow][chooseCol] = gameBoard[chooseRow][chooseCol];
								mone++;
							}
							printUserBoard(userBoard, rows, cols);
						}
						else
						{
							printf("\nInvalid move, please enter valid choice!\n\n");
							printUserBoard(userBoard, rows, cols);
						}
					}
					else
						break;
				}

				else if (markOrReveal == 'F' || markOrReveal == 'f')
				{
					if (userBoard[chooseRow][chooseCol] == 'X')
					{
						userBoard[chooseRow][chooseCol] = 'F';
						printUserBoard(userBoard, rows, cols);
					}
					else
					{
						printf("\nYou have already marked this place!\n\n");
						printUserBoard(userBoard, rows, cols);
					}
				}
			}
			else
			{
				printf("\nInvalid move, please enter valid choice!\n\n");
				printUserBoard(userBoard, rows, cols);
			}
		}
	}

	winOrLose(userBoard, gameBoard, mone, chooseRow, chooseCol, rows, cols, maxMoves);
}

void openFreeSpaces(char userBoard[][MAXSIZE], char gameBoard[][MAXSIZE], int maxRow, int maxCol, int i, int j, int arrMone[])
{
	//This is a recursive function. 
	//Whenever the user reveals an empty square, all the empty squares in its vicinity will also open and be surrounded by squares with numbers.

	if (gameBoard[i][j] == ' ' && (userBoard[i][j] == 'X' || userBoard[i][j] == 'F'))
	{

		userBoard[i][j] = ' ';
		arrMone[0] = arrMone[0] + 1;

		if (i - 1 >= 0 && j - 1 >= 0 && (userBoard[i - 1][j - 1] == 'X' || userBoard[i - 1][j - 1] == 'F'))
			openFreeSpaces(userBoard, gameBoard, maxRow, maxCol, i - 1, j - 1, arrMone);
		if (i - 1 >= 0 && (userBoard[i - 1][j] == 'X' || userBoard[i - 1][j] == 'F'))
			openFreeSpaces(userBoard, gameBoard, maxRow, maxCol, i - 1, j, arrMone);
		if (i - 1 >= 0 && j + 1 < maxCol && (userBoard[i - 1][j + 1] == 'X' || userBoard[i - 1][j + 1] == 'F'))
			openFreeSpaces(userBoard, gameBoard, maxRow, maxCol, i - 1, j + 1, arrMone);
		if (j - 1 >= 0 && (userBoard[i][j - 1] == 'X' || userBoard[i][j - 1] == 'F'))
			openFreeSpaces(userBoard, gameBoard, maxRow, maxCol, i, j - 1, arrMone);
		if (j + 1 < maxCol && (userBoard[i][j + 1] == 'X' || userBoard[i][j + 1] == 'F'))
			openFreeSpaces(userBoard, gameBoard, maxRow, maxCol, i, j + 1, arrMone);
		if (i + 1 < maxRow && j - 1 >= 0 && (userBoard[i + 1][j - 1] == 'X' || userBoard[i + 1][j - 1] == 'F'))
			openFreeSpaces(userBoard, gameBoard, maxRow, maxCol, i + 1, j - 1, arrMone);
		if (i + 1 < maxRow && (userBoard[i + 1][j] == 'X' || userBoard[i + 1][j] == 'F'))
			openFreeSpaces(userBoard, gameBoard, maxRow, maxCol, i + 1, j, arrMone);
		if (i + 1 < maxRow && j + 1 < maxCol && (userBoard[i + 1][j + 1] == 'X' || userBoard[i + 1][j + 1] == 'F'))
			openFreeSpaces(userBoard, gameBoard, maxRow, maxCol, i + 1, j + 1, arrMone);
	}
	else if (gameBoard[i][j] != '*')
	{
		userBoard[i][j] = gameBoard[i][j];
		arrMone[0] = arrMone[0] + 1;
	}

}

