#ifndef INC_MAP
#define INC_MAP
#include <cstdint>
#include "vsmda.hpp"

class CMap {
private:
	VSMDA<uint8_t>* m_staticMap;
	int m_maxX;
	int m_maxY;

public:
	void Init(int x, int y, int mines);
	void Quit();
	void printMap();
};

#endif
