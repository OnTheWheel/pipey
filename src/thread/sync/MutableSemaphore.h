#ifndef PIPEY_MUTABLESEMAPHORE_H
#define PIPEY_MUTABLESEMAPHORE_H

#include "TriableTimerableLock.h"
#include "Semaphore.h"
#include "DefaultLock.h"
#include "DefaultCondition.h"

namespace pipey {
	namespace thread {
		namespace sync {

			class MUT_SEM_INIT : public LOCK_INIT {
			public:
				MUT_SEM_INIT() :
				  nMaxLimit(0),
					  nInitialValue(0)
				  { };

				  MUT_SEM_INIT(unsigned long max, unsigned long initial) :
				  nMaxLimit(max),
					  nInitialValue(initial)
				  { };

				  unsigned long nMaxLimit;

				  unsigned long nInitialValue;

			};

			class CMutableSemaphore : public ITriableTimerableLock, public ISemaphore {
			public:
				CMutableSemaphore(void );

				virtual ~CMutableSemaphore(void );


			protected:
				unsigned long m_nMaxLimit;

				unsigned long m_nValue;

				unsigned long m_nOwner;

				bool m_bInited;

				CDefaultLock m_lock;

				CDefaultCondition m_condition;


			public:
				virtual void Init(const LOCK_INIT * pParam = NULL);

				virtual pipey::common::SYNC_RESULT AcquireLock();

				virtual void ReleaseLock();

				virtual void Close();

				virtual pipey::common::SYNC_RESULT TryLock();

				virtual pipey::common::SYNC_RESULT AcquireTimedLock(unsigned long nMilliSeconds = pipey::common::TIME_INFINITE);

				virtual void ReleaseMultiple(unsigned long nCount);

				virtual void SetMaxLimit(unsigned long nMaxLimit);

				virtual unsigned long GetMaxLimit() const;

				virtual unsigned long GetCurrentValue() const;

				virtual unsigned long GetCurrentOwner() const;

			};

		}
	}
}
#endif
