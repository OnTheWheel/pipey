#include "PosixThread.h"

using namespace pipey::thread;

void *PosixThreadFunc(void *pParam)
{
	THREAD_INIT *pInit = (THREAD_INIT *)pParam;
	pInit->pExec->Execute(pInit->pParam);

	return 0;
}

CPosixThread::CPosixThread(void ) :
m_bInited(false)
{

}

CPosixThread::~CPosixThread(void ){

	SAFE_TRY( Close() );
}

void CPosixThread::Init(const THREAD_INIT & rInit) {

	if( m_bInited  ) throw EInvalidState("EInvalidState => CPosixThread::Init - This thread is already initiated.");

	if( rInit.pExec ) {
		m_param = rInit;
		int err = pthread_create(&m_hThread, NULL, PosixThreadFunc, NULL);
		if( err == 0 ) {
		    m_bInited = true;
		    return;
		}
		else throw ESync("ESync => CPosixThread::Init - unknown exception");
	}
	else throw EInvalidParameter("EInvalidParameter => CPosixThread::Init - THREAD_INIT::pExec is NULL.");
}

SYNC_RESULT CPosixThread::Wait(unsigned long nMilliSeconds) {

	if( m_bInited )	{
		int err = pthread_join(m_hThread, NULL);

		if( err == 0 ) return SYNC_SUCCESS;
		//else if( res == WAIT_TIMEOUT ) return SYNC_TIMEOUT;
		else throw ESync("ESync => CPosixThread::Wait - unknown exception");
	}
	else throw EInvalidState("EInvalidState => CPosixThread::Wait - This thread is not properly initiated.");
}

void CPosixThread::Close() {

    if( m_hThread ) {
	if( pthread_detach(m_hThread ) == 0 ) {
		m_bInited = false;
		return;
	}
	else throw ESync("ESync => CPosixThread::Close - unknown exception");
    }
    else throw EInvalidState("EInvalidState => CPosixThread::Close - This thread is not properly initiated.");
}

