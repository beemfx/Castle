// (c) 2025 Beem Media. All rights reserved.

#include "DataStream2.h"
#include <filesystem>
#include <fstream>

CDataStream::CDataStream(const char* Filename)
{
	Open(Filename);
}

CDataStream::~CDataStream()
{
	Close();
}

bool CDataStream::Open(const char* Filename)
{
	//If a file is already open, close it.
	Close();

	const std::size_t FileSize = std::filesystem::file_size(Filename);
	m_Data.resize(FileSize);
	if (m_Data.size() == FileSize)
	{
		std::ifstream File(Filename, std::ios::binary);
		File.read(reinterpret_cast<char*>(m_Data.data()), m_Data.size());
		m_bOpen = true;
		return true;
	}

	return false;
}

void CDataStream::Close()
{
	m_Data.resize(0);
	m_ReadPtr = 0;
	m_bOpen = false;
}

std::size_t CDataStream::Read(ds_byte* pBuffer, std::size_t count)
{
	if (!m_bOpen)
	{
		return 0;
	}

	std::size_t SizeToRead = count;

	if ((m_ReadPtr + SizeToRead) > m_Data.size())
	{
		count = m_Data.size() - m_ReadPtr;
	}

	std::memcpy(pBuffer, m_Data.data() + m_ReadPtr, SizeToRead);
	m_ReadPtr += SizeToRead;
	return SizeToRead;
}

std::size_t CDataStream::GetSize() const
{
	if (!m_bOpen)
	{
		return 0;
	}

	return m_Data.size();
}

std::size_t CDataStream::Seek(signed long nDistance, MOVE_T nMethod)
{
	if (!m_bOpen)
	{
		return 0;
	}

	switch (nMethod)
	{
	case MOVE_START:
		m_ReadPtr = nDistance;
		break;
	case MOVE_CUR:
		m_ReadPtr += nDistance;
		break;
	case MOVE_END:
		m_ReadPtr = m_Data.size() + nDistance;
		break;
	}

	//Make sure the new data pointer is within the file data.
	m_ReadPtr = EG_Clamp<std::size_t>(m_ReadPtr, 0, m_Data.size());

	return m_ReadPtr;
}

std::size_t CDataStream::Tell() const
{
	if (!m_bOpen)
	{
		return 0;
	}

	return m_ReadPtr;
}