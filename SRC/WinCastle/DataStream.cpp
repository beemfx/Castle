// (c) 2025 Beem Media. All rights reserved.
//
// Apparently the original DataStream.cpp was lost, so I had to
// recreate it.

#include "DataStream.h"
#include <vector>

using DS_byte = unsigned char;

struct DataStreamInternal
{
	std::vector<DS_byte> Data;
	std::size_t ReadPointer = 0;
};

HDATASTREAM CreateDataStream(
	void* lpData,
	unsigned long dwDesiredAccess,
	unsigned long dwCreationDisposition,
	unsigned long dwSize)
{
	DataStreamInternal* NewDS = new DataStreamInternal;
	if (NewDS)
	{
		NewDS->Data.resize(dwSize);
		std::memcpy(NewDS->Data.data(), lpData, NewDS->Data.size());
	}
	return reinterpret_cast<HDATASTREAM>(NewDS);
}

int CloseDataStream(
	HDATASTREAM hDataStream)
{
	DataStreamInternal* DS = reinterpret_cast<DataStreamInternal*>(hDataStream);

	if (DS)
	{
		delete DS;
	}

	return 0;
}

unsigned long GetDataStreamSize(
	HDATASTREAM hDataStream)
{
	DataStreamInternal* DS = reinterpret_cast<DataStreamInternal*>(hDataStream);

	return DS ? DS->Data.size() : 0;
}

int ReadDataStream(
	HDATASTREAM hDataStream,
	void* lpBuffer,
	unsigned long nNumberOfBytesToRead,
	unsigned long* lpNumberOfBytesRead)
{
	DataStreamInternal* DS = reinterpret_cast<DataStreamInternal*>(hDataStream);

	if (DS)
	{
		unsigned long SizeToRead = nNumberOfBytesToRead;
		if (DS->ReadPointer + SizeToRead > DS->Data.size())
		{
			SizeToRead = DS->Data.size() - DS->ReadPointer;
		}

		if (lpNumberOfBytesRead)
		{
			*lpNumberOfBytesRead = SizeToRead;
		}

		std::memcpy(lpBuffer, DS->Data.data() + DS->ReadPointer, SizeToRead);
		DS->ReadPointer += SizeToRead;

		return 1;
	}

	return 0;
}

int WriteDataStream(
	HDATASTREAM hDataStream,
	const void* lpBuffer,
	unsigned long nNumberOfBytesToWrite,
	unsigned long* lpNumberOfBytesWritten)
{
	DataStreamInternal* DS = reinterpret_cast<DataStreamInternal*>(hDataStream);
	// This is never used.
	return 0;
}

unsigned long SetDataStreamPointer(
	HDATASTREAM hDataStream,
	signed long lDistanceToMove,
	unsigned long dwMoveMethod)
{
	DataStreamInternal* DS = reinterpret_cast<DataStreamInternal*>(hDataStream);
	
	if (DS)
	{
		switch (dwMoveMethod)
		{
			case STREAM_BEGIN:
				DS->ReadPointer = lDistanceToMove;
				break;
			case STREAM_CURRENT:
				DS->ReadPointer += lDistanceToMove;
				break;
			case STREAM_END:
				DS->ReadPointer = DS->Data.size() + lDistanceToMove;
				break;
		}

		if (DS->ReadPointer < 0)
		{
			DS->ReadPointer = 0;
		}
		if (DS->ReadPointer > DS->Data.size())
		{
			DS->ReadPointer = DS->Data.size();
		}

		return 1;
	}

	return 0;
}
