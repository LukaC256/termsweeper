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
	m_dynamicMap = new VSMDA<uint8_t>(m_size);

	// Fill the Map with zeroes / ones
	for (size_t y = 0; y < m_size.y; y++) {
		for (size_t x = 0; x < m_size.x; x++) {
			m_staticMap->Set(CVector(x, y), 0);
			m_dynamicMap->Set(CVector(x, y), 1);
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

	// Generate number fields
	for (size_t y = 0; y < m_size.y; y++)
	{
		for (size_t x = 0; x < m_size.x; x++)
		{
			CVector pos(x, y);
			if (m_staticMap->Get(pos) == 9) continue;
			int minecnt = 0;
			for (size_t d = 0; d < 8; d++)
			{
				if (m_staticMap->Get(pos + vDirs[d]) == 9) minecnt++;
			}
			m_staticMap->Set(pos, minecnt);
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
			uint8_t iDynamicField = m_dynamicMap->Get(CVector(x, y));
			switch (iDynamicField)
			{
			case 0:{
				uint8_t iStaticField = m_staticMap->Get(CVector(x, y));
				switch (iStaticField)
				{
				case 9:
					cout << charMine;
					break;
				case 0:
					cout << charFree;
					break;
				default:
					cout << (char) (48+iStaticField);
					break;
				} break;}
			case 1:
				cout << charHidden;
				break;
			case 2:
				cout << charFlag;
				break;
			case 3:
				cout << '?';
				break;
			}
		}
		cout << endl;
	}
}
