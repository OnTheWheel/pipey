#include "BasicMemoryPool.h"
#include <stdlib.h>
#include "../../common/common.h"

using namespace pipey::common::exception;
using namespace pipey::memory::pool;

void CBasicMemoryPool::Allocate(uint32_t nSize, CMemoryHandle &rHandle)
{
	if( rHandle ) 
		throw EInvalidParameter("EInvalidParameter => CBasicMemoryPool::Allocate - A memory handle specified is already initiated.");

	char *pBuffer = (char *)malloc(nSize);

	if(pBuffer == NULL)
		throw EMemoryAllocation("EMemoryAllocation => CBasicMemoryPool::Allocate - Cannot allocate memory.");
	m_handles[pBuffer] = 0;
	InitHandle(rHandle, pBuffer);
}

void CBasicMemoryPool::CloseHandle(CMemoryHandle &rHandle)
{
	if( rHandle ) {
		if( IsMine(rHandle) ) {
			char *pBuffer = GetHandleTarget(rHandle);
			m_handles[pBuffer] -= 1;
			NullifyHandle(rHandle);

			if( m_handles[pBuffer] == 0 )	{
				free(pBuffer);
				m_handles.erase(pBuffer);
			}
		} else throw EInvalidParameter("EInvalidParameter => CBasicMemoryPool::CloseHandle - Specified memory handle is owned by another memory pool.");
	} else throw EInvalidParameter("EInvalidParameter => CBasicMemoryPool::CloseHandle - Specified memory handle is not properly initiated.");

}

void CBasicMemoryPool::DuplicateHandle(const CMemoryHandle & rSource, CMemoryHandle & rTarget)
{
	if( rSource ) {
		if( rTarget ) 
			throw EInvalidParameter("EInvalidParameter => CBasicMemoryPool::DuplicateHandle - Specified memory handle(rTarget) is already initiated.");

		if( IsMine(rSource) ) {
			char *pBuffer = GetHandleTarget(rSource);
			InitHandle(rTarget, pBuffer);
			m_handles[pBuffer]++;
		} else throw ::pipey::common::exception::EInvalidParameter("EInvalidParameter => CBasicMemoryPool::DuplicateHandle - Specified memory handle(rSource) is owned by other memory pool.");
	} else throw ::pipey::common::exception::EInvalidParameter("EInvalidParameter => CBasicMemoryPool::DuplicateHandle - Specified memory handle(rSource) is not properly initiated.");
}

char *CBasicMemoryPool::GetBuffer(const  CMemoryHandle &rHandle)
{
	 if( rHandle ) {
		if( IsMine(rHandle) ) {
			return GetHandleTarget(rHandle);
		} else throw EInvalidParameter("EInvalidParameter => CBasicMemoryPool::GetBuffer - Specified memory handle is owned by another memory pool.");
	} else throw EInvalidParameter("EInvalidParameter => CBasicMemoryPool::GetBuffer - Specified memory handle is not properly initiated.");
}