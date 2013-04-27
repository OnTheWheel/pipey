#include "WindowsIOCPWorkerRoutine.h"
#include "WindowsAsyncOperation.h"
using namespace pipey::io;

void CWindowsIOCPWorkerRoutine::Execute(void * pParam)
{
	HANDLE hIOCP = (HANDLE)pParam;

	while(1) {
		DWORD numberOfBytes;
		OVERLAPPED_EX *pOverlapped;
		ULONG completionKey;
		BOOL bResult = ::GetQueuedCompletionStatus(hIOCP, &numberOfBytes, &completionKey, &((LPOVERLAPPED)pOverlapped), INFINITE);
		
		if(pOverlapped && completionKey) {
			IIOMutableOperation *pOperation = (IIOMutableOperation *)completionKey;
			if( bResult && numberOfBytes ) {
				//pOperation->SetTransferredBytes(numberOfBytes);
				/*if( pOperation->HasMoreToDo() )
					pOperation->DoMore();*/
				pOperation->Process(numberOfBytes);
			}


		} else break;
	}
}