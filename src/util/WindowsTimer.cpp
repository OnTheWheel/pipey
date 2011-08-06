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

bool CWindowsTimer::Start(unsigned long * pCurrentMilliseconds) 
{
	if( m_bSupportHighFreq ) {
		if( pCurrentMilliseconds ) {
			LARGE_INTEGER current;
			if( StartHighFrequency(&current) ) {
				*pCurrentMilliseconds = (unsigned long) ( ( (double)current.QuadPart / (double)m_liFreq.QuadPart ) * 1000 );
				return ( m_bStarted = true );
			}
			else return false;
		}
		else return StartHighFrequency();
	}
	else if( ! m_bStarted ) {
		m_nStart = m_nPrev = GetTickCount();
		if( pCurrentMilliseconds ) *pCurrentMilliseconds = m_nPrev;

		return ( m_bStarted = true );
	}

	return false;
}

bool CWindowsTimer::Check(unsigned long * pElapsedMilliseconds) 
{
	if( m_bSupportHighFreq ) {
		if( pElapsedMilliseconds ) {
			LARGE_INTEGER elapsed;
			if( CheckHighFrequency(&elapsed) ) {
				*pElapsedMilliseconds = (unsigned long) ( ( (double)elapsed.QuadPart / (double)m_liFreq.QuadPart ) * 1000 );
				return true;
			}
			else return false;
		}
		else return CheckHighFrequency();
	}
	else if( m_bStarted ) {
		unsigned long current = GetTickCount();
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

bool CWindowsTimer::StartHighFrequency(LARGE_INTEGER * pCurrent) 
{
	if( !m_bSupportHighFreq || m_bStarted ) return false;

	QueryPerformanceCounter(&m_liStart);
	if( pCurrent ) *pCurrent = m_liPrev;
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

bool CWindowsTimer::CheckTotal(unsigned long * pTotalElapsedMilliseconds) 
{
	if( ! pTotalElapsedMilliseconds ) return false;

	if( m_bSupportHighFreq ) {
		LARGE_INTEGER total;
		if( CheckTotalHighFrequency(&total) ) {
			*pTotalElapsedMilliseconds = (unsigned long) ( ( (double)total.QuadPart / (double)m_liFreq.QuadPart ) * 1000 );
			return true;
		}
		else return false;
	}
	else if( m_bStarted ) {
		unsigned long current = GetTickCount();
		
		if( current < m_nStart )
			*pTotalElapsedMilliseconds = 0xFFFFFFFF - m_nStart + current;
		else *pTotalElapsedMilliseconds = current - m_nStart;
		
		return true;
	}

	return false;
}

