#include "map.hpp"
#include "characters.hpp"
#include "vector.hpp"
#include <cstdlib>
#include <iostream>

using namespace std;

const CVector vDirs[] = {
	CVector(-1, -1),
	CVector(0, -1),
	CVector(1, -1),
	CVector(-1, 0),
	CVector(1, 0),
	CVector(-1, 1),
	CVector(0, 1),
	CVector(1, 1)
};

void CMap::Init(const int maxX, const int maxY, int mines)
{
	m_maxX = maxX;
	m_maxY = maxY;

	m_staticMap = new VSMDA<uint8_t>(maxX, maxY); // Allocate memory for the map

	// Fill the Map with zeroes
	for (size_t y = 0; y < maxX; y++) {
		for (size_t x = 0; x < maxY; x++) {
			m_staticMap->Set(x, y, 0);
		}
	}

	// Place mines randomly, but if there's already one
	// at that place, try again
	srand(time(NULL)); // Initialize PSRNG
	for (size_t i = 0; i < mines; i++) {
		while (true) {
			int x = rand() % maxX; int y = rand() % maxY;
			//cout << "pos: " << x << " : " << y << m_staticMap[x][y] << endl;
			if (m_staticMap->Get(x, y) != 9) {
				m_staticMap->Set(x, y, 9);
				break;
			}
		}
	}
}

void CMap::Quit()
{
	if (m_staticMap != NULL) {
		delete m_staticMap;
	}
}

void CMap::printMap()
{
	cout << "  ";
	for (size_t i = 0; i < m_maxX; i++) {
		cout << (char) (65 + i);
	}
	cout << endl;
	for (size_t y = 0; y < m_maxY; y++) {
		cout << y << ' ';
		for (size_t x = 0; x < m_maxX; x++) {
			uint8_t iStaticField = m_staticMap->Get(x, y);
			if (iStaticField == 9) {
				cout << charMine;
			} else {
				cout << charFree;
			}
		}
		cout << endl;
	}
}
