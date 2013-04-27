#ifndef PIPEY_WINDOWSASYNCOPERATION_H
#define PIPEY_WINDOWSASYNCOPERATION_H

#include "../IOBaseOperation.h"

namespace pipey {
	namespace io {

		class CWindowsAsyncOperation;

		struct OVERLAPPED_EX : public OVERLAPPED
		{
			CWindowsAsyncOperation *pOperation;
		};
	
		class CWindowsAsyncOperation : public IIOBaseOperation {
		public:
			CWindowsAsyncOperation(IIOTarget *pTarget, IIOOperator *pOperator, IIOListener *pListener, pipey::memory::buffer::IBuffer *pBuffer, uint32_t nRequestedBytes);
			CWindowsAsyncOperation(IIOTarget *pTarget, IIOOperator *pOperator, IIOListener *pListener, pipey::memory::buffer::IBuffer *pBuffer, uint32_t nRequestedBytes, HANDLE hEvent);
			CWindowsAsyncOperation(IIOTarget *pTarget, IIOOperator *pOperator, IIOListener *pListener, pipey::memory::buffer::IBuffer *pBuffer, uint32_t nRequestedBytes, LPWSAOVERLAPPED_COMPLETION_ROUTINE pRoutine);
			virtual ~CWindowsAsyncOperation() { };

		private:
			OVERLAPPED_EX m_overlapped;
			HANDLE m_hEvent;
			LPWSAOVERLAPPED_COMPLETION_ROUTINE m_pRoutine;
		
		public:
			virtual OVERLAPPED_EX *FillOverlapped();
			virtual LPWSAOVERLAPPED_COMPLETION_ROUTINE GetCompletionRoutine() const;
		
		};
	}
}

#endif