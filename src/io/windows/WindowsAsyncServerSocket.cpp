#include "WindowsAsyncServerSocket.h"

using namespace pipey::io;
using namespace pipey::common::exception;
using namespace std;

CWindowsAsyncServerSocket::CWindowsAsyncServerSocket()
{
}

CWindowsAsyncServerSocket::~CWindowsAsyncServerSocket()
{
	SAFE_TRY(Close());
}

descriptor_t CWindowsAsyncServerSocket::Open(const IO_TARGET_INIT *param)
{
	return 0;
}

void CWindowsAsyncServerSocket::DeleteChild(descriptor_t descriptor)
{
	map<descriptor_t, IIOTarget *>::iterator i = m_children.find(descriptor);
	if(i == m_children.end())
		throw EInvalidParameter("EInvalidParameter => CWindowsAsyncServerSocket::DeleteChild - the child soket doesn't exist.");
	else {
		i->second->NullifyParent();
		m_children.erase(descriptor);
	}
}

void CWindowsAsyncServerSocket::Close()
{
	if(IsValid()) {
		map<descriptor_t, IIOTarget *>::iterator i = m_children.begin();

		while(i != m_children.end()){
			i->second->NullifyParent();
			delete i->second;
			i++;
		}

		m_children.clear();
	} else throw EInvalidParameter("EInvalidState => CWindowsAsyncServerSocket::Close - this socket is not properly initiated.");
}
