#include "WindowsCompletionPort.h"

using namespace pipey::io;
using namespace pipey::util;
using namespace pipey::common::exception;

CWindowsCompletionPort::CWindowsCompletionPort() 
	:m_hHandle(NULL), m_workerThreads()
{
}

CWindowsCompletionPort::~CWindowsCompletionPort() 
{
	SAFE_TRY(Close());
}

void CWindowsCompletionPort::Init(const IO_MODEL_INIT * pParam) 
{
	if( ! IsValid() ) {
		if( pParam ) {
			const IOCP_INIT *pInit = dynamic_cast<const IOCP_INIT *>( pParam ) ;
			if( pInit ) {
				Init(*pInit);
			} else throw EInvalidParameter("EInvalidParameter => CWindowsCompletionPort::Init - pParam should be type of (IOCP_INIT *).");
		} else {
			IOCP_INIT param;
			Init(param);
		}
	} else throw EInvalidState("EInvalidState => CWindowsCompletionPort::Init - This IOCP is already initiated.");
}

void CWindowsCompletionPort::Init(const IOCP_INIT & param) 
{
	__try {
		m_hHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, param.nConcurrent);
		if(m_hHandle) {
			uint32_t nThreads = param.nWorkerThreads == 0 ? GetNumberOfProcessors() : param.nWorkerThreads;
			m_workerThreads.AddThread(nThreads, m_workerRoutine, (void *)m_hHandle);
		}
	} __except(EXCEPTION_EXECUTE_HANDLER ) {
		if(m_hHandle) ::CloseHandle(m_hHandle);
		m_hHandle = NULL;
		throw ERuntime("ERuntime => CWindowsCompletionPort::Init - unknown exception");
	}
}

void CWindowsCompletionPort::Close() 
{
	if(IsValid()) {
		__try {
			for(size_t i=0; i<m_workerThreads.size(); i++) 
				::PostQueuedCompletionStatus(m_hHandle, 0, 0, 0);
			m_workerThreads.Close();
			::CloseHandle(m_hHandle);
			m_hHandle = NULL;
			
		} __except(EXCEPTION_EXECUTE_HANDLER ) {
			if(m_hHandle) ::CloseHandle(m_hHandle);
			m_hHandle = NULL;
			throw ERuntime("ERuntime => CWindowsCompletionPort::Init - unknown exception");
		}
	} else throw EInvalidState("EInvalidState => CWindowsCompletionPort::Close - This IOCP is not properly initiated.");
}

bool CWindowsCompletionPort::IsValid() const
{
	return m_hHandle != NULL;
}

bool CWindowsCompletionPort::IsAttatchable(IIOTarget *pTarget) const 
{
	return true;
}

void CWindowsCompletionPort::Attach(IIOTarget *pTarget)
{
	if( pTarget && pTarget->IsValid() ) {
		if(IsValid()) {
			__try {
				HANDLE hResult = ::CreateIoCompletionPort(pTarget->GetDescriptor(), m_hHandle, (ULONG_PTR)pTarget, 0);
				if( hResult == 0 )
					throw ERuntime("ERuntime => CWindowsCompletionPort::Attach - unknown exception");
			} __except(EXCEPTION_EXECUTE_HANDLER ) {
				throw ERuntime("ERuntime => CWindowsCompletionPort::Attach - unknown exception");
			}
		} else throw EInvalidState("EInvalidState => EInvalidParameter - This IOCP is not properly initiated.");
	} else throw EInvalidParameter("EInvalidParameter => EInvalidParameter - pTarget is not valid.");
}