#ifndef PIPEY_JOB_NODE_H
#define PIPEY_JOB_NODE_H

#include "constant.h"
#include "JobCallback.h"

namespace pipey {
	namespace thread {
		namespace pool {

			template<class T>
			struct JOB_NODE 
			{
				JOB_NODE() :
    				job(), nHandle(0), eState(JOB_INVALID), pCallback(0)
    				{ };

				JOB_NODE(const T & job, IJobCallback<T> * pCallback) :
    				job(job), nHandle(0), eState(JOB_INVALID), pCallback(pCallback)
    				{ };

				T job;

				unsigned long nHandle;

				JOB_STATE eState;

				IJobCallback<T> * pCallback;

			};

		}
	}
}
#endif
