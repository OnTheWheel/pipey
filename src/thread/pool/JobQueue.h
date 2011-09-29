#ifndef PIPEY_JOBQUEUE_H
#define PIPEY_JOBQUEUE_H

#include "constant.h"
#include "JobCallback.h"

namespace pipey {
	namespace thread {
		namespace pool {

			template <typename T, typename INFO>
			class IJobQueue
			{
			public:
				IJobQueue(void) { }
				virtual ~IJobQueue(void) { }

			public:
				virtual bool IsEmpty() = 0;
				virtual bool IsPopable() = 0;

				virtual INFO* Push(const INFO &rInfo) = 0;
				virtual INFO* Pop() = 0;

				virtual void CleanupJob(INFO *pJobInfo, JOB_STATE eState = JOB_COMPLETE) = 0;

				virtual void CleanupAll() = 0;
			};

		}
	}
}

#endif