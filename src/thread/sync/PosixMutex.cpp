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

	pthread_mutexattr_t attr;
	if( pthread_mutexattr_init(&attr) != 0 ) 
		throw ESync("ESync => CPosixMutex::Init - pthread_mutexattr_init failed.");

	if( pParam ) {
		const POSIX_MUTEX_INIT *pInit = dynamic_cast<const POSIX_MUTEX_INIT *>(pParam);
		if( pInit ) {
#ifdef _POSIX_THREAD_PROCESS_SHARED
			int shared = pInit->bProcessShared ? PTHREAD_PROCESS_SHARED : PTHREAD_PROCESS_PRIVATE;
			pthread_mutexattr_setpshared(&attr, shared);
#endif
			pthread_mutexattr_settype(&attr, pInit->nType);
		} else throw EInvalidParameter("EInvalidParameter => CPosixMutex::Init - pParam should be type of (POSIX_MUTEX_INIT *).");
	} else {
#ifdef _POSIX_THREAD_PROCESS_SHARED
		pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_PRIVATE);
#endif		
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	}

	int err = pthread_mutex_init(&m_hMutex, &attr);
	pthread_mutexattr_destroy(&attr);

	if( err == 0 )
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
			throw ESync("ESync => CPosixMutex::AcquireLock - deadlock");
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
			throw ESync("ESync => CPosixMutex::ReleaseLock - The calling thread does not own the mutex.");
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
			throw ESync("ESync => CPosixMutex::AcquireLock - deadlock");
		else throw ESync("ESync => CPosixMutex::AcquireTimedLock - unknown exception");
	}
	else throw EInvalidState("EInvalidState => CPosixMutex::AcquireTimedLock - This mutex is not properly initiated.");
}

pthread_mutex_t *CPosixMutex::GetHandle()
{	if( m_bInited )	{
		return &m_hMutex;
	}
	else throw EInvalidState("EInvalidState => CPosixMutex::GetHandle - This mutex is not properly initiated.");

}
