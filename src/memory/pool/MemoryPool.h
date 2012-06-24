#ifndef PIPEY_MEMORYPOOL_H
#define PIPEY_MEMORYPOOL_H

#include "../HandleManipulator.h"

namespace pipey {
	namespace memory {
		namespace pool {

			struct BUF_INFO
			{
				BUF_INFO(char *pBuffer)
					:bValid(true), nHandle(1), pBuffer(pBuffer)
				{ }

				bool bValid;
				uint32_t nHandle;
				char *pBuffer;
			};

			typedef ::pipey::memory::CObjectHandle<BUF_INFO> CMemoryHandle;

			class IMemoryPool : public ::pipey::memory::IHandleManipulator<BUF_INFO>
			{
			public:
				IMemoryPool(void) { };
				virtual ~IMemoryPool(void) { };

			public:
				virtual void Allocate(uint32_t nSize, CMemoryHandle &rHandle) = 0;
				virtual void Release(CMemoryHandle &rHandle) = 0;

				virtual void CloseHandle(CMemoryHandle & rHandle) = 0;
				virtual void DuplicateHandle(const CMemoryHandle & rSource, CMemoryHandle & rTarget) = 0;

				virtual char *GetBuffer(const CMemoryHandle &rHandle) = 0;
			};
		}
	}
}

#endif