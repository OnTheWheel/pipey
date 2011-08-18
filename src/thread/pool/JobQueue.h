#ifndef PIPEY_JOBQUEUE_H
#define PIPEY_JOBQUEUE_H

#include "job_node.h"

namespace pipey {
	namespace thread {
		namespace pool {

			template <typename T>
			class IJobQueue
			{
			public:
				IJobQueue(void) { }
				virtual ~IJobQueue(void) { }

			public:
				virtual bool IsEmpty() = 0;
				virtual bool IsPopable() = 0;

				virtual JOB_NODE<T>* Push(const T &job, IJobCallback<T> *pCallback) = 0;
				virtual JOB_NODE<T>* Pop() = 0;

				virtual bool CleanupJob(JOB_NODE<T> *pJobNode, JOB_STATE eState = JOB_COMPLETE) = 0;
				virtual void Cleanup() = 0;
			};

		}
	}
}

#endif