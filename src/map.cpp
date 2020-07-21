#include "map.hpp"
#include "characters.hpp"
#include "vector.hpp"
#include <cstdlib>
#include <ctime>
#include <random>
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

CMap::CMap(const CVector size, const int mines) :
	m_size(size), m_mines(mines), m_staticMap(size),
	m_dynamicMap(size)
{
	// Fill the Map with zeroes / ones
	for (size_t y = 0; y < m_size.y; y++)
	{
		for (size_t x = 0; x < m_size.x; x++)
		{
			m_staticMap.Set(CVector(x, y), 0);
			m_dynamicMap.Set(CVector(x, y), 1);
		}
	}

	// Place mines randomly, but if there's already one
	// at that place, try again
	random_device seed_ng;
	default_random_engine ran_ng(seed_ng()); // Seed generator
	uniform_int_distribution<int> x_pos(0, m_size.x - 1);
	uniform_int_distribution<int> y_pos(0, m_size.y - 1);
	for (size_t i = 0; i < m_mines; i++)
	{
		while (true)
		{
			int x = x_pos(ran_ng); int y = y_pos(ran_ng);
			//cout << "pos: " << x << " : " << y << m_staticMap[x][y] << endl;
			if (m_staticMap.Get(CVector(x, y)) != 9)
			{
				m_staticMap.Set(CVector(x, y), 9);
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
			if (m_staticMap.Get(pos) == 9)
				continue;
			int minecnt = 0;
			for (size_t d = 0; d < 8; d++)
			{
				if (m_staticMap.Get(pos + vDirs[d]) == 9)
					minecnt++;
			}
			m_staticMap.Set(pos, minecnt);
		}
	}
}

void CMap::printMap(bool showEntireField)
{
	cerr << "\x1b[H\x1b[J  ";
	for (size_t i = 0; i < m_size.x; i++)
	{
		cerr << (char) (65 + i);
	}
	cerr << endl;

	int num_flags = 0;
	for (size_t y = 0; y < m_size.y; y++)
	{
		cerr << y << ' ';
		for (size_t x = 0; x < m_size.x; x++)
		{
			uint8_t iDynamicField = m_dynamicMap.Get(CVector(x, y));
			uint8_t iStaticField = m_staticMap.Get(CVector(x, y));
			if (showEntireField && iDynamicField != 2)
				iDynamicField = 0;
			switch (iDynamicField)
			{
			case 0:
			{
				switch (iStaticField)
				{
				case 9:
					if (showEntireField)
						cerr << "\x1b[91m" << charMine << "\x1b[39m";
					else
						cerr << "‽"; // This SHOULD be unreachable...
					break;
				case 0:
					if ((x + (y%2)) % 2)
					{
						cerr << "\x1b[90m" << charFree << "\x1b[39m";
					} else
						cerr << charFree;
					break;
				default:
					cerr << "\x1b[" << numberColors[iStaticField-1] << "m" << (char) (48+iStaticField) << "\x1b[39m";
					break;
				}
				break;
			}
			case 1:
				if ((x + (y%2)) % 2)
				{
					cerr << "\x1b[90m" << charHidden << "\x1b[39m";
				} else
					cerr << charHidden;
				break;
			case 2:
				if (showEntireField && iStaticField == 9)
					cerr << "\x1b[92m" << charFlag << "\x1b[39m";
				else if (showEntireField)
					cerr << "\x1b[91m" << charFlag << "\x1b[39m";
				else
					cerr << charFlag;
				num_flags++;
				break;
			case 3:
				cerr << '?';
				break;
			}
		}
		cerr << ' ' << y << endl;
	}
	cerr << "  ";
	for (size_t i = 0; i < m_size.x; i++)
	{
		cerr << (char) (65 + i);
	}
	cerr << endl;
	cerr << "Flags: " << num_flags << "/" << m_mines << endl;
}

void CMap::printMessages()
{
	while (!m_messageQueue.empty())
	{
		cerr << m_messageQueue.front() << endl;
		m_messageQueue.pop();
	}
}

bool CMap::Try(CVector pos)
{
	if (pos.x >= m_size.x || pos.x < 0 ||
		pos.y >= m_size.y || pos.y < 0)
		return true;
	if (m_dynamicMap.Get(pos) == 0)
		return true;
	m_dynamicMap.Set(pos, 0);
	switch (m_staticMap.Get(pos))
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
	if (m_staticMap.Get(pos) == 0)
	{
		m_messageQueue.push(string("This operation is not permitted!"));
		return true;
	}
	int flagcnt = 0;
	for (size_t d = 0; d < 8; d++)
	{
		if (m_dynamicMap.Get(pos + vDirs[d]) == 2)
			flagcnt++;
	}
	if (flagcnt < m_staticMap.Get(pos))
	{
		m_messageQueue.push(string("This operation is not permitted!"));
		return true;
	}

	for (size_t d = 0; d < 8; d++)
	{
		if (m_dynamicMap.Get(pos + vDirs[d]) != 2)
		{
			if (!Try(pos+vDirs[d]))
				return false;
		}
	}
	return true;
}

void CMap::Flag(CVector pos)
{
	if (pos.x >= m_size.x || pos.x < 0 ||
		pos.y >= m_size.y || pos.y < 0)
	{
		m_messageQueue.push(string("This field is outside the range!"));
		return;
	}
	int field = m_dynamicMap.Get(pos);
	if (field == 0)
	{
		m_messageQueue.push(string("This field cannot be flagged!"));
		return;
	}
	if (field == 2)
	{
		m_dynamicMap.Set(pos, 1);
		m_messageQueue.push(string("Flag removed!"));
	} else {
		m_dynamicMap.Set(pos, 2);
		m_messageQueue.push(string("Field flagged!"));
	}
}

void CMap::Mark(CVector pos)
{
	if (pos.x >= m_size.x || pos.x < 0 ||
		pos.y >= m_size.y || pos.y < 0)
	{
		m_messageQueue.push(string("This field is outside the range!"));
		return;
	}
	int field = m_dynamicMap.Get(pos);
	if (field == 0)
	{
		m_messageQueue.push(string("This field cannot be marked!"));
		return;
	}
	if (field == 3)
	{
		m_dynamicMap.Set(pos, 1);
		m_messageQueue.push(string("Mark removed!"));
	} else {
		m_dynamicMap.Set(pos, 3);
		m_messageQueue.push(string("Field marked!"));
	}
}

bool CMap::GameWon()
{
	for (size_t y = 0; y < m_size.y; y++)
	{
		for (size_t x = 0; x < m_size.x; x++)
		{
			if (m_staticMap.Get(CVector(x,y)) != 9 && m_dynamicMap.Get(CVector(x,y)) != 0)
				return false;
		}
	}
	// Game is won, set all mines under flags
	for (size_t y = 0; y < m_size.y; y++)
	{
		for (size_t x = 0; x < m_size.x; x++)
		{
			if (m_staticMap.Get(CVector(x,y)) == 9)
				m_dynamicMap.Set(CVector(x,y), 2);
		}
	}
	return true;
}
