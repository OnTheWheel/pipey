#include "WindowsMutex.h"

using namespace pipey::common;
using namespace pipey::common::exception;
using namespace pipey::thread::sync;

CWindowsMutex::CWindowsMutex() :
m_hMutex(NULL),
m_bInited(false)
{

}

CWindowsMutex::~CWindowsMutex()
{
	SAFE_TRY( Close() );
}

void CWindowsMutex::Init(const LOCK_INIT * pParam) 
{
	if( m_bInited ) throw EInvalidState("EInvalidState => CWindowsMutex::Init - This mutex is already initiated.");

	LPCTSTR lpName = NULL;
	BOOL bInitialOwner = FALSE;

	__try {

		if( pParam ) {
			const WIN_MUTEX_INIT *pInit = dynamic_cast<const WIN_MUTEX_INIT *>(pParam);
			if( pInit )	{
				lpName = pInit->lpName;
				bInitialOwner = pInit->bInitialOwner;
			}
			else throw EInvalidParameter("EInvalidParameter => CWindowsMutex::Init - pParam should be type of (WIN_MUTEX_INIT *).");
		}
	
		if( m_hMutex = ::CreateMutex(NULL, bInitialOwner, lpName) )
			m_bInited = true;
		else throw ESync("ESync => CWindowsMutex::Init - unknown exception");
	}
	__except(EXCEPTION_EXECUTE_HANDLER ) {
		throw ESync("ESync => CWindowsMutex::Init - unknown exception");
	}
}

SYNC_RESULT CWindowsMutex::AcquireLock() 
{
	return AcquireTimedLock(TIME_INFINITE);
}

void CWindowsMutex::ReleaseLock() 
{
	if( m_bInited )	{
		__try {
			if( ! ::ReleaseMutex(m_hMutex) )
				throw ESync("ESync => CWindowsMutex::ReleaseLock - unknown exception");
		}
		__except(EXCEPTION_EXECUTE_HANDLER ) {
			throw ESync("ESync => CWindowsMutex::ReleaseLock - unknown exception");
		}
	}
	else throw EInvalidState("EInvalidState => CWindowsMutex::AcquireTimedLock - This mutex is not properly initiated.");
}

void CWindowsMutex::Close() {

	if( m_bInited )	{
		__try {
			if( ::CloseHandle(m_hMutex) ) {
				m_bInited = false;
			}
			else throw ESync("ESync => CWindowsMutex::Close - unknown exception");
		}
		__except(EXCEPTION_EXECUTE_HANDLER ) {
			throw ESync("ESync => CWindowsMutex::Close - unknown exception");
		}
	}
	else throw EInvalidState("EInvalidState => CWindowsMutex::Close - This mutex is not properly initiated.");
}

SYNC_RESULT CWindowsMutex::TryLock() 
{
	SYNC_RESULT res = AcquireTimedLock(0);
	if( res == SYNC_TIMEOUT ) return SYNC_BUSY;
	else return res;
}

SYNC_RESULT CWindowsMutex::AcquireTimedLock(unsigned long nMilliSeconds) 
{
	if( m_bInited ) {
		__try {
			DWORD dwResult = ::WaitForSingleObject(m_hMutex, nMilliSeconds);

			if( dwResult == WAIT_OBJECT_0 )
				return SYNC_SUCCESS;
			else if( dwResult == WAIT_TIMEOUT )
				return SYNC_TIMEOUT;
			else if( dwResult == WAIT_ABANDONED )
				return SYNC_ABANDONED;
			else throw ESync("ESync => CWindowsMutex::AcquireTimedLock - unknown exception");
		}
		__except(EXCEPTION_EXECUTE_HANDLER ) {
			throw ESync("ESync => CWindowsMutex::AcquireTimedLock - unknown exception");
		}
	}
	else throw EInvalidState("EInvalidState => CWindowsMutex::AcquireTimedLock - This mutex is not properly initiated.");
}

