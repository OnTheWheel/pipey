#include "FixedBuffer.h"

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

unsigned long CFixedBuffer::Find(const char &value)
{
	return Find(m_nReadIndex, m_nWriteIndex - m_nReadIndex, value);
}

unsigned long CFixedBuffer::Find(const unsigned long &nStartIndex, const unsigned long &nLength, const char &value)
{
	if( nStartIndex >= m_nCapacity )
		throw EInvalidParameter("EInvalidParameter => CFixedBuffer::Find - nStartIndex cannot be geq than capacity.");

	if( m_nCapacity - nStartIndex < nLength )
		throw EInvalidParameter("EInvalidParameter => CFixedBuffer::Find - nStartIndex + nLength cannot be greater than capacity.");

	for(unsigned long i = nStartIndex; i < nLength; i++)
	{
		if( m_pBuffer[i] == value )
			return nStartIndex + i;
	}

	return pipey::common::INVALID;
}