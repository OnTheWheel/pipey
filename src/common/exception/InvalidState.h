#ifndef PIPEY_INVALIDSTATE_H
#define PIPEY_INVALIDSTATE_H

#include "Runtime.h"

namespace pipey {
	namespace common {
		namespace exception {

			class EInvalidState : public ERuntime{
			public:
				EInvalidState();
				EInvalidState(const char * pMsg, bool bAllocatedMemory = false);

				virtual ~EInvalidState() throw();
			};

		}
	}
}
#endif