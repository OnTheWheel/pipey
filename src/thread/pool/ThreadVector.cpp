#include "ThreadVector.h"

using namespace pipey::common::exception;
using namespace pipey::thread;
using namespace pipey::thread::pool;

CThreadVector::CThreadVector(void ) :
m_bThreadReady(false)
{

}

CThreadVector::~CThreadVector(void )
{
	SAFE_TRY( Close() );
}

void CThreadVector::AddThread(unsigned long nCount, IExecutable & rExecutable, void * pParam) 
{
	if( nCount == 0 ) throw EInvalidParameter("EInvalidParameter => CThreadVector::AddThread - nCount cannot be zero.");

	SAFE_TRY( m_threadReady.Close() );

	unsigned long nPrev = size();

	try	{
		m_threadReady.Init();

		THREAD_INIT init;
		init.pParam = pParam;
		init.pExec = &rExecutable;

		if( size() + nCount > capacity() )
			reserve( size() + nCount - capacity());

		unsigned long i;
		for( i=0; i<nCount; i++) {
			CDefaultThread *pThr = NULL;
			pThr = new CDefaultThread();
			try	{
				pThr->Init(init);
			}
			catch(...) {
				delete pThr;
				throw;
			}
			push_back(pThr);
		}
		m_bThreadReady = true;
		m_threadReady.Awake();
	}
	catch(...) {
		m_bThreadReady = false;
		SAFE_TRY( m_threadReady.Awake() );
		unsigned long i;
		for(i = size() ; i > nPrev ; i--) {
			SAFE_TRY( at(i-1)->Wait() );
			SAFE_TRY( delete at(i-1) );
			pop_back();
		}
		throw;
	}
}

bool CThreadVector::IsThreadReady() 
{
	m_threadReady.Wait();
	m_threadReady.Awake();
	return m_bThreadReady;
}

void CThreadVector::Close() 
{
	for(unsigned long i=0; i<size(); i++) {
		SAFE_TRY( at(i)->Wait() );
		delete at(i);
	}
	clear();

	SAFE_TRY( m_threadReady.Close() );
}

