#ifndef PIPEY_IOOPERATION_H
#define PIPEY_IOOPERATION_H

#include "IOTarget.h"
#include "IOListener.h"
#include "../memory/buffer/Buffer.h"

namespace pipey {
	namespace io {
	
		class IIOOperation {
		public:
			IIOOperation() { };
			virtual ~IIOOperation() { };

		public:
			virtual bool IsDone() const = 0;
			virtual bool IsSuccess() const = 0;
			virtual uint32_t GetCause() const = 0;
			virtual const IIOTarget *GetTarget() const = 0;
			virtual uint32_t GetTransferredBytes() const = 0;
			virtual uint32_t GetTotalTransferredBytes() const = 0;
			virtual uint32_t GetRequestedBytes() const = 0;
			virtual const IIOListener *GetListener() const = 0;
			virtual bool HasMoreToDo() const = 0;
		};

		class IIOMutableOperation : public IIOOperation {
		public:
			IIOMutableOperation() { };
			virtual ~IIOMutableOperation() { };

		public:
			virtual void SetDone(bool bDone) = 0;
			virtual void SetSuccess(bool bSuccess) = 0;
			virtual void SetCause(uint32_t nCause) = 0;
			//virtual void SetTarget(IIOTarget *pTarget) = 0;
			virtual void SetTransferredBytes(uint32_t nBytes) = 0;
			//virtual void SetTotalTransferredBytes(uint32_t nBytes) = 0;
			//virtual void SetRequestedBytes(uint32_t nBytes) = 0;
			//virtual void SetListener(IIOListener pListener) = 0;
			virtual pipey::memory::buffer::IBuffer *GetBuffer() = 0;
			
			virtual void DoMore() = 0;
		};
	}
}

#endif