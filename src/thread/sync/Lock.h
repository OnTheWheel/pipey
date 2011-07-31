#ifndef PIPEY_LOCK_H
#define PIPEY_LOCK_H

#include "../../common/common.h"

namespace pipey {
	namespace thread {
		namespace sync {

			class LOCK_INIT {
			public:
				LOCK_INIT() {};

				virtual ~LOCK_INIT() {};

			};

			class ILock {
			public:
				ILock(void ) { };

				virtual ~ILock(void ) { };

				virtual void Init(const LOCK_INIT * pParam = NULL) = 0;

				virtual pipey::common::SYNC_RESULT AcquireLock() = 0;

				virtual void ReleaseLock() = 0;

				virtual void Close() = 0;

			};

		}
	}
}
#endif