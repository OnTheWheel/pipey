#include "WindowsAsyncSocket.h"

using namespace pipey::io;
using namespace pipey::common::exception;

CWindowsAsyncSocket::CWindowsAsyncSocket()
{
}

CWindowsAsyncSocket::CWindowsAsyncSocket(IServerSocket *pParent, HANDLE hSocket)
: IBaseTarget(pParent, hSocket)
{
}

CWindowsAsyncSocket::~CWindowsAsyncSocket()
{
	SAFE_TRY(Close());
}

descriptor_t CWindowsAsyncSocket::Open(const IO_TARGET_INIT *param)
{
	return 0;
}

void CWindowsAsyncSocket::Close()
{
	if(IsValid()) {
		if(GetParent()) 
			((IServerSocket *)GetParent())->DeleteChild(GetDescriptor());
		
	} else throw EInvalidParameter("EInvalidState => CWindowsAsyncSocket::Close - this socket is not properly initiated.");
}
