#ifndef PIPEY_LOGICAL_H
#define PIPEY_LOGICAL_H

#include "PipeyException.h"

namespace pipey {
	namespace common {
		namespace exception {

			class ELogical : public EPipeyException {
			public:
				ELogical();
				ELogical(const char * pMsg, bool bAllocatedMemory = false);

				virtual ~ELogical() throw();
			};

		}
	}
}
#endif