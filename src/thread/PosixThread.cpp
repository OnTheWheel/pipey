#include "PosixThread.h"

using namespace pipey::common;
using namespace pipey::common::exception;
using namespace pipey::thread;

void *PosixThreadFunc(void *pParam)
{
	THREAD_INIT *pInit = (THREAD_INIT *)pParam;
	pInit->pExec->Execute(pInit->pParam);

	return 0;
}

CPosixThread::CPosixThread(void ) :
m_bInited(false),
m_bDetached(false)
{

}

CPosixThread::~CPosixThread(void )
{
	SAFE_TRY( Detach() );
}

void CPosixThread::Init(const THREAD_INIT & rInit) 
{
	if( m_bInited ) throw EInvalidState("EInvalidState => CPosixThread::Init - This thread is already initiated.");

	if( rInit.pExec ) {
		m_param = rInit;
		int32_t err = pthread_create(&m_hThread, NULL, PosixThreadFunc, &m_param);
		if( err == 0 ) {
		    m_bInited = true;
		    return;
		}
		else throw ESync("ESync => CPosixThread::Init - unknown exception");
	}
	else throw EInvalidParameter("EInvalidParameter => CPosixThread::Init - THREAD_INIT::pExec is NULL.");
}

void CPosixThread::Detach()
{
	if( m_bInited && !m_bDetached) {
		if( pthread_detach(m_hThread ) == 0 ) {
			m_bDetached = true;
			return;
		}
		else throw ESync("ESync => CPosixThread::Detach - unknown exception");
    }
    else throw EInvalidState("EInvalidState => CPosixThread::Detach - This thread is not properly initiated.");
}

SYNC_RESULT CPosixThread::Wait() 
{
	if( m_bInited && !m_bDetached)	{
		int32_t err = pthread_join(m_hThread, NULL);

		if( err == 0 ) {
		    m_bDetached = true;
		    return SYNC_SUCCESS;
		}
		else throw ESync("ESync => CPosixThread::Wait - unknown exception");
	}
	else throw EInvalidState("EInvalidState => CPosixThread::Wait - This thread is not properly initiated.");
}

