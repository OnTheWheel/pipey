#ifndef PIPEY_SIMPLETHREADPOOL_H
#define PIPEY_SIMPLETHREADPOOL_H

#include "job_info.h"
#include "SimpleJobQueue.h"
#include "BaseThreadPool.h"

namespace pipey {
	namespace thread {
		namespace pool {

			template <typename T>
			class CSimpleThreadPool : public IBaseThreadPool<T, JOB_INFO<T> >
			{
			public:
				CSimpleThreadPool(void)
				:IBaseThreadPool<T, JOB_INFO<T> >( new CSimpleJobQueue<T>() )
				{
				}

			public:
				virtual void PushJob(const T &rJob, IJobCallback<T> *pCallback, ::pipey::memory::CObjectHandle< JOB_INFO<T> > * pHandle);
			};

			template <typename T>
			void CSimpleThreadPool<T>::PushJob(const T &rJob, IJobCallback<T> *pCallback, ::pipey::memory::CObjectHandle< JOB_INFO<T> > * pHandle)
			{
				IBaseThreadPool<T, JOB_INFO<T> >::PushJob(JOB_INFO<T>(rJob, pCallback), pHandle);
			}

		}
	}
}

#endif