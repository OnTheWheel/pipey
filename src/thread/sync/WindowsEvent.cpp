#include "WindowsEvent.h"

using namespace pipey::common;
using namespace pipey::common::exception;
using namespace pipey::thread::sync;

CWindowsEvent::CWindowsEvent(void ) :
m_hEvent(NULL),
m_bAutoReset(true),
m_bInited(false)
{

}

CWindowsEvent::~CWindowsEvent(void ){

	SAFE_TRY( Close() );
}

void CWindowsEvent::Init(const CONDITION_INIT * pParam) {

	if( ! m_bInited )
	{
		BOOL bManualReset;
		BOOL bInitialState;
		LPCTSTR lpName;

		if( pParam == NULL )
		{
			bManualReset = FALSE;
			m_bAutoReset = true;
			bInitialState = FALSE;
			lpName = NULL;
		}
		else
		{
			const WIN_EVENT_INIT *pInit = dynamic_cast<const WIN_EVENT_INIT *>(pParam);
			if( pInit )
			{
				bManualReset = ! (pInit->bAutoReset);
				m_bAutoReset = pInit->bAutoReset;
				bInitialState = pInit->bInitialState;
				lpName = pInit->lpName;
			}
			else throw EInvalidParameter("EInvalidParameter => CWindowsEvent::Init - pParam should be type of (WIN_EVENT_INIT *).");
		}

		__try
		{
			if( m_hEvent = CreateEvent(NULL, bManualReset, bInitialState, lpName) )
				m_bInited = true;
			else throw ESync("ESync => CWindowsEvent::Init - unknown exception");
		}
		__except(EXCEPTION_EXECUTE_HANDLER )
		{
			throw ESync("ESync => CWindowsEvent::Init - unknown exception");
		}

	}
	else throw EInvalidState("EInvalidState => CWindowsEvent::Init - This event is already initiated.");
}

SYNC_RESULT CWindowsEvent::Wait(unsigned long nMilliSeconds) {

	if( m_bInited )
	{
		DWORD res = 0;
		__try
		{
			DWORD res = ::WaitForSingleObject(m_hEvent, nMilliSeconds);
		}
		__except(EXCEPTION_EXECUTE_HANDLER )
		{
			throw ESync("ESync => CWindowsEvent::Wait - unknown exception");
		}

		if( res == WAIT_OBJECT_0) return SYNC_SUCCESS;
		else if( res == WAIT_TIMEOUT ) return SYNC_TIMEOUT;
		else throw ESync("ESync => CWindowsEvent::Wait - unknown exception");
	}
	else throw EInvalidState("EInvalidState => CWindowsEvent::Wait - This event is not properly initiated.");
}

SYNC_RESULT CWindowsEvent::Test() {

	SYNC_RESULT res = Wait(0);

	if( res == SYNC_TIMEOUT )
		return SYNC_BUSY;
	else return res;
}

void CWindowsEvent::Awake() {

	if( m_bInited )
	{
		__try
		{
			if( ! SetEvent(m_hEvent) )
				throw ESync("ESync => CWindowsEvent::Awake - unknown exception");
		}
		__except(EXCEPTION_EXECUTE_HANDLER )
		{
			throw ESync("ESync => CWindowsEvent::Awake - unknown exception");
		}
	}
	else throw EInvalidState("EInvalidState => CWindowsEvent::Awake - This event is not properly initiated.");
}

void CWindowsEvent::Close() {

	if( m_bInited )
	{
		__try
		{
			if( CloseHandle(m_hEvent) )
				m_bInited = false;
			else throw ESync("ESync => CWindowsEvent::Close - unknown exception");
		}
		__except(EXCEPTION_EXECUTE_HANDLER )
		{
			throw ESync("ESync => CWindowsEvent::Close - unknown exception");
		}
	}
	else throw EInvalidState("EInvalidState => CWindowsEvent::Close - This event is not properly initiated.");
}

void CWindowsEvent::ResetEvent() {

	if( m_bInited )
	{
		__try
		{
			if( ! ::ResetEvent(m_hEvent) )
			throw ESync("ESync => CWindowsEvent::ResetEvent - unknown exception");
		}
		__except(EXCEPTION_EXECUTE_HANDLER )
		{
			throw ESync("ESync => CWindowsEvent::ResetEvent - unknown exception");
		}
	}
	else throw EInvalidState("EInvalidState => CWindowsEvent::ResetEvent - This event is not properly initiated.");
}

