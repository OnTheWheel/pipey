#ifndef PIPEY_LOCKPTR_H
#define PIPEY_LOCKPTR_H

#include "Lock.h"

namespace pipey {
	namespace thread {
		namespace sync {

			class CLockPtr
			{
			public:
				explicit CLockPtr(ILock * pLock);

				virtual ~CLockPtr();


			private:
				ILock * m_pLock;

				bool m_bLocked;

				CLockPtr(const CLockPtr & rhs) { };

				const CLockPtr & operator =(const CLockPtr & rhs) { return *this; };


			public:
				virtual pipey::common::SYNC_RESULT AcquireLock();

				virtual void ReleaseLock();

				virtual bool IsValid() const;

				virtual operator bool() const;

				virtual bool IsLocked() const;

				virtual void Close();


			protected:
				virtual void SetLocked();

			};

		}
	}
}
#endif
