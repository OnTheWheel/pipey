#ifndef PIPEY_SIMPLETHREADPOOL_H
#define PIPEY_SIMPLETHREADPOOL_H

#include "job_info.h"
#include "SimpleJobQueue.h"
#include "DefaultThreadPool.h"

namespace pipey {
	namespace thread {
		namespace pool {

			template <typename T>
			class CSimpleThreadPool : public CDefaultThreadPool<T, JOB_INFO<T> >
			{
			public:
				CSimpleThreadPool(void)
				:CDefaultThreadPool<T, JOB_INFO<T> >( new CSimpleJobQueue<T>() )
				{
				}
			};

		}
	}
}

#endif