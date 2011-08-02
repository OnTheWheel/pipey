#include "PosixMutex.h"
#include <cerrno>
using namespace pipey::common;
using namespace pipey::common::exception;
using namespace pipey::thread::sync;

CPosixMutex::CPosixMutex() :
m_bInited(false)
{

}

CPosixMutex::~CPosixMutex()
{
	SAFE_TRY( Close() );
}

void CPosixMutex::Init(const LOCK_INIT * pParam) 
{
	if( m_bInited ) throw EInvalidState("EInvalidState => CPosixMutex::Init - This mutex is already initiated.");

	if( pParam ) {
		const POSIX_MUTEX_INIT *pInit = dynamic_cast<const POSIX_MUTEX_INIT *>(pParam);

		if( pInit )	{
			//lpName = pInit->lpName;
			//bInitialOwner = pInit->bInitialOwner;
		}
		else throw EInvalidParameter("EInvalidParameter => CPosixMutex::Init - pParam should be type of (POSIX_MUTEX_INIT *).");
	}

	if( pthread_mutex_init(&m_hMutex, NULL) == 0 )
		m_bInited = true;
	else throw ESync("ESync => CPosixMutex::Init - unknown exception");
}
			
SYNC_RESULT CPosixMutex::AcquireLock() 
{
	if( m_bInited )	{
		int err =  pthread_mutex_lock(&m_hMutex);

		if( err == 0 )
			return SYNC_SUCCESS;
		else if( err == EDEADLK )
			return SYNC_DEADLOCK;
		else if (err ==  EINVAL )
			throw EInvalidState("EInvalidState => CPosixMutex::AcquireLock - This mutex is not properly initiated.");
		else throw ESync("ESync => CPosixMutex::AcquireLock - unknown exception");
	}
	else throw EInvalidState("EInvalidState => CPosixMutex::AcquireLock - This mutex is not properly initiated.");
	
}

void CPosixMutex::ReleaseLock() 
{
	if( m_bInited )	{
		int err = pthread_mutex_unlock(&m_hMutex);

		if( err == 0 ) return;
		else if( err == EINVAL )
			throw EInvalidState("EInvalidState => CPosixMutex::ReleaseLock - This mutex is not properly initiated.");
		else if( err == EPERM )
			throw ESync("ESync => CPosixMutex::ReleaseLock - The calling thread does not own the mutex (''error checking'' mutexes only).");
		else throw ESync("ESync => CPosixMutex::AcquireLock - unknown exception");
	}
	else throw EInvalidState("EInvalidState => CPosixMutex::ReleaseLock - This mutex is not properly initiated.");
}

void CPosixMutex::Close() {

	if( m_bInited )	{

		int err = pthread_mutex_destroy(&m_hMutex);
		if( err == 0 ) 
			m_bInited = false;
		else if( err == EBUSY ) throw ESync("ESync => CPosixMutex::Close - The mutex is currently locked.");
		else throw ESync("ESync => CPosixMutex::Close - unknown exception");
	}
	else throw EInvalidState("EInvalidState => CPosixMutex::Close - This mutex is not properly initiated.");
}

SYNC_RESULT CPosixMutex::TryLock() 
{
	if( m_bInited )	{
		int err = pthread_mutex_trylock(&m_hMutex);
		if( err == 0 ) 
			 return SYNC_SUCCESS;
		else if( err == EBUSY )
			return SYNC_BUSY;
		else throw ESync("ESync => CPosixMutex::TryLock - unknown exception");
	}
	else throw EInvalidState("EInvalidState => CPosixMutex::TryLock - This mutex is not properly initiated.");
}

#include "../../util/PosixTimeHelper.h"

SYNC_RESULT CPosixMutex::AcquireTimedLock(unsigned long nMilliSeconds) 
{
	if( m_bInited ) {

		if( nMilliSeconds == TIME_INFINITE )
			return AcquireLock();

		timespec term;
		pipey::util::RelativeToAbsolute(nMilliSeconds, term);
		int err = pthread_mutex_timedlock(&m_hMutex, &term);

		if( err == 0 )
			return SYNC_SUCCESS;
		else if( err== ETIMEDOUT )
			return SYNC_TIMEOUT;
		else if( err == EDEADLK )
			return SYNC_DEADLOCK;
		else throw ESync("ESync => CPosixMutex::AcquireTimedLock - unknown exception");
	}
	else throw EInvalidState("EInvalidState => CPosixMutex::AcquireTimedLock - This mutex is not properly initiated.");
}

