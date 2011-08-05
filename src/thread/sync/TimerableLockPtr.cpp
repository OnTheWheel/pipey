#include "TimerableLockPtr.h"

using namespace pipey::common;
using namespace pipey::common::exception;
using namespace pipey::thread::sync;

CTimerableLockPtr::CTimerableLockPtr(ITimerableLock * pLock) :
CLockPtr(pLock),
m_pLock(pLock)
{

}

CTimerableLockPtr::~CTimerableLockPtr()
{

}

SYNC_RESULT CTimerableLockPtr::AcquireTimedLock(unsigned long nMilliSeconds) 
{
	if( IsValid() ) {
		if( !IsLocked() ) {
			SYNC_RESULT result = m_pLock->AcquireTimedLock(nMilliSeconds);
			if( result != SYNC_TIMEOUT ) 
				SetLocked();
				
			return result;
		}
		else throw EInvalidState("EInvalidState => CTimerableLockPtr::AcquireTimedLock - This lock pointer is already locked.");
	}
	else throw EInvalidState("EInvalidState => CTimerableLockPtr::AcquireTimedLock - This lock pointer is not properly initiated.");
}

