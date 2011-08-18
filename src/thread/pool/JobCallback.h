#ifndef PIPEY_JOBCALLBACK_H
#define PIPEY_JOBCALLBACK_H

#include <stdexcept>

namespace pipey {
	namespace thread {
		namespace pool {

			template<typename T>
			class IJobCallback {
			
			public:
				IJobCallback(void ) { };

				virtual ~IJobCallback(void ) { };

				virtual void ProcessJob(T & job) = 0;

				virtual void OnCancel(T & job, bool bTimeout = false) = 0;

				virtual void OnException(T & job, const std::exception & e) = 0;

			};

		}
	}
}
#endif
