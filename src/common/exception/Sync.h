#ifndef PIPEY_SYNC_H
#define PIPEY_SYNC_H

#include "Runtime.h"

namespace pipey {
	namespace common {
		namespace exception {

			class ESync : public ERuntime{
			public:
				ESync();
				ESync(const char * pMsg, bool bAllocatedMemory = false);

				virtual ~ESync() throw();
			};

		}
	}
}
#endif