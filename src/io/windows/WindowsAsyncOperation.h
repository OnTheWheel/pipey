#ifndef PIPEY_WINDOWSASYNCOPERATION_H
#define PIPEY_WINDOWSASYNCOPERATION_H

#include "../IOBaseOperation.h"
#include "overlapped_ex.h"

namespace pipey {
	namespace io {
	
		class CWindowsAsyncOperation : public IIOBaseOperation {
		public:
			CWindowsAsyncOperation(IIOTarget *pTarget, IIOOperator *pOperator, IIOListener *pListener, pipey::memory::buffer::IBuffer *pBuffer, uint32_t nRequestedBytes);
			virtual ~CWindowsAsyncOperation() { };

		private:
			OVERLAPPED_EX m_overlapped;
		
		public:
			virtual OVERLAPPED_EX *GetOverlapped();
		
		};
	}
}

#endif