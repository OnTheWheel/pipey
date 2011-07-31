#include "WindowsCritricalSection.h"

using namespace pipey::common;
using namespace pipey::common::exception;
using namespace pipey::thread::sync;

CWindowsCritricalSection::CWindowsCritricalSection() :
m_bInited(false)
{

}

CWindowsCritricalSection::~CWindowsCritricalSection()
{
	SAFE_TRY( Close() );
}

void CWindowsCritricalSection::Init(const LOCK_INIT * pParam) 
{
	if( ! m_bInited ) {
		__try {
			InitializeCriticalSection(&m_cs);
			m_bInited = true;
		} 
		__except(EXCEPTION_EXECUTE_HANDLER ) {
			throw ESync("ESync => CWindowsCritricalSection::Init - unknown exception");
		}
	}
	else throw EInvalidState("EInvalidState => CWindowsCritricalSection::Init - This CritricalSection is already initiated.");
}

SYNC_RESULT CWindowsCritricalSection::AcquireLock() 
{
	if( m_bInited ) {
		__try {
			EnterCriticalSection(&m_cs);
			return SYNC_SUCCESS;
		}
		__except(EXCEPTION_EXECUTE_HANDLER ) {
			throw ESync("ESync => CWindowsCritricalSection::AcquireLock - unknown exception");
		}
	}
	else throw EInvalidState("EInvalidState => CWindowsCritricalSection::AcquireLock - This CritricalSection is not properly initiated.");
}

void CWindowsCritricalSection::ReleaseLock() {

	if( m_bInited ) {
		__try {
			LeaveCriticalSection(&m_cs);
		}
		__except(EXCEPTION_EXECUTE_HANDLER ) {
			throw ESync("ESync => CWindowsCritricalSection::ReleaseLock - unknown exception");
		}
	}
	else throw EInvalidState("EInvalidState => CWindowsCritricalSection::ReleaseLock - This CritricalSection is not properly initiated.");
}

void CWindowsCritricalSection::Close() {

	if( m_bInited ) {
		__try {
			DeleteCriticalSection(&m_cs);
			m_bInited = false;
		}
		__except(EXCEPTION_EXECUTE_HANDLER ) {
			throw ESync("ESync => CWindowsCritricalSection::Close - unknown exception");
		}
	}
	else throw EInvalidState("EInvalidState => CWindowsCritricalSection::Close - This CritricalSection is not properly initiated.");
}

#if defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0400)
SYNC_RESULT CWindowsCritricalSection::TryLock() 
{
	if( m_bInited ) {
		__try {
			BOOL bRes = TryEnterCriticalSection(&m_cs);
			if( bRes == FALSE ) return SYNC_FAIL;
			else return SYNC_SUCCESS;
		}
		__except(EXCEPTION_EXECUTE_HANDLER ) {
			throw ESync("ESync => CWindowsCritricalSection::TryLock - unknown exception");
		}
	}
	else throw EInvalidState("EInvalidState => CWindowsCritricalSection::ReleaseLock - This CritricalSection is not properly initiated.");
}
#endif
