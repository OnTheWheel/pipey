#ifndef PIPEY_INVALIDPARAMETER_H
#define PIPEY_INVALIDPARAMETER_H

#include "Logical.h"

namespace pipey {
	namespace common {
		namespace exception {

			class EInvalidParameter : public ELogical {
			public:
				EInvalidParameter();
				EInvalidParameter(const char * pMsg, bool bAllocatedMemory = false);

				virtual ~EInvalidParameter() throw();
			};

		}
	}
}
#endif