#ifndef PIPEY_BASICMEMORYPOOL_H
#define PIPEY_BASICMEMORYPOOL_H

#include "MemoryPool.h"
#include <list>

namespace pipey {
	namespace memory {
		namespace pool {
			struct BASIC_BUF_INFO : public BUF_INFO
			{
				BASIC_BUF_INFO(char *pBuffer)
					: BUF_INFO(pBuffer)
				{ 
				}

				 ::std::list< BASIC_BUF_INFO *>::iterator iter;
			};

			class CBasicMemoryPool : public IMemoryPool
			{
			public:
				CBasicMemoryPool(void) { };
				virtual ~CBasicMemoryPool(void) { };

			private:
				

			public:
				virtual void Allocate(uint32_t nSize, CMemoryHandle &rHandle);
				virtual void Release(CMemoryHandle &rHandle);

				virtual void CloseHandle(CMemoryHandle & rHandle);
				virtual void DuplicateHandle(const CMemoryHandle & rSource, CMemoryHandle & rTarget);

				virtual char *GetBuffer(const CMemoryHandle &rHandle);

			private:
				::std::list<BASIC_BUF_INFO *> m_buffers;
			};
		}
	}
}

#endif