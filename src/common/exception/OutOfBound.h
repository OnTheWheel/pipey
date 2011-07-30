#ifndef PIPEY_OUTOFBOUND_H
#define PIPEY_OUTOFBOUND_H

#include "Logical.h"

namespace pipey {
	namespace common {
		namespace exception {

			class EOutOfBound : public ELogical {
			public:
				EOutOfBound();
				EOutOfBound(const char * pMsg, bool bAllocatedMemory = false);

				virtual ~EOutOfBound() throw();
			};

		}
	}
}
#endif