#ifndef PIPEY_MEMORYCORRUPTION_H
#define PIPEY_MEMORYCORRUPTION_H

#include "Runtime.h"

namespace pipey {
	namespace common {
		namespace exception {

			class EMemoryCorruption : public ERuntime{
			public:
				EMemoryCorruption();
				EMemoryCorruption(const char * pMsg, bool bAllocatedMemory = false);

				virtual ~EMemoryCorruption() throw();
			};

		}
	}
}
#endif