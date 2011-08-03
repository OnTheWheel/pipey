#ifndef PIPEY_CONDITION_H
#define PIPEY_CONDITION_H


#include "../../common/common.h"

namespace pipey {
	namespace thread {
		namespace sync {

			class CONDITION_INIT {
			public:
				CONDITION_INIT() {};

				virtual ~CONDITION_INIT() {};

			};

			class ICondition {
			public:
				ICondition(void ) { };

				virtual ~ICondition(void ) { };

				virtual void Init(const CONDITION_INIT * pParam) = 0;

				virtual pipey::common::SYNC_RESULT Wait(unsigned long nMilliSeconds = pipey::common::TIME_INFINITE) = 0;

				virtual pipey::common::SYNC_RESULT Test() = 0;

				virtual void Awake() = 0;

				virtual void Close() = 0;

			};

		}
	}
}
#endif
