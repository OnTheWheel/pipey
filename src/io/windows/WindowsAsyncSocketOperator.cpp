#include "WindowsAsyncSocketOperator.h"
#include "WindowsAsyncOperation.h"

using namespace pipey::io;
using namespace pipey::common::exception;
using namespace pipey::memory::buffer;

void CWindowsAsyncSocketReader::DoMore(IIOMutableOperation *pOperation)
{
	if( pOperation ) {
		if( HasMoreToDo(pOperation) ) {
			CWindowsAsyncOperation *pAsync = dynamic_cast<CWindowsAsyncOperation *>(pOperation);
			if( pAsync ) {
				IBuffer *pBuffer = pAsync->GetBuffer();
				pBuffer->SetWriteIndex(pBuffer->GetWriteIndex() + pAsync->GetTransferredBytes());
				IO_BUFFER *pIoBuffer = pBuffer->GetIOBuffer();

				OVERLAPPED_EX *pOverlapped = pAsync->GetOverlapped();
				memset(pOverlapped, 0, sizeof(*pOverlapped));
				
				DWORD nRecvd = 0;
				DWORD nFlag = 0;

				int32_t result = WSARecv((SOCKET)pAsync->GetTarget()->GetDescriptor(), 
											pIoBuffer->pBuffer, 
											pIoBuffer->nBuffer, 
											&nRecvd, &nFlag, 
											pOverlapped, NULL);

				pAsync->SetTransferredBytes(nRecvd);

				if( result == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING ) {
					pAsync->SetDone(true);
					pAsync->SetSuccess(false);
					pAsync->SetCause(WSAGetLastError());
				} else if( ! HasMoreToDo(pAsync) ) {
					pAsync->SetDone(true);
					pAsync->SetSuccess(true);
				}
			} else throw EInvalidParameter("EInvalidParameter =>  CWindowsAsyncSocketReader::DoMore - pOperation should be type of (CWindowsAsyncOperation *).");
		} else throw EInvalidState("EInvalidState =>  CWindowsAsyncSocketReader::DoMore - Has nothing to do more.");
	} else throw EInvalidParameter("EInvalidParameter => CWindowsAsyncSocketReader::DoMore - pOperation cannot be NULL.");
}

bool CWindowsAsyncSocketReader::HasMoreToDo(const IIOOperation *pOperation) const
{
	if( pOperation ) {
		return pOperation->GetRequestedBytes() > pOperation->GetTotalTransferredBytes();
	} else throw EInvalidParameter("EInvalidParameter => CWindowsAsyncSocketReader::HasMore - pOperation cannot be NULL.");
}

void CWindowsAsyncSocketWriter::DoMore(IIOMutableOperation *pOperation)
{
	
}

bool CWindowsAsyncSocketWriter::HasMoreToDo(const IIOOperation *pOperation) const
{
	if( pOperation ) {
		return pOperation->GetRequestedBytes() > pOperation->GetTotalTransferredBytes();
	} else throw EInvalidParameter("EInvalidParameter => CWindowsAsyncSocketWriter::HasMore - pOperation cannot be NULL.");
}