#include "PipeyException.h"
#include <stdlib.h>

using namespace pipey::common::exception;

static const char * _PipeyExceptionMsg = "EPipeyException : undefined exception";


EPipeyException::EPipeyException(void) :
m_pMsg(_PipeyExceptionMsg),
m_bAllocatedMemory(false) 
{
}

EPipeyException::EPipeyException(const char * pMsg, bool bAllocatedMemory) :
m_pMsg(pMsg),
m_bAllocatedMemory(bAllocatedMemory) 
{
}

EPipeyException::~EPipeyException(void) throw()
{
	if( m_pMsg && m_bAllocatedMemory )
		free(static_cast<void *>(const_cast<char *>(m_pMsg)));
}

const char *EPipeyException::what() const throw()
{
	return m_pMsg;
}
