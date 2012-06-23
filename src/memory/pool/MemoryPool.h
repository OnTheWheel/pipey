#ifndef PIPEY_MEMORYPOOL_H
#define PIPEY_MEMORYPOOL_H

#include "../HandleManipulator.h"

namespace pipey {
	namespace memory {
		namespace pool {

			typedef ::pipey::memory::CObjectHandle<char> CMemoryHandle;

			class IMemoryPool : public ::pipey::memory::IHandleManipulator<char>
			{
			public:
				IMemoryPool(void) { };
				virtual ~IMemoryPool(void) { };

			public:
				virtual void Allocate(uint32_t nSize, CMemoryHandle &rHandle) = 0;
				virtual void CloseHandle(CMemoryHandle & rHandle) = 0;
				virtual void DuplicateHandle(const CMemoryHandle & rSource, CMemoryHandle & rTarget) = 0;

				virtual char *GetBuffer(const CMemoryHandle &rHandle) = 0;
			};
		}
	}
}

#endif