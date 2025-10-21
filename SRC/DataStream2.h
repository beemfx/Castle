
class CDataStream
{
private:
	bool m_bOpen;
	int  m_nSize;
	int  m_nDataPointer;
	
	unsigned char* m_pData;
public:
	enum MOVE_T
	{
		MOVE_START=0,
		MOVE_END=1,
		MOVE_CUR=2,
	};
public:
	CDataStream();
	CDataStream(char* szFile);
	~CDataStream();
	
	bool Open(const char* szFile);
	void Close();
	int Read(unsigned char* pBuffer, int count);
	int GetSize();
	int Seek(signed long nDistance, MOVE_T nMethod);
	int Tell();
};