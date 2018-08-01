#include <iostream>
#include "vector.hpp"
#include "map.hpp"

using namespace std;

void fPrintHelp()
{
	cout << "Positioned commands:\n";
	cout << "\tTxy : Tries at field xy\n";
	cout << "\tFxy : Flags field xy\n";
	cout << "\t?xy : Marks field xy\n";
	cout << "General commands:\n";
	cout << "\tC : Tries all non-flagged fields\n";
	cout << "\tS : Save the Game\n";
	cout << "\tR : Restart the Game from the same settings\n";
	cout << "\tQ : Quit\n";
}

int main(int argc, char* args[])
{
	int iXSize = 8; // Default values for the playfield
	int iYSize = 8;
	int iMines = 10;
	for (size_t a = 0; a < argc; a++) // CLI-Arguments
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

	CMap Map;
	Map.Init(CVector(iXSize, iYSize), iMines);
	while (true)
	{
		Map.printMap();
		cout << "\nPlease enter command (help with H): ";
		char* pcCommand = new char;
		cin >> pcCommand;
		switch (pcCommand[0])
		{
		case 'Q':
		case 'q':
			delete pcCommand;
			Map.Quit();
			return 0;
		case 'H':
			fPrintHelp();
			break;
		}
		delete pcCommand;
	}
	Map.Quit();
	return 0;
}
