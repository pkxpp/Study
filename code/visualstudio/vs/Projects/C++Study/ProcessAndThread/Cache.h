#include <string>
struct BufferReaderInfo
{
	size_t m_key;
	int m_dwTimeTouched;
	int m_dwTouchCount;
	void* m_piBufferReader;
	std::string m_strFileName;
};