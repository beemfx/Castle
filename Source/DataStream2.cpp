#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include "DataStream2.h"

CDataStream::CDataStream():
m_nSize(0),m_nDataPointer(0),m_bOpen(false),m_pData(0)
{

}

CDataStream::CDataStream(char *szFile):
m_nSize(0),m_nDataPointer(0),m_bOpen(false),m_pData(0)
{
	Open(szFile);
}

CDataStream::~CDataStream()
{
	Close();
}

bool CDataStream::Open(char* szFile)
{
	//If a file is already open, close it.
	if(m_bOpen)
		Close();
		
	FILE* pTemp=NULL;
	errno_t Res = fopen_s( &pTemp, szFile, "rb");
	if( 0 != Res )
	{
		return false;
	}
		
	//Get the file size.
	fseek(pTemp, 0, SEEK_END);
	m_nSize=ftell(pTemp);
	m_pData=(unsigned char*)malloc(m_nSize);
	
	if(!m_pData)
	{
		fclose(pTemp);
		return false;
	}
	
	fseek(pTemp, 0, SEEK_SET);
	fread(m_pData, 1, m_nSize, pTemp);
	fclose(pTemp);
	m_nDataPointer=0;
	m_bOpen=true;
	return true;
}

void CDataStream::Close()
{
	if(!m_bOpen)
		return;
		
	if(m_pData){free(m_pData);m_pData=0;}
	m_nSize=0;
	m_nDataPointer=0;
	m_bOpen=false;
}

int CDataStream::Read(unsigned char* pBuffer, int count)
{
	if(!m_bOpen)
		return 0;
	
	if(m_nSize < (m_nDataPointer + count) )
		count = m_nSize-m_nDataPointer;
		
	memcpy(pBuffer, m_pData+m_nDataPointer, count);
	m_nDataPointer+=count;
	return count;
}

int CDataStream::GetSize()
{
	if(!m_bOpen)
		return 0;
	return m_nSize;
}

int CDataStream::Seek(signed long nDistance, MOVE_T nMethod)
{
	if(!m_bOpen)
		return 0;
		
	switch(nMethod)
	{
	case MOVE_START:
		m_nDataPointer=nDistance;
		break;
	case MOVE_CUR:
		m_nDataPointer+=nDistance;
		break;
	case MOVE_END:
		m_nDataPointer=m_nSize+nDistance;
		break;
	}
	
	//Make sure the new data pointer is within the file data.
	m_nDataPointer=m_nDataPointer>m_nSize?m_nSize:m_nDataPointer<0?0:m_nDataPointer;
		
	return m_nDataPointer;
}

int CDataStream::Tell()
{
	if(!m_bOpen)
		return 0;
		
	return m_nDataPointer;
}