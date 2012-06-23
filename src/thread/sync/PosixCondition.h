#ifndef PIPEY_POSIXCONDITION_H
#define PIPEY_POSIXCONDITION_H

#include "Condition.h"
#include <pthread.h>
#include "PosixMutex.h"

namespace pipey {
	namespace thread {
		namespace sync {

			/// specifies attributes needed to initiate a posix condition variable.
			class POSIX_COND_INIT : public CONDITION_INIT {
			public:
				POSIX_COND_INIT(void ) :
#ifdef _POSIX_THREAD_PROCESS_SHARED
					bProcessShared(false),
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
				/// Specifies whether a condition variable can be used for inter-process synchronization or not. This attribute is valid only if the target system defines _POSIX_THREAD_PROCESS_SHARED. Default is false.
				bool bProcessShared;
#endif
				/// A pointer to CPosixMutex to protect a condition variable. If you specify NULL, CPosixCondition creates and manages it's own mutex. Default is NULL.
				CPosixMutex *pMutex;
			};

			/// represents a posix condition variable.
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
				/// initiate a condition variable with specified attributes.
				/// if you don't specify pParam, it initiates a condition variable with default attributes.
				/// (bProcessShared == false, pMutex == NULL)
				/// @warning pParam should be type of pointer to POSIX_COND_INIT not CONDITION_INIT.
				/// @param pParam a pointer to POSIX_COND_INIT. Default is NULL.
				virtual void Init(const CONDITION_INIT * pParam = NULL);

				virtual pipey::common::SYNC_RESULT Wait(uint32_t nMilliSeconds = pipey::common::TIME_INFINITE);

				virtual pipey::common::SYNC_RESULT Test();

				virtual void Awake();

				/// broadcast a signal to all blocked threads, and awake them.
				virtual void AwakeAll();

				virtual void Close();
			};

		}
	}
}
#endif
