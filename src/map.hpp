#pragma once
#include <cstdint>
#include <queue>
#include <string>
#include "vsmda.hpp"

class CMap
{
private:
	VSMDA<uint8_t>* m_staticMap;
	VSMDA<uint8_t>* m_dynamicMap;
	std::queue<std::string> m_messageQueue;
	CVector m_size;
	int m_mines;

public:
	CMap(CVector size, int mines);
	~CMap();
	void printMap(bool showEntireField = false);
	void printMessages();
	bool Try(CVector pos);
	bool TryAround(CVector pos);
	void Flag(CVector pos);
	void Mark(CVector pos);
	bool GameWon();
};
