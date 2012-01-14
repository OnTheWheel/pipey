#ifndef PIPEY_CONDITION_H
#define PIPEY_CONDITION_H


#include "../../common/common.h"

namespace pipey {
	namespace thread {
		namespace sync {

			/** specifies attributes needed to initiate a condition variable.

			    CONDITION_INIT cannot be used directly. You should instantiate a derived class correspond to a specific condition variable.
				(e.g.\ You need WIN_EVENT_INIT to initiate CWindowsEvent.\ )
			*/
			class CONDITION_INIT {
			public:
				CONDITION_INIT() {};

				virtual ~CONDITION_INIT() {};

			};

			/// interface of all the condition variable classes.
			class ICondition {
			public:
				ICondition(void ) { };

				virtual ~ICondition(void ) { };

				/// initiate a condition variable with specified attributes.
				/// if you don't specify pParam, it initiates a condition variable with default attributes.
				/// @param pParam a pointer to subclass of CONDITION_INIT. Default is NULL.
				virtual void Init(const CONDITION_INIT * pParam) = 0;

				/// wait until a condition variable is signaled.
				/// if you don't specify nMilliSeconds, it blocks until a condition variable become signaled.
				/// @param nMilliSeconds time to wait in ms. Default is TIME_INFINITE.
				/// @return 
				///			- SYNC_SUCCESS	: the condition variable is signaled-state and return successfully.\n
				///			- SYNC_TIMEOUT	: operation failed due to time-out.
				virtual pipey::common::SYNC_RESULT Wait(unsigned long nMilliSeconds = pipey::common::TIME_INFINITE) = 0;

				/// try to wait for a condition variable, and returns SYNC_BUSY immediately if the condition variable is not signaled at that moment.
				/// @return 
				///			- SYNC_SUCCESS	: the condition variable is signaled-state at that moment. \n
				///			- SYNC_BUSY		: the condition variable is not signaled at that moment.
				virtual pipey::common::SYNC_RESULT Test() = 0;

				/// send a signal to the condition variable.
				virtual void Awake() = 0;

				/// close a condition variable, and clean up all the resources.
				virtual void Close() = 0;

			};

		}
	}
}
#endif
