#include "WindowsThread.h"

using namespace pipey::common;
using namespace pipey::common::exception;
using namespace pipey::thread;

DWORD WINAPI WindowsThreadFunc(LPVOID lpParam)
{
	THREAD_INIT *pInit = (THREAD_INIT *)lpParam;
	pInit->pExec->Execute(pInit->pParam);

	return 0;
}

CWindowsThread::CWindowsThread(void ) :
m_hThread(NULL),
m_bDetached(false),
m_bInited(false)
{

}

CWindowsThread::~CWindowsThread(void ){

	SAFE_TRY( Detach() );
}

void CWindowsThread::Init(const THREAD_INIT & rInit) {

	if( m_bInited ) throw EInvalidState("EInvalidState => CWindowsThread::Init - This thread is already initiated.");

	if( rInit.pExec ) {
		m_param = rInit;
		DWORD dwId;
		__try {
			m_hThread = ::CreateThread(NULL, 0, WindowsThreadFunc, &m_param, 0, &dwId);
			if( m_hThread ) {
				m_bInited = true;
				return;
			}
			else throw ESync("ESync => CWindowsThread::Init - unknown exception");
		}
		__except(EXCEPTION_EXECUTE_HANDLER ) {
			throw ESync("ESync => CWindowsThread::Init - unknown exception");
		}
	}
	else throw EInvalidParameter("EInvalidParameter => CWindowsThread::Init - THREAD_INIT::pExec is NULL.");
}

void CWindowsThread::Detach()
{
	if( m_bInited && !m_bDetached ) {
		__try {
			if( ::CloseHandle(m_hThread) ) {
				m_hThread = NULL;
				m_bDetached = true;
				return;
			}
			else throw ESync("ESync => CWindowsThread::Detach - unknown exception");
		}
		__except(EXCEPTION_EXECUTE_HANDLER ) {
			throw ESync("ESync => CWindowsThread::Detach - unknown exception");
		}
	}
	else throw EInvalidState("EInvalidState => CWindowsThread::Detach - This thread is already detached or not properly initiated.");
}

SYNC_RESULT CWindowsThread::Wait(unsigned long nMilliSeconds) {

	if( m_bInited && !m_bDetached) {
		__try {
			DWORD res = ::WaitForSingleObject(m_hThread, nMilliSeconds);

			if( res == WAIT_OBJECT_0) return SYNC_SUCCESS;
			else if( res == WAIT_TIMEOUT ) return SYNC_TIMEOUT;
			else throw ESync("ESync => CWindowsThread::Wait - unknown exception");
		}
		__except(EXCEPTION_EXECUTE_HANDLER ) {
			throw ESync("ESync => CWindowsThread::Wait - unknown exception");
		}
	}
	else throw EInvalidState("EInvalidState => CWindowsThread::Wait - This thread is already detached or not properly initiated.");
}
