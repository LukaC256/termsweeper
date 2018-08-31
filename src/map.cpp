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

	m_messageQueue = new queue<string>();

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
	cout << "\x1b[H\x1b[J  ";
	for (size_t i = 0; i < m_size.x; i++)
	{
		cout << (char) (65 + i);
	}
	cout << endl;
	for (size_t y = 0; y < m_size.y; y++)
	{
		cout << y << ' ';
		for (size_t x = 0; x < m_size.x; x++)
		{
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
					cout << "\x1b[" << numberColors[iStaticField-1] << "m" << (char) (48+iStaticField) << "\x1b[39m";
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
		cout << ' ' << y << endl;
	}
	cout << "  ";
	for (size_t i = 0; i < m_size.x; i++)
	{
		cout << (char) (65 + i);
	}
	cout << endl;
}

void CMap::printMessages()
{
	while (!m_messageQueue->empty())
	{
		cout << m_messageQueue->front() << endl;
		m_messageQueue->pop();
	}
}

bool CMap::Try(CVector pos)
{
	if (pos.x >= m_size.x || pos.x < 0 ||
		pos.y >= m_size.y || pos.y < 0) return true;
	if (m_dynamicMap->Get(pos) == 0) return true;
	m_dynamicMap->Set(pos, 0);
	switch (m_staticMap->Get(pos))
	{
	case 0:
		for (size_t d = 0; d < 8; d++)
		{
			Try(pos + vDirs[d]);
		}
		break;
	case 9:
		return false;
	default:
		break;
	}
	return true;
}

bool CMap::TryAround(CVector pos)
{
	if (m_staticMap->Get(pos) == 0)
	{
		m_messageQueue->push(string("This operation is not permitted!"));
		return true;
	}
	int flagcnt = 0;
	for (size_t d = 0; d < 8; d++)
	{
		if (m_dynamicMap->Get(pos + vDirs[d]) == 2) flagcnt++;
	}
	if (flagcnt < m_staticMap->Get(pos))
	{
		m_messageQueue->push(string("This operation is not permitted!"));
		return true;
	}

	for (size_t d = 0; d < 8; d++)
	{
		if (m_dynamicMap->Get(pos + vDirs[d]) != 2)
		{
			if (!Try(pos+vDirs[d])) return false;
		}
	}
	return true;
}

void CMap::Flag(CVector pos)
{
	if (pos.x >= m_size.x || pos.x < 0 ||
		pos.y >= m_size.y || pos.y < 0)
	{
		m_messageQueue->push(string("This field is outside the range!"));
		return;
	}
	int field = m_dynamicMap->Get(pos);
	if (field == 0)
	{
		m_messageQueue->push(string("This field cannot be flagged!"));
		return;
	}
	if (field == 2)
	{
		m_dynamicMap->Set(pos, 1);
		m_messageQueue->push(string("Flag removed!"));
	} else
	{
		m_dynamicMap->Set(pos, 2);
		m_messageQueue->push(string("Field flagged!"));
	}
}

void CMap::Mark(CVector pos)
{
	if (pos.x >= m_size.x || pos.x < 0 ||
		pos.y >= m_size.y || pos.y < 0)
	{
		m_messageQueue->push(string("This field is outside the range!"));
		return;
	}
	int field = m_dynamicMap->Get(pos);
	if (field == 0)
	{
		m_messageQueue->push(string("This field cannot be marked!"));
		return;
	}
	if (field == 3)
	{
		m_dynamicMap->Set(pos, 1);
		m_messageQueue->push(string("Mark removed!"));
	} else
	{
		m_dynamicMap->Set(pos, 3);
		m_messageQueue->push(string("Field marked!"));
	}
}

bool CMap::GameWon()
{
	for (size_t y = 0; y < m_size.y; y++)
		for (size_t x = 0; x < m_size.x; x++)
			if (m_staticMap->Get(CVector(x,y)) != 9 && m_dynamicMap->Get(CVector(x,y)) != 0)
				return false;
	return true;
}