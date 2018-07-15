#include <cstdlib>

template <class T>
class VSMDA
{
	private:
		T* m_array;
		size_t m_sizeX;
		size_t m_sizeY;
	public:
		VSMDA(size_t x, size_t y):
		m_sizeX (x), m_sizeY (y)
		{
			m_array = new T [x*y];
		};
		~VSMDA()
		{
			delete[] m_array;
		};
		T Get(size_t x, size_t y)
		{
			return m_array[y*m_sizeX+x];
		};
		void Set(size_t x, size_t y, T value)
		{
			m_array[y*m_sizeX+x] = value;
		};
};
