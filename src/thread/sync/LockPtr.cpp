
#include "LockPtr.h"

using namespace pipey::common;
using namespace pipey::common::exception;
using namespace pipey::thread::sync;

CLockPtr::CLockPtr(ILock * pLock) :
m_pLock(pLock),
m_bLocked(false)
{

}

CLockPtr::~CLockPtr()
{
	SAFE_TRY( Close() );
}

SYNC_RESULT CLockPtr::AcquireLock() 
{
	if( IsValid() )	{
		if( !IsLocked() ) {
			SYNC_RESULT result = m_pLock->AcquireLock();
			if( result == SYNC_SUCCESS || result == SYNC_ABANDONED ) 
				SetLocked();
				
			return result;
		}
		else throw EInvalidState("EInvalidState => CLockPtr::AcquireLock - This lock pointer is already locked.");
	}
	else throw EInvalidState("EInvalidState => CLockPtr::AcquireLock - This lock pointer is not properly initiated.");
}

void CLockPtr::ReleaseLock() 
{
	if( IsValid() )	{
		if( IsLocked() ) {
			m_pLock->ReleaseLock();
			m_bLocked = false;
		}
		else throw EInvalidState("EInvalidState => CLockPtr::ReleaseLock - This lock pointer is not locked.");
	}
	else throw EInvalidState("EInvalidState => CLockPtr::ReleaseLock - This lock pointer is not properly initiated.");
}

bool CLockPtr::IsValid() const 
{
	return ( m_pLock != NULL );
}

CLockPtr::operator bool() const 
{
	return IsValid();
}

bool CLockPtr::IsLocked() const 
{
	return m_bLocked;
}

void CLockPtr::Close() 
{
	if( IsValid() )	{
		if( IsLocked() ) {
			ReleaseLock();
			m_bLocked = false;
			m_pLock = NULL;
		}
	}
	else throw EInvalidState("EInvalidState => CLockPtr::Close - This lock pointer is not properly initiated.");
}

void CLockPtr::SetLocked() 
{
	if( IsValid() ) {
		if( !IsLocked() )
			m_bLocked = true;
		else throw EInvalidState("EInvalidState => CLockPtr::SetLocked - This lock pointer is already locked.");
	}
	else throw EInvalidState("EInvalidState => CLockPtr::SetLocked - This lock pointer is not properly initiated.");
}

