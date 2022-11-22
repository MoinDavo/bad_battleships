//TODO: don't close window after the game was won
//TODO: add some kind of multiplayer functionality? local 2P (taking turns on machine)?

#include <iostream>
#include "color.hpp"

const int FIELDSIZE = 16;
int field[FIELDSIZE][FIELDSIZE];

void clearConsole() {
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}

void printField() {
	for (int i = 0; i < FIELDSIZE; i++)
	{
		std::cout << "\n";
		for (int j = 0; j < FIELDSIZE; j++)
		{
			if (field[i][j] == 0)
			{
				std::cout << dye::blue(field[i][j]);
			}
			else if (field[i][j] == 5)
			{
				std::cout << dye::green(field[i][j]);
			}
			else if (field[i][j] == 1)
			{
				std::cout << dye::red(field[i][j]);
			}
			else
			{
				std::cout << field[i][j];
			}
			std::cout << " ";
		}
	}
	std::cout << "\n \n";
}

bool checkShip(int y, int x, int length, bool isHorizontal) { //check is a Ship is present along 1 axis
	bool output = false;

	if (isHorizontal)
	{
		for (int i = x; i < length + x; i++)
		{
			if (field[y][i] == 5)
			{
				output = true;
			}
		}
	}
	else
	{
		for (int i = y; i < length + y; i++)
		{
			if (field[i][x] == 5)
			{
				output = true;
			}
		}
	}
	
	return output;
}

void placeShip(int y, int x, int length, bool isHorizontal) {

	if (checkShip(y, x, length, isHorizontal))
	{
		std::cout << dye::red("Ship couldn't be placed (antother Ship is in the way)! \n");
		printField();
		return;
	}

	if (isHorizontal)
	{

		if ( (y + length) > FIELDSIZE) //catch if ship is (partialy) outside the field
		{
			std::cout << dye::red("Ship couldn't be placed (to long/ to far right)! \n");
			printField();
			return;
		}
		else
		{
			for (int i = x; i < length + x; i++)
			{
				field[y][i] = 5; //5 to show theres a ship (alphanumeric S = 5)
			}
		}
	}
	else {
		if ( (x + length) > FIELDSIZE) //catch if ship is (partialy) outside the field
		{
			std::cout << dye::red("Ship couldn't be placed (to long/to far down)! \n");
			printField();
			return;
		}
		else {
			for (int i = y; i < length + y; i++)
			{
				field[i][x] = 5; //5 to show theres a ship (alphanumeric S = 5)
			}
		}
	}

	//output
	std::cout << "Ship placed of length: " << length;
	printField();
}

void shoot(int y, int x) {
	if (field[y][x] == 5)
	{
		std::cout << dye::aqua("Ship hit! Nice aim!");
		field[y][x] = 1; //1 to show shot hit (1 = true)
	}
	else if (field[y][x] == 1 || field[y][x] == 9)
	{
		std::cout << dye::red("Already shot there! Getting forgetful?");
	}
	else if (field[y][x] == 0)
	{
		std::cout << dye::aqua("Ship missed! Better luck next time");
		field[y][x] = 9; //9 to show a missed shot (9 = nine ~ nein)
	}
	printField();
}

bool gameEnd() {
	int counter = 0;

	for (int i = 0; i < FIELDSIZE; i++)
	{
		if ((checkShip(i, 0, FIELDSIZE, true)))
		{
			counter++;
		}
	}
	if (counter > 0)
	{
		return false;
	}
	else if (counter == 0)
	{
		std::cout << dye::yellow_on_light_red("Congratulations you've won!");
		return true;
	}

}

int main()
{
	int userInput;
	int userX;
	int userY;
	int userLength;
	bool userHorizontal;

	for (int i = 0; i < FIELDSIZE; i++)
	{
		for (int j = 0; j < FIELDSIZE; j++)
		{
			field[i][j] = 0;
		}
	}

	printField(); //print the first field just to show the size

	/* testing for functions
	placeShip(1, 2, 5, false);
	placeShip(0, 0, 1, false);
	placeShip(1, 6, 2, false);
	placeShip(3, 3, 4, false);
	placeShip(0, 15, 10, false); //test for out of bounds ship -> it works
	
	shoot(0, 0);
	shoot(15, 15);
	shoot(15, 15);
	*/

	while (true)
	{
		std::cout << "next action: \n";
		std::cout << "[1] place ship \n";
		std::cout << "[2] SHOOT!!! \n";
		//std::cout << "[3] show field \n";
		std::cout << "[0] end game \n ";
		std::cin >> userInput;

		switch (userInput)
		{
		case 1:
			std::cout << "\tinput the ship's length: \n\t";
			std::cin >> userLength;
			while (userLength > FIELDSIZE)
			{
				std::cout << dye::red("\tShip larger than field! FIELDSIZE=") << FIELDSIZE << ("\n\t");
				std::cin >> userLength;
			}

			std::cout << "\tinput the ship's X-coordinate: \n\t";
			std::cin >> userX;

			std::cout << "\tinput the ship's Y-coordinate: \n\t";
			std::cin >> userY;

			std::cout << "\tShall the ship be horizontal?: [0][1]\n\t";
			std::cin >> userHorizontal;

			clearConsole();
			placeShip(userY-1, userX-1, userLength, userHorizontal);
			break;

		case 2:
			std::cout << "\tinput the shot's X-coordinate: \n\t";
			std::cin >> userX;

			std::cout << "\tinput the shot's Y-coordinate: \n\t";
			std::cin >> userY;

			shoot(userY-1, userX-1);
			break;

		case 3:
			printField();
			break;

		case 0:
			return 0;

		default:
			std::cout << dye::red("NOT A VALID INPUT!");
			break;
		}

		if (gameEnd())
		{
			return 0;
		}
	}
}