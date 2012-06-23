#ifndef PIPEY_BASICMEMORYPOOL_H
#define PIPEY_BASICMEMORYPOOL_H

#include "MemoryPool.h"
#include <map>

namespace pipey {
	namespace memory {
		namespace pool {

			class CBasicMemoryPool : public IMemoryPool
			{
			public:
				CBasicMemoryPool(void) { };
				virtual ~CBasicMemoryPool(void) { };

			public:
				virtual void Allocate(uint32_t nSize, CMemoryHandle &rHandle);
				virtual void CloseHandle(CMemoryHandle & rHandle);
				virtual void DuplicateHandle(const CMemoryHandle & rSource, CMemoryHandle & rTarget);

				virtual char *GetBuffer(const CMemoryHandle &rHandle);

			private:
				std::map<char *, uint32_t> m_handles;
			};
		}
	}
}

#endif