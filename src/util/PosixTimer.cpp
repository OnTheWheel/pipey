#include "PosixTimer.h"
#include "PosixTimeHelper.h"

using namespace pipey::util;

CPosixTimer::CPosixTimer() :
m_bStarted(false),
m_nPrev(0),
m_nStart(0)
{

}

CPosixTimer::~CPosixTimer()
{

}

bool CPosixTimer::Start(unsigned long * pCurrentMilliseconds) 
{
	if( ! m_bStarted ) {
		m_nStart = m_nPrev = GetTimeMilliSeconds();
		if( pCurrentMilliseconds ) *pCurrentMilliseconds = m_nPrev;

		return ( m_bStarted = true );
	}

	return false;
}

bool CPosixTimer::Check(unsigned long * pElapsedMilliseconds) 
{
	if( m_bStarted ) {
		unsigned long current = GetTimeMilliSeconds();;
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

bool CPosixTimer::CheckTotal(unsigned long * pTotalElapsedMilliseconds) 
{
	if( ! pTotalElapsedMilliseconds ) return false;
	
	if( m_bStarted ) {
		unsigned long current = GetTimeMilliSeconds();
		
		if( current < m_nStart )
			*pTotalElapsedMilliseconds = 0xFFFFFFFF - m_nStart + current;
		else *pTotalElapsedMilliseconds = current - m_nStart;
		
		return true;
	}

	return false;
}

