#include "FixedBuffer.h"

using namespace pipey::common;
using namespace pipey::common::exception;
using namespace pipey::memory::buffer;
using namespace pipey::memory::pool;
using namespace pipey::util;

CFixedBuffer::CFixedBuffer(const uint32_t &nLength, const BYTE_ENDIAN &endian, IMemoryPool *pMemoryPool)
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

CFixedBuffer::CFixedBuffer(const uint32_t &nLength, IMemoryPool *pMemoryPool)
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

CFixedBuffer::CFixedBuffer(const pipey::memory::buffer::IBuffer *pParentBuffer, const uint32_t &nIndex, const uint32_t &nLength)
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

uint32_t CFixedBuffer::GetCapacity() const
{
	return m_nCapacity;
}

uint32_t CFixedBuffer::GetReadIndex() const
{
	return m_nReadIndex;
}

uint32_t CFixedBuffer::GetWriteIndex() const
{
	return m_nWriteIndex;
}

uint32_t CFixedBuffer::GetReadableLength() const
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

uint32_t CFixedBuffer::SetReadIndex(const uint32_t &nIndex)
{
	if( nIndex > m_nWriteIndex )
		throw EInvalidParameter("EInvalidParameter => CFixedBuffer::SetReadIndex - nIndex cannot be greater than write index.");

	uint32_t prev = m_nReadIndex;
	m_nReadIndex = nIndex;
	return prev;
}

uint32_t CFixedBuffer::SetWriteIndex(const uint32_t &nIndex)
{
	if( nIndex < m_nReadIndex )
		throw EInvalidParameter("EInvalidParameter => CFixedBuffer::SetWriteIndex - nIndex cannot be less than read index.");

	if( nIndex > m_nCapacity )
		throw EInvalidParameter("EInvalidParameter => CFixedBuffer::SetWriteIndex - nIndex cannot be greater than capacity.");

	uint32_t prev = m_nWriteIndex;
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

bool CFixedBuffer::EnsureWritable(const uint32_t &nLength)
{
	return m_nWriteIndex + nLength <= m_nCapacity;
}

uint32_t CFixedBuffer::Find(const char &value) const
{
	uint32_t result = Find(m_nReadIndex, m_nWriteIndex - m_nReadIndex, value);
	if( result ==  pipey::common::INVALID )
		return  pipey::common::INVALID;
	else return result - m_nReadIndex;
}

uint32_t CFixedBuffer::Find(const uint32_t &nStartIndex, const uint32_t &nLength, const char &value) const
{
	if( nStartIndex >= m_nCapacity )
		throw EInvalidParameter("EInvalidParameter => CFixedBuffer::Find - nStartIndex cannot be geq than capacity.");

	if( m_nCapacity - nStartIndex < nLength )
		throw EInvalidParameter("EInvalidParameter => CFixedBuffer::Find - nStartIndex + nLength cannot be greater than capacity.");

	for(uint32_t i = 0; i < nLength; i++)
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

int16_t CFixedBuffer::ReadInteger16()
{
	return ReadUnsignedInteger16();
}

uint16_t CFixedBuffer::ReadUnsignedInteger16()
{
	if( sizeof(uint16_t) > GetReadableLength() )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::ReadUnsignedInteger16 - out of index.");
	
	uint16_t value = *((uint16_t *)(m_pBuffer + m_nReadIndex));;
	if( GetSystemByteOrder() != GetEndian() )
		value = REVERSE_ORDER16(value);

	m_nReadIndex += sizeof(value);

	return value;
}

int32_t CFixedBuffer::ReadInteger32()
{
	return ReadUnsignedInteger32();
}

uint32_t CFixedBuffer::ReadUnsignedInteger32()
{
	if( sizeof(uint32_t) > GetReadableLength() )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::ReadUnsignedInt - out of index.");
	
	uint32_t value = *((uint32_t *)(m_pBuffer + m_nReadIndex));
	if( GetSystemByteOrder() != GetEndian() )
		value = REVERSE_ORDER32(value);

	m_nReadIndex += sizeof(value);

	return value;
}

int64_t CFixedBuffer::ReadInteger64()
{
	return ReadUnsignedInteger64();
}

uint64_t CFixedBuffer::ReadUnsignedInteger64()
{
	if( sizeof(uint64_t) > GetReadableLength() )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::ReadUnsignedInteger64 - out of index.");
	
	uint64_t value = *((uint64_t *)(m_pBuffer + m_nReadIndex));
	if( GetSystemByteOrder() != GetEndian() )
		value = REVERSE_ORDER64(value);

	m_nReadIndex += sizeof(value);

	return value;
}

float CFixedBuffer::ReadFloat()
{
	if( sizeof(float) > GetReadableLength() )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::ReadFloat - out of index.");
	
	uint32_t value = *((uint32_t *)(m_pBuffer + m_nReadIndex));
	if( GetSystemByteOrder() != GetEndian() )
		value = REVERSE_ORDER32(value);

	m_nReadIndex += sizeof(float);

	return *( (float *)&value );
}

double CFixedBuffer::ReadDouble()
{
	if( sizeof(double) > GetReadableLength() )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::ReadDouble - out of index.");
	
	uint64_t value = *((uint64_t *)(m_pBuffer + m_nReadIndex));
	if( GetSystemByteOrder() != GetEndian() )
		value = REVERSE_ORDER64(value);

	m_nReadIndex += sizeof(double);

	return *( (double *)&value );
}

void CFixedBuffer::ReadBytes(char *pBuffer, const uint32_t &nLength)
{
	if( nLength > GetReadableLength() )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::ReadBytes - out of index.");

	memcpy(pBuffer, m_pBuffer + m_nReadIndex, nLength);
	m_nReadIndex += nLength;
}

void CFixedBuffer::ReadBytes(IBuffer *pBuffer, const uint32_t &nLength)
{
	if( nLength > GetReadableLength() )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::ReadBytes - out of index.");

	pBuffer->WriteBytes(m_pBuffer + m_nReadIndex, nLength);
	m_nReadIndex += nLength;
}

void CFixedBuffer::ReadString(char *pBuffer, const uint32_t &nSize)
{
	uint32_t end = Find(NULL);
	
	if( end == pipey::common::INVALID )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::ReadString - cannot find end of the string.");
	if( nSize < end )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::ReadString - specified buffer is too small.");

	memcpy(pBuffer, m_pBuffer + m_nReadIndex, end + 1);
	m_nReadIndex += (end + 1);
}

void CFixedBuffer::ReadString(IBuffer *pBuffer)
{
	uint32_t end = Find(NULL);
	
	if( end == pipey::common::INVALID )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::ReadString - cannot find end of the string.");

	pBuffer->WriteBytes( m_pBuffer + m_nReadIndex, end + 1 );
	m_nReadIndex += (end + 1);
}

char CFixedBuffer::GetChar(const uint32_t &nIndex) const
{
	if( m_nCapacity <= nIndex || sizeof(char) > m_nCapacity - nIndex )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetChar - out of index.");

	return m_pBuffer[nIndex];
}

unsigned char CFixedBuffer::GetUnsignedChar(const uint32_t &nIndex) const
{
	return (unsigned char)GetChar(nIndex);
}

int16_t CFixedBuffer::GetInteger16(const uint32_t &nIndex) const
{
	return GetUnsignedInteger16(nIndex);
}

uint16_t CFixedBuffer::GetUnsignedInteger16(const uint32_t &nIndex) const
{
	if( m_nCapacity <= nIndex || sizeof(uint16_t) > m_nCapacity - nIndex )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetUnsignedInteger16 - out of index.");

	uint16_t value = *((uint16_t *)(m_pBuffer + nIndex));
	if( GetSystemByteOrder() != GetEndian() )
		value = REVERSE_ORDER16(value);

	return value;
}

uint32_t CFixedBuffer::GetUnsignedInteger32(const uint32_t &nIndex) const
{
	if( m_nCapacity <= nIndex || sizeof(uint32_t) > m_nCapacity - nIndex )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetUnsignedInt - out of index.");

	uint32_t value = *((uint32_t *)(m_pBuffer + nIndex));
	if( GetSystemByteOrder() != GetEndian() )
		value = REVERSE_ORDER32(value);

	return value;

}

int32_t CFixedBuffer::GetInteger32(const uint32_t &nIndex) const
{
	return GetUnsignedInteger32(nIndex);
}


uint64_t CFixedBuffer::GetUnsignedInteger64(const uint32_t &nIndex) const
{
	if( m_nCapacity <= nIndex || sizeof(uint64_t) > m_nCapacity - nIndex )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetUnsignedInteger64 - out of index.");

	uint64_t value = *((uint64_t *)(m_pBuffer + nIndex));
	if( GetSystemByteOrder() != GetEndian() )
		value = REVERSE_ORDER64(value);

	return value;
}

int64_t CFixedBuffer::GetInteger64(const uint32_t &nIndex) const
{
	return GetInteger64(nIndex);
}

float CFixedBuffer::GetFloat(const uint32_t &nIndex) const
{
	if( m_nCapacity <= nIndex || sizeof(float) > m_nCapacity - nIndex )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetFloat - out of index.");

	uint32_t value = *((uint32_t *)(m_pBuffer + nIndex));
	if( GetSystemByteOrder() != GetEndian() )
		value = REVERSE_ORDER32(value);

	return *( (float *)&value );
}

double CFixedBuffer::GetDouble(const uint32_t &nIndex) const
{
	if( m_nCapacity <= nIndex || sizeof(double) > m_nCapacity - nIndex )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetDouble - out of index.");

	uint64_t value = *((uint64_t *)(m_pBuffer + nIndex));
	if( GetSystemByteOrder() != GetEndian() )
		value = REVERSE_ORDER64(value);

	return *( (double *)&value );
}


void CFixedBuffer::GetBytes(const uint32_t &nIndex, char *pBuffer, const uint32_t &nLength) const
{
	if( m_nCapacity <= nIndex || nLength > m_nCapacity - nIndex )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetBytes - out of index.");

	memcpy(pBuffer, m_pBuffer + nIndex, nLength);
}

void CFixedBuffer::GetBytes(const uint32_t &nIndex, IBuffer *pBuffer, const uint32_t &nLength) const
{
	if( m_nCapacity <= nIndex || nLength > m_nCapacity - nIndex )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetBytes - out of index.");

	pBuffer->WriteBytes(m_pBuffer + nIndex, nLength);
}

void CFixedBuffer::GetString(const uint32_t &nIndex, char *pBuffer, const uint32_t &nSize) const
{
	if( m_nCapacity <= nIndex )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetString - out of index.");

	uint32_t end = Find(nIndex, m_nCapacity - nIndex, NULL);
	
	if( end == pipey::common::INVALID )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetString - cannot find end of the string.");
	if( nSize < end - nIndex + 1 )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetString - specified buffer is too small.");

	memcpy(pBuffer, m_pBuffer + nIndex, end - nIndex + 1);
}

void CFixedBuffer::GetString(const uint32_t &nIndex, IBuffer *pBuffer) const
{
	if( m_nCapacity <= nIndex )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::GetString - out of index.");

	uint32_t end = Find(nIndex, m_nCapacity - nIndex, NULL);
	
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

void CFixedBuffer::WriteInteger16(const int16_t &value)
{
	WriteUnsignedInteger16(value);
}

void CFixedBuffer::WriteUnsignedInteger16(const uint16_t &value)
{
	if( EnsureWritable(sizeof(value)) ) {
		uint16_t *pBuffer = (uint16_t *)( m_pBuffer + m_nWriteIndex );

		if( GetSystemByteOrder() == GetEndian() )
			*pBuffer = value;
		else *pBuffer = REVERSE_ORDER16(value);

		m_nWriteIndex += sizeof(value);
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::WriteUnsignedInteger16 - out of index.");
}

void CFixedBuffer::WriteInteger32(const int32_t &value)
{
	WriteUnsignedInteger32(value);
}

void CFixedBuffer::WriteUnsignedInteger32(const uint32_t &value)
{
	if( EnsureWritable(sizeof(value)) ) {
		uint32_t *pBuffer = (uint32_t *)( m_pBuffer + m_nWriteIndex );

		if( GetSystemByteOrder() == GetEndian() )
			*pBuffer = value;
		else *pBuffer = REVERSE_ORDER32(value);

		m_nWriteIndex += sizeof(value);
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::WriteUnsignedInt - out of index.");
}

void CFixedBuffer::WriteInteger64(const int64_t &value)
{
	WriteUnsignedInteger64(value);	
}

void CFixedBuffer::WriteUnsignedInteger64(const uint64_t &value)
{
	if( EnsureWritable(sizeof(value)) ) {
		uint64_t *pBuffer = (uint64_t *)( m_pBuffer + m_nWriteIndex );

		if( GetSystemByteOrder() == GetEndian() )
			*pBuffer = value;
		else *pBuffer = REVERSE_ORDER64(value);

		m_nWriteIndex += sizeof(value);
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::WriteUnsignedInteger64 - out of index.");
}

void CFixedBuffer::WriteFloat(const float &value)
{
	if( EnsureWritable(sizeof(value)) ) {
		uint32_t *pValue = (uint32_t *)&value;
		uint32_t *pBuffer = (uint32_t *)( m_pBuffer + m_nWriteIndex );

		if( GetSystemByteOrder() == GetEndian() )
			*pBuffer = *pValue;
		else *pBuffer = REVERSE_ORDER32(*pValue);

		m_nWriteIndex += sizeof(value);
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::WriteFloat - out of index.");
}

void CFixedBuffer::WriteDouble(const double &value)
{
	if( EnsureWritable(sizeof(value)) ) {
		uint64_t *pValue = (uint64_t *)&value;
		uint64_t *pBuffer = (uint64_t *)( m_pBuffer + m_nWriteIndex );

		if( GetSystemByteOrder() == GetEndian() )
			*pBuffer = *pValue;
		else *pBuffer = REVERSE_ORDER64(*pValue);

		m_nWriteIndex += sizeof(value);
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::WriteDouble - out of index.");
}

void CFixedBuffer::WriteBytes(const char *pBuffer, const uint32_t &nLength)
{
	if( EnsureWritable(nLength) ) {
		memcpy(m_pBuffer + m_nWriteIndex, pBuffer, nLength);
		m_nWriteIndex += nLength;
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::WriteDouble - out of index.");
}

void CFixedBuffer::WriteBytes(IBuffer *pBuffer, const uint32_t &nLength)
{
	if( EnsureWritable(nLength) ) {
		pBuffer->ReadBytes(m_pBuffer, nLength);
		m_nWriteIndex += nLength;
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::WriteDouble - out of index.");
}

void CFixedBuffer::WriteString(const char *pBuffer)
{
	uint32_t nLength = strlen(pBuffer) + 1;
	if( EnsureWritable(nLength) ) {
		memcpy(m_pBuffer + m_nWriteIndex, pBuffer, nLength);
		m_nWriteIndex += nLength;
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::WriteString - out of index.");
}

void CFixedBuffer::WriteString(IBuffer *pBuffer)
{
	pBuffer->ReadString(this);
}

void CFixedBuffer::WriteZero(const uint32_t &nLength)
{
	if( EnsureWritable(nLength) ) {
		memset(m_pBuffer + m_nWriteIndex, 0, nLength);
		m_nWriteIndex += nLength;
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::WriteDouble - out of index.");
}

void CFixedBuffer::SetChar(const uint32_t &nIndex, const char &value)
{
	if( m_nCapacity > nIndex && sizeof(value) <= m_nCapacity - nIndex )
		m_pBuffer[nIndex] = value;
	else throw EOutOfBound("EOutOfBound => CFixedBuffer::SetChar - out of index.");
}

void CFixedBuffer::SetUnsignedChar(const uint32_t &nIndex, const unsigned char &value)
{
	SetChar(nIndex, value);
}

void CFixedBuffer::SetUnsignedInteger16(const uint32_t &nIndex, const uint16_t &value)
{
	if( m_nCapacity > nIndex && sizeof(value) <= m_nCapacity - nIndex ) {
		uint16_t *pBuffer = (uint16_t *)( m_pBuffer + nIndex );

		if( GetSystemByteOrder() == GetEndian() )
			*pBuffer = value;
		else *pBuffer = REVERSE_ORDER16(value);
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::SetUnsignedInteger16 - out of index.");
}

void CFixedBuffer::SetInteger16(const uint32_t &nIndex, const int16_t &value)
{
	SetUnsignedInteger16(nIndex, value);
}

void CFixedBuffer::SetUnsignedInteger32(const uint32_t &nIndex, const uint32_t &value)
{
	if( m_nCapacity > nIndex && sizeof(value) <= m_nCapacity - nIndex ) {
		uint32_t *pBuffer = (uint32_t *)( m_pBuffer + nIndex );

		if( GetSystemByteOrder() == GetEndian() )
			*pBuffer = value;
		else *pBuffer = REVERSE_ORDER32(value);
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::SetUnsignedInt - out of index.");
}

void CFixedBuffer::SetInteger32(const uint32_t &nIndex, const int32_t &value)
{
	SetUnsignedInteger32(nIndex, value);
}

void CFixedBuffer::SetUnsignedInteger64(const uint32_t &nIndex, const uint64_t &value)
{
	if( m_nCapacity > nIndex && sizeof(value) <= m_nCapacity - nIndex ) {
		uint64_t *pBuffer = (uint64_t *)( m_pBuffer + nIndex );

		if( GetSystemByteOrder() == GetEndian() )
			*pBuffer = value;
		else *pBuffer = REVERSE_ORDER64(value);
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::SetUnsignedInteger64 - out of index.");
}

void CFixedBuffer::SetInteger64(const uint32_t &nIndex, const int64_t &value)
{
	SetUnsignedInteger64(nIndex, value);
}

void CFixedBuffer::SetFloat(const uint32_t &nIndex, const float &value)
{
	if( m_nCapacity > nIndex && sizeof(value) <= m_nCapacity - nIndex ) {
		uint32_t *pValue = (uint32_t *)&value;
		uint32_t *pBuffer = (uint32_t *)( m_pBuffer + nIndex );

		if( GetSystemByteOrder() == GetEndian() )
			*pBuffer = *pValue;
		else *pBuffer = REVERSE_ORDER32(*pValue);
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::SetFloat - out of index.");
}

void CFixedBuffer::SetDouble(const uint32_t &nIndex, const double &value)
{
	if( m_nCapacity > nIndex && sizeof(value) <= m_nCapacity - nIndex ) {
		uint64_t *pValue = (uint64_t *)&value;
		uint64_t *pBuffer = (uint64_t *)( m_pBuffer + nIndex );

		if( GetSystemByteOrder() == GetEndian() )
			*pBuffer = *pValue;
		else *pBuffer = REVERSE_ORDER64(*pValue);
	} else throw EOutOfBound("EOutOfBound => CFixedBuffer::SetDouble - out of index.");
}

void CFixedBuffer::SetBytes(const uint32_t &nIndex, const char *pBuffer, const uint32_t &nLength)
{
	if( m_nCapacity > nIndex && nLength <= m_nCapacity - nIndex )
		memcpy( m_pBuffer + nIndex, pBuffer, nLength);
	else throw EOutOfBound("EOutOfBound => CFixedBuffer::SetBytes - out of index.");
}

void CFixedBuffer::SetBytes(const uint32_t &nIndex, IBuffer *pBuffer, const uint32_t &nLength)
{
	if( m_nCapacity > nIndex && nLength <= m_nCapacity - nIndex )
		pBuffer->ReadBytes(m_pBuffer, nLength);
	else throw EOutOfBound("EOutOfBound => CFixedBuffer::SetBytes - out of index.");
}

void CFixedBuffer::SetString(const uint32_t &nIndex, const char *pBuffer)
{
	uint32_t nLength = strlen(pBuffer) + 1;
	if( m_nCapacity > nIndex && nLength <= m_nCapacity - nIndex )
		memcpy(m_pBuffer + nIndex, pBuffer, nLength);
	else throw EOutOfBound("EOutOfBound => CFixedBuffer::SetString - out of index.");
}

void CFixedBuffer::SetString(const uint32_t &nIndex, IBuffer *pBuffer)
{
	uint32_t nLength = pBuffer->Find(NULL);
	if( nLength != ::pipey::common::INVALID && m_nCapacity > nIndex && nLength + 1 <= m_nCapacity - nIndex )
		pBuffer->ReadString(m_pBuffer + nIndex, nLength + 1);
	else throw EOutOfBound("EOutOfBound => CFixedBuffer::SetString - out of index.");
}

void CFixedBuffer::SetZero(const uint32_t &nIndex, const uint32_t &nLength)
{
	if( m_nCapacity > nIndex && nLength <= m_nCapacity - nIndex )
		memset(m_pBuffer + nIndex, 0, nLength);
	else throw EOutOfBound("EOutOfBound => CFixedBuffer::SetZero - out of index.");
}

void CFixedBuffer::SetZero()
{
	SetZero(0, m_nCapacity);
}

uint32_t CFixedBuffer::Skip(const uint32_t &nLength)
{
	if( nLength > GetReadableLength() )
		throw EOutOfBound("EOutOfBound => CFixedBuffer::Skip - out of index.");

	return (m_nReadIndex += nLength);
}

uint32_t CFixedBuffer::SkipTo(const char &target)
{
	uint32_t position = Find(target);
	if(position == ::pipey::common::INVALID) 
		return ::pipey::common::INVALID;
	else return (m_nReadIndex += position);
}

uint32_t CFixedBuffer::SkipAll()
{
	return Skip( GetReadableLength() );
}