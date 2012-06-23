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

				  MUT_SEM_INIT(uint32_t max, uint32_t initial) :
				  nMaxLimit(max),
					  nInitialValue(initial)
				  { };

				  uint32_t nMaxLimit;

				  uint32_t nInitialValue;

			};

			class CMutableSemaphore : public ITriableTimerableLock, public ISemaphore {
			public:
				CMutableSemaphore(void );

				virtual ~CMutableSemaphore(void );


			protected:
				uint32_t m_nMaxLimit;

				uint32_t m_nValue;

				uint32_t m_nOwner;

				bool m_bInited;

				CDefaultLock m_lock;

				CDefaultCondition m_condition;


			public:
				virtual void Init(const LOCK_INIT * pParam = NULL);

				virtual pipey::common::SYNC_RESULT AcquireLock();

				virtual void ReleaseLock();

				virtual void Close();

				virtual pipey::common::SYNC_RESULT TryLock();

				virtual pipey::common::SYNC_RESULT AcquireTimedLock(uint32_t nMilliSeconds = pipey::common::TIME_INFINITE);

				virtual void ReleaseMultiple(uint32_t nCount);

				virtual void SetMaxLimit(uint32_t nMaxLimit);

				virtual uint32_t GetMaxLimit() const;

				virtual uint32_t GetCurrentValue() const;

				virtual uint32_t GetCurrentOwner() const;

			};

		}
	}
}
#endif
