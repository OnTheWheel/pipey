#ifndef PIPEY_JOB_INFO_H
#define PIPEY_JOB_INFO_H

#include "constant.h"
#include "JobCallback.h"
#include "SimpleJobQueue.h"
#include "../../util/RandomPicker.h"
#include <list>

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

			template<typename T>
			struct CONTEXT_INFO;

			template<typename T>
			struct CONTEXT_JOB_INFO : public JOB_INFO<T>
			{
				CONTEXT_JOB_INFO() :
    				JOB_INFO<T>(),
					pContext(NULL)
    			{ }

				CONTEXT_JOB_INFO(const CONTEXT_JOB_INFO<T> &info) :
					JOB_INFO<T>(info.job, info.pCallback),
					pContext(info.pContext)
    			{ }

				CONTEXT_JOB_INFO(const T &job, IJobCallback<T> *pCallback, CONTEXT_INFO<T> *pContext) :
    				JOB_INFO<T>(job, pCallback),
						pContext(pContext)
    			{ }
		
				CONTEXT_INFO<T> *pContext;
			};

			template<typename T>
			struct CONTEXT_INFO
			{
				CONTEXT_INFO():
					queue(), nHandle(0), nPickerReference(RP_INVALID_REFERENCE), pRunningJob(NULL), bValid(true)
				{ }

				CSimpleJobQueue< T, CONTEXT_JOB_INFO<T> > queue;
				unsigned long nHandle;
				unsigned long nPickerReference;
				CONTEXT_JOB_INFO<T> *pRunningJob;
				typename ::std::list< CONTEXT_INFO<T>* >::iterator iter;
				bool bValid;
			};

		}
	}
}
#endif
