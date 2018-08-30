#ifndef INC_MAP
#define INC_MAP
#include <cstdint>
#include <queue>
#include <string>
#include "vsmda.hpp"

class CMap {
private:
	VSMDA<uint8_t>* m_staticMap;
	VSMDA<uint8_t>* m_dynamicMap;
	std::queue<std::string>* m_messageQueue;
	CVector m_size;

public:
	void Init(CVector size, int mines);
	void Quit();
	void printMap();
	void printMessages();
	bool Try(CVector pos);
	bool TryAround(CVector pos);
	void Flag(CVector pos);
};

#endif
