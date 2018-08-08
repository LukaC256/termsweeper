#ifndef INC_MAP
#define INC_MAP
#include <cstdint>
#include "vsmda.hpp"

class CMap {
private:
	VSMDA<uint8_t>* m_staticMap;
	VSMDA<uint8_t>* m_dynamicMap;
	CVector m_size;

public:
	void Init(CVector size, int mines);
	void Quit();
	void printMap();
	bool Try(CVector pos);
	void Flag(CVector pos);
};

#endif
