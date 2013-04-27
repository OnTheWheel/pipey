#include "IOBaseOperation.h"

using namespace pipey::io;
using namespace pipey::memory::buffer;
using namespace pipey::common::exception;

IIOBaseOperation::IIOBaseOperation(IIOTarget *pTarget, IIOOperator *pOperator, IIOListener *pListener, IBuffer *pBuffer, uint32_t nRequestedBytes)
	:m_pTarget(pTarget),
	m_pOperator(pOperator),
	m_pListener(pListener),
	m_pBuffer(pBuffer),
	m_nRequestedBytes(nRequestedBytes),
	m_nTotalTransfferedBytes(0),
	m_nTransfferedBytes(0),
	m_bDone(false),
	m_bSuccess(false),
	m_nCause(0)
{
}

bool IIOBaseOperation::IsDone() const
{
	return m_bDone;
}

bool IIOBaseOperation::IsSuccess() const
{
	return m_bSuccess;
}

uint32_t IIOBaseOperation::GetCause() const
{
	return m_nCause;
}

const IIOTarget *IIOBaseOperation::GetTarget() const
{
	return m_pTarget;
}

uint32_t IIOBaseOperation::GetTransferredBytes() const
{
	return m_nTransfferedBytes;
}

uint32_t IIOBaseOperation::GetTotalTransferredBytes() const
{
	return m_nTotalTransfferedBytes;
}

uint32_t IIOBaseOperation::GetRequestedBytes() const
{
	return m_nRequestedBytes;
}

const IIOListener *IIOBaseOperation::GetListener() const
{
	return m_pListener;
}

bool IIOBaseOperation::HasMoreToDo() const
{
	return m_pOperator->HasMoreToDo(this);
}

void IIOBaseOperation::SetDone(bool bDone)
{
	m_bDone = bDone;
}

void IIOBaseOperation::SetSuccess(bool bSuccess)
{
	m_bSuccess = bSuccess;
}

void IIOBaseOperation::SetCause(uint32_t nCause)
{
	m_nCause = nCause;
}

void IIOBaseOperation::SetTransferredBytes(uint32_t nBytes)
{
	m_nTransfferedBytes = nBytes;
	m_nTotalTransfferedBytes += nBytes;
}

IBuffer *IIOBaseOperation::GetBuffer()
{
	return m_pBuffer;
}

void IIOBaseOperation::DoMore()
{
	m_pOperator->DoMore(this);
}