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
m_hThread(NULL)
{

}

CWindowsThread::~CWindowsThread(void ){

	SAFE_TRY( Close() );
}

void CWindowsThread::Init(const THREAD_INIT & rInit) {

	if( m_hThread ) throw EInvalidState("EInvalidState => CWindowsThread::Init - This thread is already initiated.");

	if( rInit.pExec )
	{
		m_param = rInit;
		DWORD dwId;
		__try
		{
			m_hThread = ::CreateThread(NULL, 0, WindowsThreadFunc, &m_param, 0, &dwId);
			if( m_hThread ) return;
			else throw ESync("ESync => CWindowsThread::Init - unknown exception");
		}
		__except(EXCEPTION_EXECUTE_HANDLER )
		{
			throw ESync("ESync => CWindowsThread::Init - unknown exception");
		}
	}
	else throw EInvalidParameter("EInvalidParameter => CWindowsThread::Init - THREAD_INIT::pExec is NULL.");
}

SYNC_RESULT CWindowsThread::Wait(unsigned long nMilliSeconds) {

	if( m_hThread )
	{
		__try
		{
			DWORD res = ::WaitForSingleObject(m_hThread, nMilliSeconds);

			if( res == WAIT_OBJECT_0) return SYNC_SUCCESS;
			else if( res == WAIT_TIMEOUT ) return SYNC_TIMEOUT;
			else throw ESync("ESync => CWindowsThread::Wait - unknown exception");
		}
		__except(EXCEPTION_EXECUTE_HANDLER )
		{
			throw ESync("ESync => CWindowsThread::Wait - unknown exception");
		}
	}
	else throw EInvalidState("EInvalidState => CWindowsThread::Wait - This thread is not properly initiated.");
}

void CWindowsThread::Close() {

	if( m_hThread )
	{
		__try
		{
			if( ::CloseHandle(m_hThread) )
			{
				m_hThread = NULL;
				return;
			}
			else throw ESync("ESync => CWindowsThread::Close - unknown exception");
		}
		__except(EXCEPTION_EXECUTE_HANDLER )
		{
			throw ESync("ESync => CWindowsThread::Close - unknown exception");
		}
	}
	else throw EInvalidState("EInvalidState => CWindowsThread::Close - This thread is not properly initiated.");
}

