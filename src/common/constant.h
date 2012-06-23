/** @file
 This file defines constants and enumarations that referenced by several modules.
*/

#ifndef PIPEY_COMMON_CONSTANT_H
#define PIPEY_COMMON_CONSTANT_H

#include "type.h"
/// root name space of the pipey.
namespace pipey {

	/// includes common components referenced by other modules.
	namespace common {

		/// indicates infinite time.
		static const uint32_t TIME_INFINITE = 0xFFFFFFFF;

		/// indicates invalid value.
		static const uint32_t INVALID = 0xFFFFFFFF;

		/// represents return values of thread or synchronization related functions.
		enum SYNC_RESULT {
			SYNC_SUCCESS		///< intended operation has been succeeded.
			,SYNC_FAIL			///< intended operation has been failed for some reasons.
			,SYNC_TIMEOUT		///< specified time limit has been expired.
			,SYNC_BUSY			///< another thread currently uses specified resource.
			,SYNC_ABANDONED		///< the thread that previously owned the mutex has been terminated without releases the mutex( only for Windows). 
								///< But the caller thread is allowed to possess the mutex. 
		};

		enum JOB_STATE {
		  JOB_WAIT,
		  JOB_READY,
		  JOB_PROCESSING,
		  JOB_COMPLETE,
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
#endif
