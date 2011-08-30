/** @file
 This file defines constants and enumarations referenced in thread pool module.
*/

#ifndef PIPEY_THREAD_POOL_CONSTANT_H
#define PIPEY_THREAD_POOL_CONSTANT_H

namespace pipey {
	namespace thread {

		/// includes classes related to thread pool.
		namespace pool {

			enum JOB_STATE {
			  JOB_WAIT,
			  JOB_READY,
			  JOB_PROCESSING,
			  JOB_COMPLETE,
			  JOB_EXCEPTION,
			  JOB_CANCEL,
			  JOB_TIMEOUT,
			  JOB_INVALID
			};

			enum JOB_PRIORITY {
			  PRIORITY_HIGH =  0,
			  PRIORITY_MIDDLE_HIGH =  1,
			  PRIORITY_NORMAL =  2,
			  PRIORITY_MIDDLE_LOW =  3,
			  PRIORITY_LOW =  4
			};

		}
	}
}
#endif
