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

void CMap::Init(const CVector size, const int mines)
{
	m_size = size;
	m_staticMap = new VSMDA<uint8_t>(m_size); // Allocate memory for the map

	// Fill the Map with zeroes
	for (size_t y = 0; y < m_size.y; y++) {
		for (size_t x = 0; x < m_size.x; x++) {
			m_staticMap->Set(CVector(x, y), 0);
		}
	}

	// Place mines randomly, but if there's already one
	// at that place, try again
	srand(time(NULL)); // Initialize PSRNG
	for (size_t i = 0; i < mines; i++) {
		while (true) {
			int x = rand() % m_size.x; int y = rand() % m_size.y;
			//cout << "pos: " << x << " : " << y << m_staticMap[x][y] << endl;
			if (m_staticMap->Get(CVector(x, y)) != 9) {
				m_staticMap->Set(CVector(x, y), 9);
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
	for (size_t i = 0; i < m_size.x; i++) {
		cout << (char) (65 + i);
	}
	cout << endl;
	for (size_t y = 0; y < m_size.y; y++) {
		cout << y << ' ';
		for (size_t x = 0; x < m_size.x; x++) {
			uint8_t iStaticField = m_staticMap->Get(CVector(x, y));
			if (iStaticField == 9) {
				cout << charMine;
			} else {
				cout << charFree;
			}
		}
		cout << endl;
	}
}
