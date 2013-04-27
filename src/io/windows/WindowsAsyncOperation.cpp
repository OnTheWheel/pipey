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
	m_hEvent = NULL;
	m_pRoutine = NULL;
}

CWindowsAsyncOperation::CWindowsAsyncOperation(IIOTarget *pTarget, IIOOperator *pOperator, 
											   IIOListener *pListener, IBuffer *pBuffer, 
											   uint32_t nRequestedBytes,
											   HANDLE hHandle)
	:IIOBaseOperation(pTarget, pOperator, pListener, pBuffer, nRequestedBytes)
{
	memset(&m_overlapped, 0, sizeof(m_overlapped));
	m_hEvent = hHandle;
	m_pRoutine = NULL;
}

CWindowsAsyncOperation::CWindowsAsyncOperation(IIOTarget *pTarget, IIOOperator *pOperator, 
											   IIOListener *pListener, IBuffer *pBuffer, 
											   uint32_t nRequestedBytes,
											   LPWSAOVERLAPPED_COMPLETION_ROUTINE pRoutine)
	:IIOBaseOperation(pTarget, pOperator, pListener, pBuffer, nRequestedBytes)
{
	memset(&m_overlapped, 0, sizeof(m_overlapped));
	m_hEvent = NULL;
	m_pRoutine = pRoutine;
}

OVERLAPPED_EX *CWindowsAsyncOperation::FillOverlapped()
{
	memset(&m_overlapped, 0, sizeof(m_overlapped));
	m_overlapped.pOperation = this;
	m_overlapped.hEvent = m_hEvent;
	
	return &m_overlapped;
}

LPWSAOVERLAPPED_COMPLETION_ROUTINE CWindowsAsyncOperation::GetCompletionRoutine() const
{
	return m_pRoutine;
}