#ifndef PIPEY_MEMORYALLOCATION_H
#define PIPEY_MEMORYALLOCATION_H

#include "Runtime.h"

namespace pipey {
	namespace common {
		namespace exception {

			class EMemoryAllocation : public ERuntime{
			public:
				EMemoryAllocation();
				EMemoryAllocation(const char * pMsg, bool bAllocatedMemory = false);

				virtual ~EMemoryAllocation() throw();
			};

		}
	}
}
#endif