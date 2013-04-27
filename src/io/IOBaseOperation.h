#ifndef PIPEY_BASEOPERATION_H
#define PIPEY_BASEOPERATION_H

#include "IOOperator.h"

namespace pipey {
	namespace io {
	
		class IIOBaseOperation : public IIOMutableOperation {
		public:
			IIOBaseOperation(IIOTarget *pTarget, IIOOperator *pOperator, IIOListener *pListener, pipey::memory::buffer::IBuffer *pBuffer, uint32_t nRequestedBytes);
			virtual ~IIOBaseOperation() { };

		private:
			IIOTarget *m_pTarget;
			IIOOperator *m_pOperator;
			IIOListener *m_pListener;
			pipey::memory::buffer::IBuffer *m_pBuffer;

			uint32_t m_nRequestedBytes;
			uint32_t m_nTransfferedBytes;
			uint32_t m_nTotalTransfferedBytes;

			bool m_bDone;
			bool m_bSuccess;

			uint32_t m_nCause;
	
		public:
			virtual bool IsDone() const;
			virtual bool IsSuccess() const;
			virtual uint32_t GetCause() const;
			virtual const IIOTarget *GetTarget() const;
			virtual uint32_t GetTransferredBytes() const;
			virtual uint32_t GetTotalTransferredBytes() const;
			virtual uint32_t GetRequestedBytes() const;
			virtual const IIOListener *GetListener() const;
			virtual bool HasMoreToDo() const;

			virtual void SetDone(bool bDone);
			virtual void SetSuccess(bool bSuccess);
			virtual void SetCause(uint32_t nCause);
			//virtual void SetTarget(IIOTarget *pTarget);
			virtual void SetTransferredBytes(uint32_t nBytes);
			//virtual void SetTotalTransferredBytes(uint32_t nBytes);
			//virtual void SetRequestedBytes(uint32_t nBytes);
			//virtual void SetListener(IIOListener pListener);
			virtual pipey::memory::buffer::IBuffer *GetBuffer();
			
			virtual void DoMore();
		
		};
	}
}

#endif