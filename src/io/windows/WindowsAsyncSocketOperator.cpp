#include "WindowsAsyncSocketOperator.h"
#include "WindowsAsyncOperation.h"

using namespace pipey::io;
using namespace pipey::common::exception;
using namespace pipey::memory::buffer;

void CWindowsAsyncSocketReader::Process(IIOMutableOperation *pOperation)
{
	if( pOperation ) {
		CWindowsAsyncOperation *pAsync = dynamic_cast<CWindowsAsyncOperation *>(pOperation);
		if( pAsync ) {
			IBuffer *pBuffer = pAsync->GetBuffer();
			pBuffer->SetWriteIndex(pBuffer->GetWriteIndex() + pAsync->GetTransferredBytes());

			if( HasMoreToDo(pOperation) ) {
				IO_BUFFER *pIoBuffer = pBuffer->GetIOBuffer();

				OVERLAPPED_EX *pOverlapped = pAsync->GetOverlapped();
				memset(pOverlapped, 0, sizeof(*pOverlapped));
				pOverlapped->pOperator = this;

				DWORD nFlag = 0;

				int32_t result = WSARecv((SOCKET)pAsync->GetTarget()->GetDescriptor(), 
					pIoBuffer->pBuffer, 
					pIoBuffer->nBuffer, 
					NULL, &nFlag, 
					pOverlapped, NULL);

				int err;
				if( result == SOCKET_ERROR && (err = WSAGetLastError()) != WSA_IO_PENDING ) {
					pAsync->SetDone(true);
					pAsync->SetSuccess(false);
					pAsync->SetCause(err);
				}
			} else {
				pAsync->SetDone(true);
				pAsync->SetSuccess(true);
			}
		} else throw EInvalidParameter("EInvalidParameter =>  CWindowsAsyncSocketReader::DoMore - pOperation should be type of (CWindowsAsyncOperation *).");
	} else throw EInvalidParameter("EInvalidParameter => CWindowsAsyncSocketReader::DoMore - pOperation cannot be NULL.");
}


bool CWindowsAsyncSocketReader::HasMoreToDo(const IIOOperation *pOperation) const
{
	if( pOperation ) {
		return pOperation->GetRequestedBytes() > pOperation->GetTotalTransferredBytes();
	} else throw EInvalidParameter("EInvalidParameter => CWindowsAsyncSocketReader::HasMore - pOperation cannot be NULL.");
}

void CWindowsAsyncSocketWriter::Process(IIOMutableOperation *pOperation)
{

}

bool CWindowsAsyncSocketWriter::HasMoreToDo(const IIOOperation *pOperation) const
{
	if( pOperation ) {
		return pOperation->GetRequestedBytes() > pOperation->GetTotalTransferredBytes();
	} else throw EInvalidParameter("EInvalidParameter => CWindowsAsyncSocketWriter::HasMore - pOperation cannot be NULL.");
}