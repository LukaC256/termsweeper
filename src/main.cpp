#include <iostream>
#include <string>
#include <readline/readline.h>
#include "vector.hpp"
#include "map.hpp"


using namespace std;

void fPrintHelp()
{
	cout << "Positioned commands:\n";
	cout << "\tTxy : Tries at field xy\n";
	cout << "\tXxy : Try non-flagged fields around xy\n";
	cout << "\tFxy : Flags field xy\n";
	cout << "\t?xy : Marks field xy\n";
	cout << "General commands:\n";
	cout << "\tR : Restart the Game from the same settings\n";
	cout << "\tV : Retype the Playfield\n";
	cout << "\tQ : Quit\n";
	cout << "\nAll commands and parameters are case insensitive\n";
}

CVector fParsePosition(string input)
{
	int x = toupper((int)input.at(1)) - 65;
	int y = ((int)input.at(2)) - 48;
	if (x < 0 || y < 0)
		throw 1;
	return CVector(x, y);
}

bool fContinueQuestion()
{
	cout << "Do you want to continue with the same Settings? [y/N]";
	char inchar = ' ';
	cin >> inchar;
	return (inchar == 'y' || inchar == 'Y');
}

int main(int argc, char* args[])
{
	int iXSize = 8; // Default values for the playfield
	int iYSize = 8;
	int iMines = 10;
	for (size_t a = 0; a < argc; a++) // CLI-Argument parsing
	{
		char* arg = args[a]; // Get the first char
		if (*arg == '-')
		{
			switch(arg[1]) //Decide based on the second char
			{
			case 'x': // X Size
				a++; // Get the next argument
				if (a >= argc) break; // Sanity check
				iXSize = atoi(args[a]); // Turn it into an Integer
				break;
			case 'y': // Y Size
				a++;
				if (a >= argc) break;
				iYSize = atoi(args[a]);
				break;
			case 'm': // Number of mines
				a++;
				if (a >= argc) break;
				iMines = atoi(args[a]);
				break;
			default: // Error and Exit on unknown argument
				cout << "Unrecognised argument " << arg[1] << "!\n";
				return 1;
				break;
			}
		}
	}

	// Sanity checks for the Parameters
	if (iXSize <= 1 || iXSize > 26)
	{
		cout << "Invalid value for -x: " << iXSize << endl;
		return 1;
	}
	if (iYSize <= 1 || iYSize > 10)
	{
		cout << "Invalid value for -y: " << iYSize << endl;
		return 1;
	}
	if (iMines < 1 || iMines >= iXSize*iYSize)
	{
		cout << "Invalid value for -m: " << iMines << endl;
		return 1;
	}

	cout << "\x1b]2;Termsweeper\x1b\\";

	CMap Map;
	Map.Init(CVector(iXSize, iYSize), iMines); // Initializing Map
	bool bContinueLoop = true;
	Map.printMap();
	while (bContinueLoop)
	{
		Map.printMessages();
		char* line = readline("Please enter command (help with H): ");
		string sCommand(line);
		delete[] line;

		if (sCommand.size() < 1) continue;
		switch (sCommand.at(0)) // Command parsing
		{
		case 'T':
		case 't':
			try
			{
				if (!Map.Try(fParsePosition(sCommand)))
				{
					Map.printMap(true);
					cout << "Game Over!\n";
					if (fContinueQuestion())
					{
						Map.Quit();
						Map.Init(CVector(iXSize, iYSize), iMines);
						Map.printMap();
					} else {
						bContinueLoop = false;
					}
				} else {
					Map.printMap();
				}
			} catch (...) {
				cout << "Invalid or missing Position!\n";
				break;
			}
			break;
		case 'F':
		case 'f':
			try
			{
				Map.Flag(fParsePosition(sCommand));
				Map.printMap();
			} catch (...) {
				cout << "Invalid or missing Position!\n";
				break;
			}
			break;
		case '?':
			try
			{
				Map.Mark(fParsePosition(sCommand));
				Map.printMap();
			} catch (...) {
				cout << "Invalid or missing Position!\n";
				break;
			}
			break;
		case 'X':
		case 'x':
			try
			{
				if (!Map.TryAround(fParsePosition(sCommand)))
				{
					Map.printMap(true);
					cout << "Game Over!\n";
					if (fContinueQuestion())
					{
						Map.Quit();
						Map.Init(CVector(iXSize, iYSize), iMines);
						Map.printMap();
					} else {
						bContinueLoop = false;
					}
				} else {
					Map.printMap();
				}
			} catch (...) {
				cout << "Invalid or missing Position!\n";
				break;
			}
			break;
		case 'Q':
		case 'q':
			bContinueLoop = false;
			break;
		case 'H':
		case 'h':
			fPrintHelp();
			break;
		case 'V':
		case 'v':
			Map.printMap();
			break;
		case 'r':
		case 'R':
			Map.Quit();
			Map.Init(CVector(iXSize, iYSize), iMines);
			Map.printMap();
			break;
		}
		if (Map.GameWon())
		{
			cout << "Game Completed!\n";
			if (fContinueQuestion())
			{
				Map.Quit();
				Map.Init(CVector(iXSize, iYSize), iMines);
				Map.printMap();
			} else {
				break;
			}
		}
	}
	Map.Quit();
	return 0;
}
