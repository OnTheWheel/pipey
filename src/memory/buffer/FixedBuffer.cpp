#include "FixedBuffer.h"

using namespace pipey::common;
using namespace pipey::common::exception;
using namespace pipey::memory::buffer;
using namespace pipey::memory::pool;
using namespace pipey::util;

CFixedBuffer::CFixedBuffer(const unsigned long &nLength, const BYTE_ENDIAN &endian, IMemoryPool *pMemoryPool)
	: m_nCapacity(nLength),
	m_endian(endian),
	m_pMemoryPool(pMemoryPool),
	m_nReadIndex(0),
	m_nWriteIndex(0),
	m_nMarkedReadIndex(0),
	m_nMarkedWriteIndex(0)
{
	m_pMemoryPool->Allocate(m_nCapacity, m_hMemory);
	m_pBuffer = m_pMemoryPool->GetBuffer(m_hMemory);
}

CFixedBuffer::CFixedBuffer(const unsigned long &nLength, IMemoryPool *pMemoryPool)
	: m_nCapacity(nLength),
	m_endian(GetSystemByteOrder()),
	m_pMemoryPool(pMemoryPool),
	m_nReadIndex(0),
	m_nWriteIndex(0),
	m_nMarkedReadIndex(0),
	m_nMarkedWriteIndex(0)
{
	m_pMemoryPool->Allocate(m_nCapacity, m_hMemory);
	m_pBuffer = m_pMemoryPool->GetBuffer(m_hMemory);
}

CFixedBuffer::CFixedBuffer(const pipey::memory::buffer::IBuffer *pParentBuffer, const unsigned long &nIndex, const unsigned long &nLength)
	: m_nCapacity(nLength),
	m_nReadIndex(0),
	m_nWriteIndex(0),
	m_nMarkedReadIndex(0),
	m_nMarkedWriteIndex(0)
{
	if( nLength == 0 )
		throw EInvalidParameter("EInvalidParameter => CFixedBuffer::CFixedBuffer - nLength cannot be zero.");

	if( nIndex + nLength > pParentBuffer->GetCapacity() )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::CFixedBuffer - nIndex + nLength cannot be greater than capacity of a parent beffer.");

	m_pMemoryPool = pParentBuffer->GetMemoryPool();
	m_pMemoryPool->DuplicateHandle(pParentBuffer->GetMemoryHandle(), m_hMemory);
	m_pBuffer = m_pMemoryPool->GetBuffer(m_hMemory);

	m_endian = pParentBuffer->GetEndian();
}

CFixedBuffer::~CFixedBuffer()
{
}

unsigned long CFixedBuffer::GetCapacity() const
{
	return m_nCapacity;
}

unsigned long CFixedBuffer::GetReadIndex() const
{
	return m_nReadIndex;
}

unsigned long CFixedBuffer::GetWriteIndex() const
{
	return m_nWriteIndex;
}

unsigned long CFixedBuffer::GetReadableLength() const
{
	return m_nWriteIndex - m_nReadIndex;
}

const CMemoryHandle &CFixedBuffer::GetMemoryHandle() const
{
	return m_hMemory;
}

IMemoryPool *CFixedBuffer::GetMemoryPool() const
{
	return m_pMemoryPool;
}

unsigned long CFixedBuffer::SetReadIndex(const unsigned long &nIndex)
{
	if( nIndex > m_nWriteIndex )
		throw EInvalidParameter("EInvalidParameter => CFixedBuffer::SetReadIndex - nIndex cannot be greater than write index.");

	unsigned long prev = m_nReadIndex;
	m_nReadIndex = nIndex;
	return prev;
}

unsigned long CFixedBuffer::SetWriteIndex(const unsigned long &nIndex)
{
	if( nIndex < m_nReadIndex )
		throw EInvalidParameter("EInvalidParameter => CFixedBuffer::SetWriteIndex - nIndex cannot be less than read index.");

	if( nIndex > m_nCapacity )
		throw EInvalidParameter("EInvalidParameter => CFixedBuffer::SetWriteIndex - nIndex cannot be greater than capacity.");

	unsigned long prev = m_nWriteIndex;
	m_nWriteIndex = nIndex;
	return prev;
}

bool CFixedBuffer::IsReadable() const
{
	return GetReadableLength() > 0;
}

bool CFixedBuffer::IsWritable() const
{
	return m_nWriteIndex < m_nCapacity;
}

BYTE_ENDIAN CFixedBuffer::GetEndian() const
{
	return m_endian;
}

bool CFixedBuffer::EnsureWritable(const unsigned long &nLength)
{
	return m_nWriteIndex + nLength <= m_nCapacity;
}

unsigned long CFixedBuffer::Find(const char &value) const
{
	unsigned long result = Find(m_nReadIndex, m_nWriteIndex - m_nReadIndex, value);
	if( result ==  pipey::common::INVALID )
		return  pipey::common::INVALID;
	else return result - m_nReadIndex;
}

unsigned long CFixedBuffer::Find(const unsigned long &nStartIndex, const unsigned long &nLength, const char &value) const
{
	if( nStartIndex >= m_nCapacity )
		throw EInvalidParameter("EInvalidParameter => CFixedBuffer::Find - nStartIndex cannot be geq than capacity.");

	if( m_nCapacity - nStartIndex < nLength )
		throw EInvalidParameter("EInvalidParameter => CFixedBuffer::Find - nStartIndex + nLength cannot be greater than capacity.");

	for(unsigned long i = 0; i < nLength; i++)
	{
		if( m_pBuffer[nStartIndex + i] == value )
			return nStartIndex + i;
	}

	return pipey::common::INVALID;
}

char CFixedBuffer::ReadChar()
{
	if( sizeof(char) > GetReadableLength() )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::ReadChar - out of index.");

	return m_pBuffer[m_nReadIndex++];
}

unsigned char CFixedBuffer::ReadUnsignedChar()
{
	return (unsigned char)ReadChar();
}

short CFixedBuffer::ReadShort()
{
	return ReadUnsignedShort();
}

unsigned short CFixedBuffer::ReadUnsignedShort()
{
	if( sizeof(unsigned short) > GetReadableLength() )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::ReadUnsignedShort - out of index.");
	
	unsigned short value = *((unsigned short *)(m_pBuffer + m_nReadIndex));;
	if( GetSystemByteOrder() != GetEndian() )
		value = REVERSE_ORDER16(value);

	m_nReadIndex += sizeof(value);

	return value;
}

int CFixedBuffer::ReadInt()
{
	return ReadUnsignedInt();
}

unsigned int CFixedBuffer::ReadUnsignedInt()
{
	if( sizeof(unsigned int) > GetReadableLength() )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::ReadUnsignedInt - out of index.");
	
	unsigned int value = *((unsigned int *)(m_pBuffer + m_nReadIndex));
	if( GetSystemByteOrder() != GetEndian() )
		value = REVERSE_ORDER32(value);

	m_nReadIndex += sizeof(value);

	return value;
}

long CFixedBuffer::ReadLong()
{
	return ReadUnsignedLong();
}

unsigned long CFixedBuffer::ReadUnsignedLong()
{
	if( sizeof(unsigned long) > GetReadableLength() )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::ReadUnsignedLong - out of index.");
	
	unsigned long value = *((unsigned long *)(m_pBuffer + m_nReadIndex));
	if( GetSystemByteOrder() != GetEndian() )
		value = REVERSE_ORDER32(value);

	m_nReadIndex += sizeof(value);

	return value;
}

_integer64 CFixedBuffer::ReadInteger64()
{
	return ReadUnsignedInteger64();
}

_uinteger64 CFixedBuffer::ReadUnsignedInteger64()
{
	if( sizeof(_uinteger64) > GetReadableLength() )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::ReadUnsignedInteger64 - out of index.");
	
	_uinteger64 value = *((_uinteger64 *)(m_pBuffer + m_nReadIndex));
	if( GetSystemByteOrder() != GetEndian() )
		value = REVERSE_ORDER64(value);

	m_nReadIndex += sizeof(value);

	return value;
}

float CFixedBuffer::ReadFloat()
{
	if( sizeof(float) > GetReadableLength() )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::ReadFloat - out of index.");
	
	unsigned long value = *((unsigned long *)(m_pBuffer + m_nReadIndex));
	if( GetSystemByteOrder() != GetEndian() )
		value = REVERSE_ORDER32(value);

	m_nReadIndex += sizeof(float);

	return *( (float *)&value );
}

double CFixedBuffer::ReadDouble()
{
	if( sizeof(double) > GetReadableLength() )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::ReadDouble - out of index.");
	
	_uinteger64 value = *((_uinteger64 *)(m_pBuffer + m_nReadIndex));
	if( GetSystemByteOrder() != GetEndian() )
		value = REVERSE_ORDER64(value);

	m_nReadIndex += sizeof(double);

	return *( (double *)&value );
}

void CFixedBuffer::ReadBytes(char *pBuffer, const unsigned long &nLength)
{
	if( nLength > GetReadableLength() )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::ReadBytes - out of index.");

	memcpy(pBuffer, m_pBuffer + m_nReadIndex, nLength);
	m_nReadIndex += nLength;
}

void CFixedBuffer::ReadBytes(IBuffer *pBuffer, const unsigned long &nLength)
{
	if( nLength > GetReadableLength() )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::ReadBytes - out of index.");

	pBuffer->WriteBytes(m_pBuffer + m_nReadIndex, nLength);
	m_nReadIndex += nLength;
}

void CFixedBuffer::ReadString(char *pBuffer, const unsigned long &nSize)
{
	unsigned long end = Find(NULL);
	
	if( end == pipey::common::INVALID )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::ReadString - cannot find end of the string.");
	if( nSize < end )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::ReadString - specified buffer is too small.");

	memcpy(pBuffer, m_pBuffer + m_nReadIndex, end + 1);
	m_nReadIndex += (end + 1);
}

void CFixedBuffer::ReadString(IBuffer *pBuffer)
{
	unsigned long end = Find(NULL);
	
	if( end == pipey::common::INVALID )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::ReadString - cannot find end of the string.");

	pBuffer->WriteBytes( m_pBuffer + m_nReadIndex, end + 1 );
	m_nReadIndex += (end + 1);
}

char CFixedBuffer::GetChar(const unsigned long &nIndex) const
{
	if( m_nCapacity <= nIndex || sizeof(char) > m_nCapacity - nIndex )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetChar - out of index.");

	return m_pBuffer[nIndex];
}

unsigned char CFixedBuffer::GetUnsignedChar(const unsigned long &nIndex) const
{
	return (unsigned char)GetChar(nIndex);
}

short CFixedBuffer::GetShort(const unsigned long &nIndex) const
{
	return GetUnsignedShort(nIndex);
}

unsigned short CFixedBuffer::GetUnsignedShort(const unsigned long &nIndex) const
{
	if( m_nCapacity <= nIndex || sizeof(unsigned short) > m_nCapacity - nIndex )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetUnsignedShort - out of index.");

	unsigned short value = *((unsigned short *)(m_pBuffer + nIndex));
	if( GetSystemByteOrder() != GetEndian() )
		value = REVERSE_ORDER16(value);

	return value;
}

unsigned int CFixedBuffer::GetUnsignedInt(const unsigned long &nIndex) const
{
	if( m_nCapacity <= nIndex || sizeof(unsigned int) > m_nCapacity - nIndex )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetUnsignedInt - out of index.");

	unsigned int value = *((unsigned int *)(m_pBuffer + nIndex));
	if( GetSystemByteOrder() != GetEndian() )
		value = REVERSE_ORDER32(value);

	return value;

}

int CFixedBuffer::GetInt(const unsigned long &nIndex) const
{
	return GetUnsignedInt(nIndex);
}

unsigned long CFixedBuffer::GetUnsignedLong(const unsigned long &nIndex) const
{
	if( m_nCapacity <= nIndex || sizeof(unsigned long) > m_nCapacity - nIndex )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetUnsignedLong - out of index.");

	unsigned long value = *((unsigned long *)(m_pBuffer + nIndex));
	if( GetSystemByteOrder() != GetEndian() )
		value = REVERSE_ORDER32(value);

	return value;

}

long CFixedBuffer::GetLong(const unsigned long &nIndex) const
{
	return GetUnsignedLong(nIndex);
}

_uinteger64 CFixedBuffer::GetUnsignedInteger64(const unsigned long &nIndex) const
{
	if( m_nCapacity <= nIndex || sizeof(_uinteger64) > m_nCapacity - nIndex )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetUnsignedInteger64 - out of index.");

	_uinteger64 value = *((_uinteger64 *)(m_pBuffer + nIndex));
	if( GetSystemByteOrder() != GetEndian() )
		value = REVERSE_ORDER64(value);

	return value;
}

_integer64 CFixedBuffer::GetInteger64(const unsigned long &nIndex) const
{
	return GetInteger64(nIndex);
}

float CFixedBuffer::GetFloat(const unsigned long &nIndex) const
{
	if( m_nCapacity <= nIndex || sizeof(float) > m_nCapacity - nIndex )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetFloat - out of index.");

	unsigned long value = *((unsigned long *)(m_pBuffer + nIndex));
	if( GetSystemByteOrder() != GetEndian() )
		value = REVERSE_ORDER32(value);

	return *( (float *)&value );
}

double CFixedBuffer::GetDouble(const unsigned long &nIndex) const
{
	if( m_nCapacity <= nIndex || sizeof(double) > m_nCapacity - nIndex )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetDouble - out of index.");

	_uinteger64 value = *((_uinteger64 *)(m_pBuffer + nIndex));
	if( GetSystemByteOrder() != GetEndian() )
		value = REVERSE_ORDER64(value);

	return *( (double *)&value );
}


void CFixedBuffer::GetBytes(const unsigned long &nIndex, char *pBuffer, const unsigned long &nLength) const
{
	if( m_nCapacity <= nIndex || nLength > m_nCapacity - nIndex )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetBytes - out of index.");

	memcpy(pBuffer, m_pBuffer + nIndex, nLength);
}

void CFixedBuffer::GetBytes(const unsigned long &nIndex, IBuffer *pBuffer, const unsigned long &nLength) const
{
	if( m_nCapacity <= nIndex || nLength > m_nCapacity - nIndex )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetBytes - out of index.");

	pBuffer->WriteBytes(m_pBuffer + nIndex, nLength);
}

void CFixedBuffer::GetString(const unsigned long &nIndex, char *pBuffer, const unsigned long &nSize) const
{
	if( m_nCapacity <= nIndex )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetString - out of index.");

	unsigned long end = Find(nIndex, m_nCapacity - nIndex, NULL);
	
	if( end == pipey::common::INVALID )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetString - cannot find end of the string.");
	if( nSize < end - nIndex + 1 )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetString - specified buffer is too small.");

	memcpy(pBuffer, m_pBuffer + nIndex, end - nIndex + 1);
}

void CFixedBuffer::GetString(const unsigned long &nIndex, IBuffer *pBuffer) const
{
	if( m_nCapacity <= nIndex )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetString - out of index.");

	unsigned long end = Find(nIndex, m_nCapacity - nIndex, NULL);
	
	if( end == pipey::common::INVALID )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::ReadString - cannot find end of the string.");

	pBuffer->WriteBytes( m_pBuffer + m_nReadIndex, end + 1 );
}

void CFixedBuffer::WriteChar(const char &value)
{
	if( EnsureWritable(sizeof(value)) ) 
		m_pBuffer[ m_nWriteIndex++ ] = value;
	else throw EOutOfBound("EOutOfBound => CFixedBuffer::WriteChar - out of index.");
}

void CFixedBuffer::WriteUnsignedChar(const unsigned char &value)
{
	WriteChar(value);
}

void CFixedBuffer::WriteShort(const short &value)
{
	WriteUnsignedShort(value);
}

void CFixedBuffer::WriteUnsignedShort(const unsigned short &value)
{
	if( EnsureWritable(sizeof(value)) ) {
		unsigned short *pBuffer = (unsigned short *)( m_pBuffer + m_nWriteIndex );

		if( GetSystemByteOrder() == GetEndian() )
			*pBuffer = value;
		else *pBuffer = REVERSE_ORDER16(value);

		m_nWriteIndex += sizeof(value);
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::WriteUnsignedShort - out of index.");
}

void CFixedBuffer::WriteInt(const int &value)
{
	WriteUnsignedInt(value);
}

void CFixedBuffer::WriteUnsignedInt(const unsigned int &value)
{
	if( EnsureWritable(sizeof(value)) ) {
		unsigned int *pBuffer = (unsigned int *)( m_pBuffer + m_nWriteIndex );

		if( GetSystemByteOrder() == GetEndian() )
			*pBuffer = value;
		else *pBuffer = REVERSE_ORDER32(value);

		m_nWriteIndex += sizeof(value);
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::WriteUnsignedInt - out of index.");
}

void CFixedBuffer::WriteLong(const long &value)
{
	WriteUnsignedLong(value);
}

void CFixedBuffer::WriteUnsignedLong(const unsigned long &value)
{
	if( EnsureWritable(sizeof(value)) ) {
		unsigned long *pBuffer = (unsigned long *)( m_pBuffer + m_nWriteIndex );

		if( GetSystemByteOrder() == GetEndian() )
			*pBuffer = value;
		else *pBuffer = REVERSE_ORDER32(value);

		m_nWriteIndex += sizeof(value);
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::WriteUnsignedLong - out of index.");
}

void CFixedBuffer::WriteInteger64(const _integer64 &value)
{
	WriteUnsignedInteger64(value);	
}

void CFixedBuffer::WriteUnsignedInteger64(const _uinteger64 &value)
{
	if( EnsureWritable(sizeof(value)) ) {
		_uinteger64 *pBuffer = (_uinteger64 *)( m_pBuffer + m_nWriteIndex );

		if( GetSystemByteOrder() == GetEndian() )
			*pBuffer = value;
		else *pBuffer = REVERSE_ORDER64(value);

		m_nWriteIndex += sizeof(value);
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::WriteUnsignedInteger64 - out of index.");
}

void CFixedBuffer::WriteFloat(const float &value)
{
	if( EnsureWritable(sizeof(value)) ) {
		unsigned long *pValue = (unsigned long *)&value;
		unsigned long *pBuffer = (unsigned long *)( m_pBuffer + m_nWriteIndex );

		if( GetSystemByteOrder() == GetEndian() )
			*pBuffer = *pValue;
		else *pBuffer = REVERSE_ORDER32(*pValue);

		m_nWriteIndex += sizeof(value);
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::WriteFloat - out of index.");
}

void CFixedBuffer::WriteDouble(const double &value)
{
	if( EnsureWritable(sizeof(value)) ) {
		_uinteger64 *pValue = (_uinteger64 *)&value;
		_uinteger64 *pBuffer = (_uinteger64 *)( m_pBuffer + m_nWriteIndex );

		if( GetSystemByteOrder() == GetEndian() )
			*pBuffer = *pValue;
		else *pBuffer = REVERSE_ORDER64(*pValue);

		m_nWriteIndex += sizeof(value);
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::WriteDouble - out of index.");
}

void CFixedBuffer::WriteBytes(const char *pBuffer, const unsigned long &nLength)
{
	if( EnsureWritable(nLength) ) {
		memcpy(m_pBuffer + m_nWriteIndex, pBuffer, nLength);
		m_nWriteIndex += nLength;
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::WriteDouble - out of index.");
}

void CFixedBuffer::WriteBytes(IBuffer *pBuffer, const unsigned long &nLength)
{
	if( EnsureWritable(nLength) ) {
		pBuffer->ReadBytes(m_pBuffer, nLength);
		m_nWriteIndex += nLength;
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::WriteDouble - out of index.");
}

void CFixedBuffer::WriteString(const char *pBuffer)
{
	unsigned long nLength = strlen(pBuffer) + 1;
	if( EnsureWritable(nLength) ) {
		memcpy(m_pBuffer + m_nWriteIndex, pBuffer, nLength);
		m_nWriteIndex += nLength;
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::WriteString - out of index.");
}

void CFixedBuffer::WriteString(IBuffer *pBuffer)
{
	pBuffer->ReadString(this);
}

void CFixedBuffer::WriteZero(const unsigned long &nLength)
{
	if( EnsureWritable(nLength) ) {
		memset(m_pBuffer + m_nWriteIndex, 0, nLength);
		m_nWriteIndex += nLength;
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::WriteDouble - out of index.");
}

void CFixedBuffer::SetChar(const unsigned long &nIndex, const char &value)
{
	if( m_nCapacity > nIndex && sizeof(value) <= m_nCapacity - nIndex )
		m_pBuffer[nIndex] = value;
	else throw EOutOfBound("EOutOfBound => CFixedBuffer::SetChar - out of index.");
}

void CFixedBuffer::SetUnsignedChar(const unsigned long &nIndex, const unsigned char &value)
{
	SetChar(nIndex, value);
}

void CFixedBuffer::SetUnsignedShort(const unsigned long &nIndex, const unsigned short &value)
{
	if( m_nCapacity > nIndex && sizeof(value) <= m_nCapacity - nIndex ) {
		unsigned short *pBuffer = (unsigned short *)( m_pBuffer + nIndex );

		if( GetSystemByteOrder() == GetEndian() )
			*pBuffer = value;
		else *pBuffer = REVERSE_ORDER16(value);
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::SetUnsignedShort - out of index.");
}

void CFixedBuffer::SetShort(const unsigned long &nIndex, const short &value)
{
	SetUnsignedShort(nIndex, value);
}

void CFixedBuffer::SetUnsignedInt(const unsigned long &nIndex, const unsigned int &value)
{
	if( m_nCapacity > nIndex && sizeof(value) <= m_nCapacity - nIndex ) {
		unsigned int *pBuffer = (unsigned int *)( m_pBuffer + nIndex );

		if( GetSystemByteOrder() == GetEndian() )
			*pBuffer = value;
		else *pBuffer = REVERSE_ORDER32(value);
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::SetUnsignedInt - out of index.");
}

void CFixedBuffer::SetInt(const unsigned long &nIndex, const int &value)
{
	SetUnsignedInt(nIndex, value);
}

void CFixedBuffer::SetUnsignedLong(const unsigned long &nIndex, const unsigned long &value)
{
	if( m_nCapacity > nIndex && sizeof(value) <= m_nCapacity - nIndex ) {
		unsigned long *pBuffer = (unsigned long *)( m_pBuffer + nIndex );

		if( GetSystemByteOrder() == GetEndian() )
			*pBuffer = value;
		else *pBuffer = REVERSE_ORDER32(value);
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::SetUnsignedLong - out of index.");
}

void CFixedBuffer::SetLong(const unsigned long &nIndex, const long &value)
{
	SetUnsignedLong(nIndex, value);
}

void CFixedBuffer::SetUnsignedInteger64(const unsigned long &nIndex, const _uinteger64 &value)
{
	if( m_nCapacity > nIndex && sizeof(value) <= m_nCapacity - nIndex ) {
		_uinteger64 *pBuffer = (_uinteger64 *)( m_pBuffer + nIndex );

		if( GetSystemByteOrder() == GetEndian() )
			*pBuffer = value;
		else *pBuffer = REVERSE_ORDER64(value);
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::SetUnsignedInteger64 - out of index.");
}

void CFixedBuffer::SetInteger64(const unsigned long &nIndex, const _integer64 &value)
{
	SetUnsignedInteger64(nIndex, value);
}

void CFixedBuffer::SetFloat(const unsigned long &nIndex, const float &value)
{
	if( m_nCapacity > nIndex && sizeof(value) <= m_nCapacity - nIndex ) {
		unsigned long *pValue = (unsigned long *)&value;
		unsigned long *pBuffer = (unsigned long *)( m_pBuffer + nIndex );

		if( GetSystemByteOrder() == GetEndian() )
			*pBuffer = *pValue;
		else *pBuffer = REVERSE_ORDER32(*pValue);
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::SetFloat - out of index.");
}

void CFixedBuffer::SetDouble(const unsigned long &nIndex, const double &value)
{
	if( m_nCapacity > nIndex && sizeof(value) <= m_nCapacity - nIndex ) {
		_uinteger64 *pValue = (_uinteger64 *)&value;
		_uinteger64 *pBuffer = (_uinteger64 *)( m_pBuffer + nIndex );

		if( GetSystemByteOrder() == GetEndian() )
			*pBuffer = *pValue;
		else *pBuffer = REVERSE_ORDER64(*pValue);
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::SetDouble - out of index.");
}

void CFixedBuffer::SetBytes(const unsigned long &nIndex, const char *pBuffer, const unsigned long &nLength)
{
	if( m_nCapacity > nIndex && nLength <= m_nCapacity - nIndex )
		memcpy( m_pBuffer + nIndex, pBuffer, nLength);
	else throw EOutOfBound("EOutOfBound => CFixedBuffer::SetBytes - out of index.");
}

void CFixedBuffer::SetBytes(const unsigned long &nIndex, IBuffer *pBuffer, const unsigned long &nLength)
{
	if( m_nCapacity > nIndex && nLength <= m_nCapacity - nIndex )
		pBuffer->ReadBytes(m_pBuffer, nLength);
	else throw EOutOfBound("EOutOfBound => CFixedBuffer::SetBytes - out of index.");
}

void CFixedBuffer::SetString(const unsigned long &nIndex, const char *pBuffer)
{
	unsigned long nLength = strlen(pBuffer) + 1;
	if( m_nCapacity > nIndex && nLength <= m_nCapacity - nIndex )
		memcpy(m_pBuffer + nIndex, pBuffer, nLength);
	else throw EOutOfBound("EOutOfBound => CFixedBuffer::SetString - out of index.");
}

void CFixedBuffer::SetString(const unsigned long &nIndex, IBuffer *pBuffer)
{
	unsigned long nLength = pBuffer->Find(NULL);
	if( nLength != ::pipey::common::INVALID && m_nCapacity > nIndex && nLength + 1 <= m_nCapacity - nIndex )
		pBuffer->ReadString(m_pBuffer + nIndex, nLength + 1);
	else throw EOutOfBound("EOutOfBound => CFixedBuffer::SetString - out of index.");
}

void CFixedBuffer::SetZero(const unsigned long &nIndex, const unsigned long &nLength)
{
	if( m_nCapacity > nIndex && nLength <= m_nCapacity - nIndex )
		memset(m_pBuffer + nIndex, 0, nLength);
	else throw EOutOfBound("EOutOfBound => CFixedBuffer::SetZero - out of index.");
}

void CFixedBuffer::SetZero()
{
	SetZero(0, m_nCapacity);
}

unsigned long CFixedBuffer::Skip(const unsigned long &nLength)
{
	if( nLength > GetReadableLength() )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::Skip - out of index.");

	return (m_nReadIndex += nLength);
}

unsigned long CFixedBuffer::SkipTo(const char &target)
{
	unsigned long position = Find(target);
	if(position == ::pipey::common::INVALID) 
		return ::pipey::common::INVALID;
	else return (m_nReadIndex += position);
}

unsigned long CFixedBuffer::SkipAll()
{
	return Skip( GetReadableLength() );
}