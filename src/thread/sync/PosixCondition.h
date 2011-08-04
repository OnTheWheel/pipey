#ifndef PIPEY_POSIXCONDITION_H
#define PIPEY_POSIXCONDITION_H

#include "Condition.h"
#include <pthread.h>
#include "PosixMutex.h"

namespace pipey {
	namespace thread {
		namespace sync {


			class POSIX_COND_INIT : public CONDITION_INIT {
			public:
				POSIX_COND_INIT(void ) :
#ifdef _POSIX_THREAD_PROCESS_SHARED
				bProcessShared(true),
#endif
				pMutex(NULL)
				{};

#ifdef _POSIX_THREAD_PROCESS_SHARED
				POSIX_COND_INIT(bool processShared, CPosixMutex *mutex) :
				bProcessShared(processShared),
				pMutex(mutex)
				{};
#else
				POSIX_COND_INIT(CPosixMutex *mutex) :                         
				pMutex(mutex)
                                {};                
#endif

#ifdef _POSIX_THREAD_PROCESS_SHARED
				bool bProcessShared;
#endif
				CPosixMutex *pMutex;
			};

			class CPosixCondition : public ICondition {
			public:
				CPosixCondition(void );

				virtual ~CPosixCondition(void );


			private:
				pthread_cond_t m_hCondition;

				bool m_bInited;

				CPosixMutex *m_pMutex;

				bool m_bOwnMutex;


			public:
				virtual void Init(const CONDITION_INIT * pParam = NULL);

				virtual pipey::common::SYNC_RESULT Wait(unsigned long nMilliSeconds = pipey::common::TIME_INFINITE);

				virtual pipey::common::SYNC_RESULT Test();

				virtual void Awake();

				virtual void AwakeAll();

				virtual void Close();
			};

		}
	}
}
#endif
