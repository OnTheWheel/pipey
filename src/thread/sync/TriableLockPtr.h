#ifndef PIPEY_TRIABLELOCKPTR_H
#define PIPEY_TRIABLELOCKPTR_H


#include "LockPtr.h"
#include "TriableLock.h"

namespace pipey {
	namespace thread {
		namespace sync {

			class CTriableLockPtr : virtual public CLockPtr 
			{
			public:
				CTriableLockPtr(ITriableLock * pLock);

				virtual ~CTriableLockPtr();


			private:
				ITriableLock * m_pLock;

				const CTriableLockPtr & operator =(const CTriableLockPtr & rhs) { return *this; };


			public:
				virtual pipey::common::SYNC_RESULT TryLock();

			};

		}
	}
}
#endif
