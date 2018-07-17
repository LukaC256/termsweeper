#include <iostream>
#include "vector.hpp"
#include "map.hpp"

using namespace std;

int main()
{
	CMap Map;
	Map.Init(CVector(10, 10), 10);
	cout << "Map Initialized!\n";
	Map.printMap();
	Map.Quit();
	return 0;
}
