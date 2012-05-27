/*
	DataStream.h - Declarations for DataStream functions.

	Copyright (c) 2003 Blaine Myers
*/

#ifndef __DATASTREAM_H__
#define __DATASTREAM_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DATASTREAM_READ  (0x80000000l)
#define DATASTREAM_WRITE (0x40000000l)

#define CREATESTREAM_NEW               1
#define CREATESTREAM_FROMDATA          2
#define CREATESTREAM_FROMDATAEXCLUSIVE 3

#define STREAM_BEGIN           0
#define STREAM_CURRENT         1
#define STREAM_END             2

typedef void* HDATASTREAM;

#define INVALID_HDATASTREAM_VALUE ((HDATASTREAM)0)


HDATASTREAM CreateDataStream(
	void* lpData, /* The data to be used in the stream. */
	unsigned long dwDesiredAccess, /* Access (Read or write). */
	unsigned long dwCreationDisposition, /* How to create the stream. */
	unsigned long dwSize); /* How big the stream is. */

int CloseDataStream(
	HDATASTREAM hDataStream);

unsigned long GetDataStreamSize(
	HDATASTREAM hDataStream);

int ReadDataStream(
	HDATASTREAM hDataStream,
	void* lpBuffer, /* Buffer to recieve data read from the stream. */
	unsigned long nNumberOfBytesToRead, /* Number of bytes to read. */
	unsigned long* lpNumberOfBytesRead); /* Receives actual bytes read. */

int WriteDataStream(
	HDATASTREAM hDataStream,
	const void* lpBuffer, /* Buffer with data to be written to the stream. */
	unsigned long nNumberOfBytesToWrite, /* Number of bytes to write. */
	unsigned long* lpNumberOfBytesWritten); /* Receives actual bytes written. */

unsigned long SetDataStreamPointer(
	HDATASTREAM hDataStream,
	signed long lDistanceToMove, /* Distance to move. */
	unsigned long dwMoveMethod); /* Move method. */




#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DATASTREAM_H__ */