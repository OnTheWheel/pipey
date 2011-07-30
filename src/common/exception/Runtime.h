#ifndef PIPEY_RUNTIME_H
#define PIPEY_RUNTIME_H

#include "PipeyException.h"

namespace pipey {
	namespace common {
		namespace exception {

			class ERuntime : public EPipeyException {
			public:
				ERuntime();
				ERuntime(const char * pMsg, bool bAllocatedMemory = false);

				virtual ~ERuntime() throw();
			};

		}
	}
}

#endif