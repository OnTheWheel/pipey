#include "PosixCondition.h"
#include <cerrno>

using namespace pipey::common;
using namespace pipey::common::exception;
using namespace pipey::thread::sync;

CPosixCondition::CPosixCondition(void ) :
m_bInited(false),
m_pMutex(NULL),
m_bOwnMutex(false)
{

}

CPosixCondition::~CPosixCondition(void )
{
	SAFE_TRY( Close() );
}

void CPosixCondition::Init(const CONDITION_INIT * pParam) 
{
	if( ! m_bInited ) {
		pthread_condattr_t attr, * pAttr = NULL;

		if( pParam == NULL ) {
			m_bOwnMutex = true;
		}
		else {
			const POSIX_COND_INIT *pInit = dynamic_cast<const POSIX_COND_INIT *>(pParam);
			if( pInit ) {
#ifdef _POSIX_THREAD_PROCESS_SHARED
				if( pthread_condattr_init(&attr) == 0) {
					if( pInit->bProcessShared )
						pthread_condattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
					else pthread_condattr_setpshared(&attr, PTHREAD_PROCESS_PRIVATE);

					pAttr = &attr;
				}
				else throw ESync("ESync => CPosixCondition::Init - pthread_condattr_init failed."); 
#endif
				if( pInit->pMutex ) {
					m_bOwnMutex = false;
					m_pMutex = pInit->pMutex;
				}
				else m_bOwnMutex = true;
			}
			else throw EInvalidParameter("EInvalidParameter => CPosixCondition::Init - pParam should be type of (POSIX_COND_INIT *).");
		}

		try {
			if( m_bOwnMutex ) {
				m_pMutex = new CPosixMutex();
				m_pMutex->Init();
			}

			if( pthread_cond_init(&m_hCondition, pAttr) == 0 )
				m_bInited = true;
			else throw ESync("ESync => CPosixCondition::Init - unknown exception");
		}
		catch(...) {
			if( m_bOwnMutex && m_pMutex ) delete m_pMutex;
			throw;
		}

	}
	else throw EInvalidState("EInvalidState => CPosixCondition::Init - This condition variable is already initiated.");
}

#include "../../util/PosixTimeHelper.h"

SYNC_RESULT CPosixCondition::Wait(unsigned long nMilliSeconds) 
{
	if( m_bInited )	{
		if( m_bOwnMutex ) {
			if( m_pMutex->AcquireLock() != SYNC_SUCCESS )
				throw ESync("ESync => CPosixCondition::Wait - Cannot acquire the lock to protect the condition variable.");
		}
		
		int err;

		if ( nMilliSeconds == TIME_INFINITE )
			err = pthread_cond_wait(&m_hCondition, m_pMutex->GetHandle());
		else {
			timespec term;
			pipey::util::RelativeToAbsolute(nMilliSeconds, term);
			err = pthread_cond_timedwait(&m_hCondition, m_pMutex->GetHandle(), &term);
		}

		if( m_bOwnMutex ) m_pMutex->ReleaseLock();

		if( err == 0 ) return SYNC_SUCCESS;
		else if( err == ETIMEDOUT ) return SYNC_TIMEOUT;
		else {
			throw ESync("ESync => CPosixCondition::Wait - unknown exception");
		}
	}
	else throw EInvalidState("EInvalidState => CPosixCondition::Wait - This condition variable is not properly initiated.");
}

SYNC_RESULT CPosixCondition::Test() 
{
	SYNC_RESULT res = Wait(0);

	if( res == SYNC_TIMEOUT )
		return SYNC_BUSY;
	else return res;
}

void CPosixCondition::Awake() 
{
	if( m_bInited ) {
		if( pthread_cond_signal(&m_hCondition) != 0 )
			throw ESync("ESync => CPosixCondition::Awake - unknown exception");
	}
	else throw EInvalidState("EInvalidState => CPosixCondition::Awake - This condition variable is not properly initiated.");
}

void CPosixCondition::AwakeAll() 
{
	if( m_bInited ) {
		if( pthread_cond_broadcast(&m_hCondition) != 0 )
			throw ESync("ESync => CPosixCondition::AwakeAll - unknown exception");
	}
	else throw EInvalidState("EInvalidState => CPosixCondition::AwakeAll - This condition variable is not properly initiated.");
}
void CPosixCondition::Close() 
{
	if( m_bInited ) {

		int err = pthread_cond_destroy(&m_hCondition);
		if( err == 0 ) {
			m_bInited = false;
			if( m_bOwnMutex ) {
				delete m_pMutex;
				m_pMutex = NULL;
			}
		}
		else if( err == EBUSY)
			throw ESync("ESync => CPosixCondition::Close -some threads are currently waiting on the condition variable.");
		else throw ESync("ESync => CPosixCondition::Close - unknown exception");
	}
	else throw EInvalidState("EInvalidState => CPosixCondition::Close - This condition variable is not properly initiated.");
}
