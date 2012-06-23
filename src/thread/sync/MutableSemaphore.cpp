#include "MutableSemaphore.h"
#include "LockPtr.h"
#include "TimerableLockPtr.h"
#include "TriableLockPtr.h"

using namespace pipey::common;
using namespace pipey::common::exception;
using namespace pipey::thread::sync;

CMutableSemaphore::CMutableSemaphore() :
m_bInited(false),
m_nMaxLimit(0),
m_nValue(0),
m_nOwner(0)
{

}

CMutableSemaphore::~CMutableSemaphore()
{
	SAFE_TRY( Close() );
}

void CMutableSemaphore::Init(const LOCK_INIT * pParam) {

	if( m_bInited ) throw EInvalidState("EInvalidState => CMutableSemaphore::Init - This semaphore is already initiated.");

	try
	{
		if( pParam )
		{
			const MUT_SEM_INIT *pInit = dynamic_cast<const MUT_SEM_INIT *>(pParam);
			if( pInit )
			{
				if( pInit->nMaxLimit > 0 && pInit->nMaxLimit >= pInit->nInitialValue )
				{
					m_nMaxLimit = pInit->nMaxLimit;
					m_nValue = pInit->nInitialValue;
				}
				else throw EInvalidParameter("EInvalidParameter => CMutableSemaphore::Init - You should meet the following condition (nMaxLimit > 0 && nMaxLimit >= nInitialValue).");
			}
			else throw EInvalidParameter("EInvalidParameter => CMutableSemaphore::Init - pParam should be type of (MUT_SEM_INIT *).");
		}
		else throw EInvalidParameter("EInvalidParameter => CMutableSemaphore::Init - pParam is NULL.");

		m_lock.Init();

#if defined(WIN32) || defined(WIN64)
		m_condition.Init();
#elif defined(__linux__) || defined(__unix__)

#ifdef _POSIX_THREAD_PROCESS_SHARED
		m_condition.Init(&POSIX_COND_INIT(false, &m_lock));	
#else
		m_condition.Init(&POSIX_COND_INIT(&m_lock));	
#endif

#endif

		m_bInited = true;
	}
	catch(...)
	{
		m_nMaxLimit = m_nValue = 0;
		SAFE_TRY( m_lock.Close() );
		SAFE_TRY( m_condition.Close() );
		throw;
	}
}

SYNC_RESULT CMutableSemaphore::AcquireLock() {

	if( ! m_bInited ) throw EInvalidState("EInvalidState => CMutableSemaphore::AcquireLock - This semaphore is not properly initiated.");

	bool bChanged = false;
	try
	{
		CLockPtr lockPtr(&m_lock);
		lockPtr.AcquireLock();

		while( m_nValue >= m_nMaxLimit )
		{
#if defined(WIN32) || defined(WIN64)
			lockPtr.ReleaseLock();
#endif
			m_condition.Wait();

#if defined(WIN32) || defined(WIN64)
			lockPtr.AcquireLock();
#endif
		}

		m_nValue++;
		m_nOwner++;
		bChanged = true;

		if( m_nValue < m_nMaxLimit )
			m_condition.Awake();

		return SYNC_SUCCESS;
	}
	catch(...)
	{
		if( bChanged )
		{
			m_nValue--;
			m_nOwner--;
		}
		throw;
	}

}

void CMutableSemaphore::ReleaseLock() {

	ReleaseMultiple(1);
}

void CMutableSemaphore::Close() {

	if( m_bInited )
	{
		m_lock.Close();
		try
		{
			m_condition.Close();
		}
		catch(...)
		{
			m_lock.Init();
		}

		m_bInited = false;
		m_nMaxLimit = m_nValue = m_nOwner = 0;
	}
	else throw EInvalidState("EInvalidState => CMutableSemaphore::Close - This semaphore is not properly initiated.");
}

SYNC_RESULT CMutableSemaphore::TryLock() {

	if( ! m_bInited ) throw EInvalidState("EInvalidState => CMutableSemaphore::TryLock - This semaphore is not properly initiated.");

	bool bChanged = false;
	try
	{
		CTriableLockPtr lockPtr(&m_lock);
		if( lockPtr.TryLock() == SYNC_SUCCESS )
		{
			if( m_nValue >= m_nMaxLimit )
				return SYNC_BUSY;
			
			m_nValue++;
			m_nOwner++;
			bChanged = true;

			if( m_nValue < m_nMaxLimit )
				m_condition.Awake();

			return SYNC_SUCCESS;
		}
		else return SYNC_BUSY;
	}
	catch(...)
	{
		if( bChanged )
		{
			m_nValue--;
			m_nOwner--;
		}
		throw;
	}
}

#include "../../util/DefaultTimer.h"
using namespace pipey::util;

SYNC_RESULT CMutableSemaphore::AcquireTimedLock(uint32_t nMilliSeconds) {

	bool bChanged = false;
	try
	{
		CDefaultTimer timer;
		timer.Start();

		if( nMilliSeconds == TIME_INFINITE ) return AcquireLock();

		if( ! m_bInited ) throw EInvalidState("EInvalidState => CMutableSemaphore::AcquireTimedLock - This semaphore is not properly initiated.");

		uint32_t totalElapsed = 0;

		SYNC_RESULT res;

		CTimerableLockPtr lockPtr(&m_lock);
		if( ( res = lockPtr.AcquireTimedLock(nMilliSeconds + 1) ) == SYNC_SUCCESS )
		{
			while( m_nValue >= m_nMaxLimit )
			{
#if defined(WIN32) || defined(WIN64)
				lockPtr.ReleaseLock();
#endif

				timer.CheckTotal(&totalElapsed);
				res =  totalElapsed > nMilliSeconds ? SYNC_TIMEOUT : m_condition.Wait(nMilliSeconds - totalElapsed + 1);
				if( res == SYNC_TIMEOUT )
					return res;

#if defined(WIN32) || defined(WIN64)
				timer.CheckTotal(&totalElapsed);
				res = totalElapsed > nMilliSeconds ? SYNC_TIMEOUT : lockPtr.AcquireTimedLock(nMilliSeconds - totalElapsed + 1);
				if( res == SYNC_TIMEOUT )
					return res;
#endif
			}

			m_nValue++;
			m_nOwner++;
			bChanged = true;

			if( m_nValue < m_nMaxLimit )
				m_condition.Awake();
			
			return SYNC_SUCCESS;
		}
		else return SYNC_TIMEOUT;
	}
	catch(...)
	{
		if( bChanged )
		{
			m_nValue--;
			m_nOwner--;
		}
		throw;
	}

}

void CMutableSemaphore::ReleaseMultiple(uint32_t nCount) {

	if( !m_bInited ) throw EInvalidState("EInvalidState => CMutableSemaphore::ReleaseMultiple - This semaphore is not properly initiated.");
	if( nCount == 0 ) throw EInvalidParameter("EInvalidParameter => CMutableSemaphore::ReleaseMultiple - nCount cannot be zero.");

	bool bChanged = false;
	uint32_t prevOwner = 0;
	try
	{
		CLockPtr lockPtr(&m_lock);
		lockPtr.AcquireLock();

		if( m_nValue < nCount  )
			throw EInvalidParameter("EInvalidParameter => CMutableSemaphore::ReleaseMultiple - nCount cannot exceed current semaphore value.");

		m_nValue -= nCount;
		prevOwner = m_nOwner;
		if( m_nOwner > nCount ) m_nOwner -= nCount;
		else m_nOwner = 0;
		bChanged = true;

		if( m_nValue < m_nMaxLimit )
			m_condition.Awake();
	}
	catch(...)
	{
		if( bChanged )
		{
			m_nValue += nCount;
			m_nOwner = prevOwner;
		}

		throw;
	}
}

void CMutableSemaphore::SetMaxLimit(uint32_t nMaxLimit) {

	if(nMaxLimit == 0) throw EInvalidParameter("EInvalidParameter => CMutableSemaphore::SetMaxLimit - nMaxLimit cannot be zero.");

	if ( m_bInited )
	{
		CLockPtr lockPtr(&m_lock);
		lockPtr.AcquireLock();

		if( nMaxLimit > m_nMaxLimit )
		{
			uint32_t prevMax = m_nMaxLimit;
			m_nMaxLimit = nMaxLimit;

			if( m_nValue < m_nMaxLimit )
			{
				try
				{
					m_condition.Awake();
				}
				catch(...)
				{
					m_nMaxLimit = prevMax;
					throw;
				}
			}
		}
		else if( nMaxLimit < m_nMaxLimit )
		{
			if( m_nValue - m_nOwner >= nMaxLimit )
				m_nValue = nMaxLimit + m_nOwner - 1;
			m_nMaxLimit = nMaxLimit;
		}
	}
	else
	{
		m_nMaxLimit = nMaxLimit;
		if( m_nValue >= m_nMaxLimit )
			m_nValue = m_nMaxLimit;
	}
}

uint32_t CMutableSemaphore::GetMaxLimit() const {

	return m_nMaxLimit;
}

uint32_t CMutableSemaphore::GetCurrentValue() const {

	return m_nValue;
}

uint32_t CMutableSemaphore::GetCurrentOwner() const {

	return m_nOwner;
}

