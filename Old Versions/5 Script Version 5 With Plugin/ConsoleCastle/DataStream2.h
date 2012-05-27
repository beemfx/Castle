#define MOVE_START 0
#define MOVE_END 1
#define MOVE_CUR 2

class CDataStream
{
private:
	bool m_bOpen;
	int m_nSize;
	int m_nDataPointer;
	
	byte* m_pData;
public:
	CDataStream();
	CDataStream(char* szFile);
	~CDataStream();
	
	bool Open(char* szFile);
	void Close();
	int Read(byte* pBuffer, int count);
	int GetSize();
	int Seek(signed long nDistance, int nMethod);
	int Tell();
};