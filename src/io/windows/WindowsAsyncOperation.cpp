#include "WindowsAsyncOperation.h"

using namespace pipey::io;
using namespace pipey::memory::buffer;
using namespace pipey::common::exception;

CWindowsAsyncOperation::CWindowsAsyncOperation(IIOTarget *pTarget, IIOOperator *pOperator, 
											   IIOListener *pListener, IBuffer *pBuffer, 
											   uint32_t nRequestedBytes)
	:IIOBaseOperation(pTarget, pOperator, pListener, pBuffer, nRequestedBytes)
{
	memset(&m_overlapped, 0, sizeof(m_overlapped));
}

OVERLAPPED_EX *CWindowsAsyncOperation::GetOverlapped()
{
	return &m_overlapped;
}