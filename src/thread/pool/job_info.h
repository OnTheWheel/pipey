#ifndef PIPEY_JOB_INFO_H
#define PIPEY_JOB_INFO_H

#include "constant.h"
#include "JobCallback.h"

namespace pipey {
	namespace thread {
		namespace pool {

			template<class T>
			struct JOB_INFO 
			{
				JOB_INFO() :
    				job(), nHandle(0), eState(JOB_INVALID), pCallback(0)
    				{ };

				JOB_INFO(const T & job, IJobCallback<T> * pCallback) :
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
