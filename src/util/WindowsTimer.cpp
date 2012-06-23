#include "WindowsTimer.h"

using namespace pipey::util;

CWindowsTimer::CWindowsTimer()
{
	m_bStarted = false;
	m_bSupportHighFreq = ( QueryPerformanceFrequency(&m_liFreq) != 0 );
	m_nPrev = m_nStart = 0;
}

CWindowsTimer::~CWindowsTimer()
{

}

bool CWindowsTimer::Start() 
{
	if( m_bSupportHighFreq )
		return StartHighFrequency();
	else if( ! m_bStarted ) {
		m_nStart = m_nPrev = GetTickCount();

		return ( m_bStarted = true );
	}

	return false;
}

bool CWindowsTimer::Check(uint32_t * pElapsedMilliseconds) 
{
	if( m_bSupportHighFreq ) {
		if( pElapsedMilliseconds ) {
			LARGE_INTEGER elapsed;
			if( CheckHighFrequency(&elapsed) ) {
				*pElapsedMilliseconds = (uint32_t) ( ( (double)elapsed.QuadPart / (double)m_liFreq.QuadPart ) * 1000 );
				return true;
			}
			else return false;
		}
		else return CheckHighFrequency();
	}
	else if( m_bStarted ) {
		uint32_t current = GetTickCount();
		if( pElapsedMilliseconds ) {
			if( current < m_nPrev )
				*pElapsedMilliseconds = 0xFFFFFFFF - m_nPrev + current;
			else *pElapsedMilliseconds = current - m_nPrev;
		}
		m_nPrev = current;
		return true;
	}

	return false;
}

bool CWindowsTimer::StartHighFrequency() 
{
	if( !m_bSupportHighFreq || m_bStarted ) return false;

	QueryPerformanceCounter(&m_liStart);
	m_liPrev = m_liStart;

	return ( m_bStarted = true );
}

bool CWindowsTimer::CheckHighFrequency(LARGE_INTEGER * pElapsed) 
{
	if( m_bSupportHighFreq && m_bStarted ) {
		if( pElapsed ) {
			LARGE_INTEGER current;
			QueryPerformanceCounter(&current);
			pElapsed->QuadPart = current.QuadPart - m_liPrev.QuadPart;
			m_liPrev = current;
		}
		else QueryPerformanceCounter(&m_liPrev);

		return true;
	}

	return false;
}

bool CWindowsTimer::CheckTotalHighFrequency(LARGE_INTEGER * pTotal) 
{
	if( m_bSupportHighFreq && m_bStarted && pTotal) {
		LARGE_INTEGER current;
		QueryPerformanceCounter(&current);
		pTotal->QuadPart = current.QuadPart - m_liStart.QuadPart;
		
		return true;
	}

	return false;
}

bool CWindowsTimer::CheckTotal(uint32_t * pTotalElapsedMilliseconds) 
{
	if( ! pTotalElapsedMilliseconds ) return false;

	if( m_bSupportHighFreq ) {
		LARGE_INTEGER total;
		if( CheckTotalHighFrequency(&total) ) {
			*pTotalElapsedMilliseconds = (uint32_t) ( ( (double)total.QuadPart / (double)m_liFreq.QuadPart ) * 1000 );
			return true;
		}
		else return false;
	}
	else if( m_bStarted ) {
		uint32_t current = GetTickCount();
		
		if( current < m_nStart )
			*pTotalElapsedMilliseconds = 0xFFFFFFFF - m_nStart + current;
		else *pTotalElapsedMilliseconds = current - m_nStart;
		
		return true;
	}

	return false;
}

