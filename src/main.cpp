#include <iostream>
#include <string>
#include <exception>
#include <getopt.h>
#include <readline/readline.h>
#include "vector.hpp"
#include "map.hpp"


using namespace std;

void fPrintHelp()
{
	cout << "Positioned commands:\n"
	        "\tTxy : Tries at field xy\n"
	        "\tXxy : Try non-flagged fields around xy\n"
	        "\tFxy : Flags field xy\n"
	        "\t?xy : Marks field xy\n"
	        "\tX-Coordinates are Letters, Y-Coordinates are Numbers.\n"
		"\tThe order does not matter\n"
	        "General commands:\n"
	        "\tR : Restart the Game from the same settings\n"
	        "\tV : Retype the Playfield\n"
	        "\tQ : Quit\n"
	        "\nAll commands and parameters are case insensitive\n";
}

CVector fParsePosition(string input)
{
	if (input.size() < 1)
		throw invalid_argument("#0 at fParsePosition");
	int x = -1, y = -1;
	for (auto it = ++input.cbegin(); it != input.cend(); it++)
	{
		char c = *it;
		if (isalpha(c) && x < 0) // X Coordinate
			x = (int)toupper(c) - 65;
		else if (isdigit(c) && y < 0) // Y Coordinate
			y = (int)c - 48;
	}
	if (x < 0 || y < 0)
		throw runtime_error("Missing or incomplete Position");
	return CVector(x, y);
}

bool fContinueQuestion()
{
	cout << "Do you want to continue with the same Settings? [y/N]";
	char inchar = ' ';
	cin >> inchar;
	return (inchar == 'y' || inchar == 'Y');
}

void fPrintOptions()
{
	cerr << "Usage: termsweeper [OPTIONS]\n"
		"A simple clone of minesweeper\n\n"
		"OPTIONS:\n"
		"\t--width, -x  : Set playfield width (max 26) (default: 8)\n"
		"\t--height, -h : Set plafield height (max 10) (default: 8)\n"
		"\t--mines, -m  : Set number of mines (max width*height) (default: 10)\n"
		"\t--help, -h   : This text\n";
}

void fPrintOptionHint()
{
	cerr << "Try \"termsweeper --help\" for more information\n";
}

int main(int argc, char* argv[])
{
	int iXSize = 8; // Default values for the playfield
	int iYSize = 8;
	int iMines = 10;
	while (true)
	{
		static struct option long_options[] =
		{
			{"width", required_argument, 0, 'x'},
			{"height", required_argument, 0, 'y'},
			{"mines", required_argument, 0, 'm'},
			{"help", no_argument, 0, 'h'},
			{0, 0, 0, 0}
		};
		int c = getopt_long(argc, argv, "x:y:m:h", long_options, nullptr);
		if (c == -1) break;
		switch (c)
		{
		case 'x':
			iXSize = atoi(optarg);
			break;
		case 'y':
			iYSize = atoi(optarg);
			break;
		case 'm':
			iMines = atoi(optarg);
			break;
		case 'h':
			fPrintOptions();
			return 0;
		case '?':
			fPrintOptionHint();
			return 1; 
		default:
			break;
		}
	}

	// Sanity checks for the Parameters
	if (iXSize <= 1 || iXSize > 26)
	{
		cerr << argv[0] << ": invalid playfield width: " << iXSize << endl;
		fPrintOptionHint();
		return 1;
	}
	if (iYSize <= 1 || iYSize > 10)
	{
		cerr << argv[0] << ": invalid playfield height: " << iYSize << endl;
		fPrintOptionHint();
		return 1;
	}
	if (iMines < 1 || iMines >= iXSize*iYSize)
	{
		cerr << argv[0] << ": invalid number of mines: " << iMines << endl;
		fPrintOptionHint();
		return 1;
	}

	cout << "\x1b]2;Termsweeper\x1b\\\x1b[?1049h";

	CMap Map;
	Map.Init(CVector(iXSize, iYSize), iMines); // Initializing Map
	bool bContinueLoop = true;
	Map.printMap();
	while (bContinueLoop)
	{
		Map.printMessages();
		char* line = readline("Please enter command (help with H): ");
		if (!line)
			break;
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
			} catch (runtime_error& rte) {
				cout << rte.what() << endl;
				break;
			} catch (exception& e) {
				cout << "Unknown Exception: " << e.what() << endl;
				break;
			}
			break;
		case 'F':
		case 'f':
			try
			{
				Map.Flag(fParsePosition(sCommand));
				Map.printMap();
			} catch (runtime_error& rte) {
				cout << rte.what() << endl;
				break;
			} catch (exception& e) {
				cout << "Unknown Exception: " << e.what() << endl;
				break;
			}
			break;
		case '?':
			try
			{
				Map.Mark(fParsePosition(sCommand));
				Map.printMap();
			} catch (runtime_error& rte) {
				cout << rte.what() << endl;
				break;
			} catch (exception& e) {
				cout << "Unknown Exception: " << e.what() << endl;
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
			} catch (runtime_error& rte) {
				cout << rte.what() << endl;
				break;
			} catch (exception& e) {
				cout << "Unknown Exception: " << e.what() << endl;
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
	cout << "\x1b[?1049l";
	return 0;
}
