#pragma once
#include <cstdlib>
#include "vector.hpp"

template <class T>
// Variable Size Multi-Dimensional Array
class VSMDA
{
private:
	T* m_array;
	CVector m_size;
public:
	VSMDA(CVector size):

	m_size(size)
	{
		m_array = new T [m_size.x*m_size.y];
	};

	~VSMDA()
	{
		delete[] m_array;
	};

	T Get(CVector pos)
	{
		if (pos.x < 0 || pos.x >= m_size.x ||
			pos.y < 0 || pos.y >= m_size.y)
			return 0;
		else
			return m_array[pos.y*m_size.x+pos.x];
	};

	void Set(CVector pos, T value)
	{
		m_array[pos.y*m_size.x+pos.x] = value;
	};
};
