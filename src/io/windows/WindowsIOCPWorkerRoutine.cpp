#include "WindowsIOCPWorkerRoutine.h"
#include "../IOTarget.h"
#include "../IOOperator.h"
using namespace pipey::io;

void CWindowsIOCPWorkerRoutine::Execute(void * pParam)
{
	HANDLE hIOCP = (HANDLE)pParam;

	while(1) {
		DWORD numberOfBytes;
		OVERLAPPED *pOverlapped;
		ULONG completionKey;
		BOOL bResult = ::GetQueuedCompletionStatus(hIOCP, &numberOfBytes, &completionKey, &pOverlapped, INFINITE);
		
		if(pOverlapped && completionKey) {
			IIOMutableOperation *pOperation = (IIOMutableOperation *)completionKey;
			if( bResult && numberOfBytes ) {
				pOperation->SetTransferredBytes(numberOfBytes);
				if( pOperation->HasMoreToDo() )
					pOperation->DoMore();
			}

		} else break;
	}
}