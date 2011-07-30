#ifndef PIPEY_DIVIDEBYZERO_H
#define PIPEY_DIVIDEBYZERO_H

#include "Logical.h"

namespace pipey {
	namespace common {
		namespace exception {

			class EDivideByZero : public ELogical {
			public:
				EDivideByZero();
				EDivideByZero(const char * pMsg, bool bAllocatedMemory = false);

				virtual ~EDivideByZero() throw();
			};

		}
	}
}
#endif