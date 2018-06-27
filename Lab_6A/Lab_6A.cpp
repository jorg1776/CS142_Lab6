// Lab_6A.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <iomanip>
using namespace std;

const int seedVal = 42;
const int numOfSlots = 9;
const int numOfRows = 12;

void DisplayMenu()
{
	cout << "Menu: Please select one of the following options :" << endl;
	cout << "\t1 - Drop a single chip into one slot" << endl;
	cout << "\t2 - Drop multiple chips into one slot" << endl;
	cout << "\t3 - Show the options menu" << endl;
	cout << "\t4 - Quit the program" << endl;
	cout << endl;
}

int ComputeWinnings(int slotNumber)
{
	int slotValues [numOfSlots] = { 100, 500, 1000, 0, 10000, 0, 1000, 500, 100 };
	return slotValues[slotNumber];
}

float CalculateAvgEarnings(float numChips, float totalEarnings)
{
	return totalEarnings / numChips;
}

void DisplayWinnings(int numChips, float winnings)
{
	cout << setprecision(2) << fixed;
	if (numChips == 1)
	{
		cout << "Winnings: $" << winnings << endl;
		cout << endl;
	}
	else if (numChips > 1)
	{
		cout << "Total winnings on " << numChips << " chips: $" << winnings << endl;
		float averageEarnings = CalculateAvgEarnings(numChips, winnings);
		cout << "Average winnings per chip: $" << averageEarnings << endl; //"setprecision(2) << fixed" is for rounding cents
		cout << endl;
	}
}

enum DropType
{
	Single,
	Multiple,
	Sequential
};

enum InputType
{
	OptionNumber,
	NumberOfChips,
	SlotSelection
};

bool ValidInput(int input, InputType type)
{
	switch (type)
	{
		case OptionNumber:
		{
			if (input < 1 || input > 4)
			{
				cout << "Invalid selection. Enter 3 to see options." << endl;
				cout << endl;
				return false;
			}
			else
				return true;
		}
		case NumberOfChips:
		{
			if (input <= 0)
			{
				cout << "Invalid number of chips." << endl;
				cout << endl;
				return false;
			}
			else
				return true;
		}
		case SlotSelection:
			if (input < 0 || input > (numOfSlots - 1))
			{
				cout << "Invalid slot." << endl;
				cout << endl;
				return false;
			}
			else
				return true;
		default:
			return false;
	}
}

int GetUserInput()
{
	int input;
	cin >> input;

	if (cin.fail()) //if input is not an int
	{
		cin.clear();
		cin.ignore(256, '\n');
		return -1; //returns -1 so ValidateInput will show error message
	}
	else
	{
		return input;
	}
}

int GetSlotNumber()
{
	int slotNumber;
	bool valid = false;

	do {
		cout << "Which slot do you want to drop the chip in (0-" << numOfSlots - 1 << ")? ";
		slotNumber = GetUserInput();

		cout << endl;

		if (!ValidInput(slotNumber, SlotSelection))
		{
			cout << "Invalid slot." << endl;
			cout << endl;
			valid = false;
		}
		else
			valid = true;

	} while (valid == false);

	return slotNumber;
}

int GetNumChips()
{
	cout << "How many chips do you want to drop (>0)? ";
	int numChips = GetUserInput();
	cout << endl;

	if (ValidInput(numChips, NumberOfChips) == false)
		return -1;
	else
		return numChips;
}

int TrackChip(float chipLocation, int numChips)
{
	float path [numOfRows + 1] = {};
	path[0] = chipLocation;

	//logic for when ball hits peg
	for (int j = 0; j < numOfRows; j++)
	{
		if (chipLocation > 0 && chipLocation < (numOfSlots - 1))
		{
			int direction = rand() % 2;

			switch (direction)
			{
				case 0:
					chipLocation += 0.5;
					path[j + 1] = chipLocation;
					break;
				case 1:
					chipLocation -= 0.5;
					path[j + 1] = chipLocation;
					break;
			}
		}
		else if (chipLocation == 0)
		{
			chipLocation += 0.5;
			path[j + 1] = chipLocation;
		}
		else if (chipLocation == (numOfSlots - 1))
		{
			chipLocation -= 0.5;
			path[j + 1] = chipLocation;
		}
	}

	//prints out path for single coin drop
	cout << setprecision(1) << fixed; //gives path output one decimal place
	if (numChips == 1)
	{
		cout << "Path: " << path << endl;
		return path[numOfRows];
	}
	else
		return path[numOfRows];
}

float DropChips(int numChips, DropType dropType)
{
	float winnings = 0;

	switch (dropType)
	{
		case Single:
		{
			int slotNumber = GetSlotNumber();
			cout << "*** Dropping chip into slot " << slotNumber << " ***" << endl;
			winnings = ComputeWinnings(TrackChip(slotNumber, numChips));
			break;
		}
		case Multiple:
		{
			int slotNumber = GetSlotNumber();
			for (int i = 0; i < numChips; i++)
			{
				winnings += ComputeWinnings(TrackChip(slotNumber, numChips));
			}

			break;
		}
		case Sequential:
		{
			int slotNumber = 0;
			int slotWinnings[numOfSlots] = {};
			int numSlotChipsDropped[numOfSlots] = {};

			for (int i = 0; i < numChips; i++)
			{
				numSlotChipsDropped[i]++;
				slotWinnings[slotNumber] += ComputeWinnings(TrackChip(slotNumber, numChips));

				if (slotNumber < (numOfSlots - 1))
					slotNumber++;
				else
					slotNumber = 0;
			}

			for (int j = 0; j < numOfSlots; j++)
			{
				if (numSlotChipsDropped[j] > 0)
				{
					cout << "Total winnings on slot " << j << " chips: $" << slotWinnings[j] << endl;
					float averageEarnings = CalculateAvgEarnings(numSlotChipsDropped[j], slotWinnings[j]);
					cout << "Average winnings per chip: $" << averageEarnings << endl; //"setprecision(2) << fixed" is for rounding cents
					cout << endl;
				}
			}
			break;
		}
	}
	return winnings;
}

void PlayGame(DropType dropType)
{
	int slotNumber;
	int numChips;
	float winnings = 0;

	switch (dropType)
	{
		case Single:
		{
			numChips = 1;
			winnings = DropChips(numChips, dropType);
			DisplayWinnings(numChips, winnings);
			break;
		}
		case Multiple:
		{
			numChips = GetNumChips();

			if (numChips < 0)
				break;

			winnings = DropChips(numChips, dropType);
			DisplayWinnings(numChips, winnings);
			break;
		}
		case Sequential:
		{
			numChips = GetNumChips();

			if (numChips < 0)
				break;

			DropChips(numChips, dropType);
			break;
		}
	}
}

int main()
{
	srand(seedVal);
	
	cout << "Welcome to the Plinko simulator! Enter 3 to see options." << endl;
	cout << endl;

	bool isValid = false;
	bool isPlaying = true;

	do
	{
		int choice;

		//prompts until input is valid or after displaying options
		do {
			cout << "Enter your selection now : ";
			choice = GetUserInput();
			cout << endl;

			if (choice == 4)
			{
				DisplayMenu();
				choice = 0; //causes selection prompt to appear again
			}

			isValid = ValidInput(choice, OptionNumber);

		} while (isValid == false);

		//action begins based on user selection
		switch (choice)
		{
			case 1:
			{
				cout << "*** Drop a single chip ***" << endl;
				cout << endl;
				PlayGame(Single);
				break;
			}
			case 2:
			{
				cout << "*** Drop multiple chips ***";
				cout << endl;
				PlayGame(Multiple);
				break;
			}
			case 3:
			{
				cout << "*** Sequentially drop multiple chips ***" << endl;
				cout << endl;
				PlayGame(Sequential);
				break;
			}
			case 5:
			{
				cout << "Goodbye!";
				isPlaying = false;
				return 0;
			}
		}

	} while (isPlaying);

	return 0;
}