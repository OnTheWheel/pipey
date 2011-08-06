#include "TriableLockPtr.h"

using namespace pipey::common;
using namespace pipey::common::exception;
using namespace pipey::thread::sync;

CTriableLockPtr::CTriableLockPtr(ITriableLock * pLock) :
CLockPtr(pLock),
m_pLock(pLock)
{

}

CTriableLockPtr::~CTriableLockPtr()
{

}

SYNC_RESULT CTriableLockPtr::TryLock() 
{
	if( IsValid() )	{
		if( !IsLocked() ) {
			SYNC_RESULT result = m_pLock->TryLock();
			if( result == SYNC_SUCCESS || result == SYNC_ABANDONED ) 
				SetLocked();
			
			return result;
		}
		else throw EInvalidState("EInvalidState => CTriableLockPtr::TryLock - This lock pointer is already locked.");
	}
	else throw EInvalidState("EInvalidState => CTriableLockPtr::TryLock - This lock pointer is not properly initiated.");
}

