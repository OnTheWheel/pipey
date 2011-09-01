#ifndef PIPEY_JOB_INFO_H
#define PIPEY_JOB_INFO_H

#include "constant.h"
#include "JobCallback.h"

namespace pipey {
	namespace thread {
		namespace pool {

			template<typename T>
			struct JOB_INFO 
			{
				JOB_INFO() :
    				job(), nHandle(0), eState(JOB_INVALID), pCallback(0)
    			{ }

				JOB_INFO(const JOB_INFO<T> &info) :
					job(info.job), nHandle(info.nHandle), eState(info.eState), pCallback(info.pCallback)
    			{ }

				JOB_INFO(const T & job, IJobCallback<T> * pCallback) :
    				job(job), nHandle(0), eState(JOB_INVALID), pCallback(pCallback)
    			{ }

				T job;

				unsigned long nHandle;

				JOB_STATE eState;

				IJobCallback<T> * pCallback;

			};

			template<typename T, typename C>
			struct CONTEXT_JOB_INFO : public JOB_INFO<T>
			{
				CONTEXT_JOB_INFO() :
    				JOB_INFO(), context()
    			{ }

				CONTEXT_JOB_INFO(const CONTEXT_JOB_INFO<T,C> &info) :
					JOB_INFO(info.job, info.pCallback), context(info.context)
    			{ }

				CONTEXT_JOB_INFO(const T & job, const C &context, IJobCallback<T> * pCallback) :
    				JOB_INFO(job, pCallback), context(context)
    			{ }

				C context;

			};

		}
	}
}
#endif
