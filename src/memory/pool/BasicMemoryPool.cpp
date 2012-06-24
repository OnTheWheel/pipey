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
	BASIC_BUF_INFO *pInfo = new BASIC_BUF_INFO(pBuffer);
	pInfo->nHandle--;
	pInfo->iter = m_buffers.insert(m_buffers.begin(), pInfo);
	InitHandle(rHandle, pInfo);
}

void CBasicMemoryPool::Release(CMemoryHandle &rHandle)
{
	if( rHandle ) {
		if( IsMine(rHandle) ) {
			BASIC_BUF_INFO *pBuffer = (BASIC_BUF_INFO *)GetHandleTarget(rHandle);
			if( pBuffer->bValid ){
				free(pBuffer->pBuffer);
				m_buffers.erase(pBuffer->iter);
				pBuffer->bValid = false;
			} else throw EInvalidState("EInvalidState => CBasicMemoryPool::Release - memory area is already released.");
		} else throw EInvalidParameter("EInvalidParameter => CBasicMemoryPool::Release - Specified memory handle is owned by another memory pool.");
	} else throw EInvalidParameter("EInvalidParameter => CBasicMemoryPool::Release - Specified memory handle is not properly initiated.");
}

void CBasicMemoryPool::CloseHandle(CMemoryHandle &rHandle)
{
	if( rHandle ) {
		if( IsMine(rHandle) ) {
			BASIC_BUF_INFO *pBuffer = (BASIC_BUF_INFO *)GetHandleTarget(rHandle);
			pBuffer->nHandle--;
			if( pBuffer->nHandle == 0 ){
				if( pBuffer->bValid ) {
					free(pBuffer->pBuffer);
					m_buffers.erase(pBuffer->iter);
				}
				delete pBuffer;
			}
			NullifyHandle(rHandle);

		} else throw EInvalidParameter("EInvalidParameter => CBasicMemoryPool::CloseHandle - Specified memory handle is owned by another memory pool.");
	} else throw EInvalidParameter("EInvalidParameter => CBasicMemoryPool::CloseHandle - Specified memory handle is not properly initiated.");

}

void CBasicMemoryPool::DuplicateHandle(const CMemoryHandle & rSource, CMemoryHandle & rTarget)
{
	if( rSource ) {
		if( rTarget ) 
			throw EInvalidParameter("EInvalidParameter => CBasicMemoryPool::DuplicateHandle - Specified memory handle(rTarget) is already initiated.");

		if( IsMine(rSource) ) {
			BASIC_BUF_INFO *pBuffer = (BASIC_BUF_INFO *)GetHandleTarget(rSource);
			InitHandle(rTarget, pBuffer);
			pBuffer->nHandle++;
		} else throw ::pipey::common::exception::EInvalidParameter("EInvalidParameter => CBasicMemoryPool::DuplicateHandle - Specified memory handle(rSource) is owned by other memory pool.");
	} else throw ::pipey::common::exception::EInvalidParameter("EInvalidParameter => CBasicMemoryPool::DuplicateHandle - Specified memory handle(rSource) is not properly initiated.");
}

char *CBasicMemoryPool::GetBuffer(const  CMemoryHandle &rHandle)
{
	 if( rHandle ) {
		if( IsMine(rHandle) ) {
			BASIC_BUF_INFO *pBuffer = (BASIC_BUF_INFO *)GetHandleTarget(rHandle);
			if( pBuffer->bValid )
				return pBuffer->pBuffer;
			else throw EInvalidState("EInvalidState => CBasicMemoryPool::GetBuffer - memory area is already released.");
		} else throw EInvalidParameter("EInvalidParameter => CBasicMemoryPool::GetBuffer - Specified memory handle is owned by another memory pool.");
	} else throw EInvalidParameter("EInvalidParameter => CBasicMemoryPool::GetBuffer - Specified memory handle is not properly initiated.");
}