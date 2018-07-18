#include <iostream>
#include "vector.hpp"
#include "map.hpp"

using namespace std;

int main(int argc, char* args[])
{
	int iXSize = 8;
	int iYSize = 8;
	int iMines = 10;
	for (size_t a = 0; a < argc; a++)
	{
		char* arg = args[a];
		if (*arg == '-')
		{
			switch(arg[1])
			{
			case 'x':
				a++;
				iXSize = atoi(args[a]);
				break;
			case 'y':
				a++;
				iYSize = atoi(args[a]);
				break;
			case 'm':
				a++;
				iMines = atoi(args[a]);
				break;
			default:
				cout << "Unrecognised argument " << arg[1] << "!\n";
				break;
			}
		}
	}
	CMap Map;
	Map.Init(CVector(iXSize, iYSize), iMines);
	cout << "Map Initialized!\n";
	Map.printMap();
	Map.Quit();
	return 0;
}
