// (c) 2025 Beem Media. All rights reserved.

#pragma once

#include "CastleTypes.h"

class CDataStream
{
public:
	using ds_byte = unsigned char;
	static_assert(sizeof(ds_byte) == 1, "A byte must have a size of 1.");

private:
	std::vector<ds_byte> m_Data;
	std::size_t m_ReadPtr = 0;

public:
	enum MOVE_T
	{
		MOVE_START = 0,
		MOVE_END = 1,
		MOVE_CUR = 2,
	};

public:
	CDataStream() = default;
	CDataStream(const eg_path& Filename);
	~CDataStream();

	bool Open(const eg_path& Filename);
	void Close();
	std::size_t Read(ds_byte* pBuffer, std::size_t count);
	std::size_t GetSize() const;
	std::size_t Seek(signed long nDistance, MOVE_T nMethod);
	std::size_t Tell() const;
};