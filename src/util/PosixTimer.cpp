#include "PosixTimer.h"
#include "PosixTimeHelper.h"

using namespace pipey::util;

#ifndef UINT64_MAX
#define UINT64_MAX 0xFFFFFFFFFFFFFFFFLL
#endif

CPosixTimer::CPosixTimer() :
m_bStarted(false),
m_nPrev(0),
m_nStart(0)
{

}

CPosixTimer::~CPosixTimer()
{

}

bool CPosixTimer::Start() 
{
	if( ! m_bStarted ) {
		m_nStart = m_nPrev = GetTimeMilliSeconds();

		return ( m_bStarted = true );
	}

	return false;
}

bool CPosixTimer::Check(uint32_t * pElapsedMilliseconds) 
{
	if( m_bStarted ) {
		uint64_t current = GetTimeMilliSeconds();;
		if( pElapsedMilliseconds ) {
			if( current < m_nPrev )
				*pElapsedMilliseconds = UINT64_MAX - m_nPrev + current;
			else *pElapsedMilliseconds = current - m_nPrev;
		}
		m_nPrev = current;
		return true;
	}

	return false;
}

bool CPosixTimer::CheckTotal(uint32_t* pTotalElapsedMilliseconds) 
{
	if( ! pTotalElapsedMilliseconds ) return false;
	
	if( m_bStarted ) {
		uint64_t current = GetTimeMilliSeconds();
		
		if( current < m_nStart )
			*pTotalElapsedMilliseconds = UINT64_MAX - m_nStart + current;
		else *pTotalElapsedMilliseconds = current - m_nStart;
		
		return true;
	}

	return false;
}

