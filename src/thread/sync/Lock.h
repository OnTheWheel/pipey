#ifndef PIPEY_LOCK_H
#define PIPEY_LOCK_H

#include "../../common/common.h"

namespace pipey {
	namespace thread {

		/// includes synchronization related interfaces and classes.
		namespace sync {

			/** specifies attributes needed to initiate a synchronization object.

			    LOCK_INIT cannot be used directly. You should instantiate a derived class correspond to a specific synchronization object.
				(e.g.\ You need WIN_MUTEX_INIT to initiate CWindowsMutex.\ )
			*/
			class LOCK_INIT {
			public:
				LOCK_INIT() {};

				virtual ~LOCK_INIT() {};

			};

			/// ancestor abstract class of all the lock and semaphore classes.
			class ILock {
			public:
				ILock(void ) { };

				virtual ~ILock(void ) { };

				/// initiate a synchronization object.
				virtual void Init(const LOCK_INIT * pParam = NULL) = 0;

				/// acquire a lock.
				virtual pipey::common::SYNC_RESULT AcquireLock() = 0;

				/// releases a lock.
				virtual void ReleaseLock() = 0;

				/// close a synchronization object and clean up all the related resources.
				virtual void Close() = 0;

			};

		}
	}
}
#endif