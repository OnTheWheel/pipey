/** @file
 This file defines constants and enumarations that referenced by several modules.
*/

#ifndef PIPEY_COMMON_CONSTANT_H
#define PIPEY_COMMON_CONSTANT_H

/// root name space of the pipey.
namespace pipey {

	/// includes common components referenced by other modules.
	namespace common {

		#if defined(WIN32) || defined(WIN64)
			typedef __int64 _integer64;
			typedef unsigned __int64 _uinteger64;
		#elif defined(__linux__) || defined(__unix__)
			typedef long long _integer64;
			typedef unsigned long long _uinteger64;
		#endif

		/// indicates infinite time.
		static const unsigned long TIME_INFINITE = 0xFFFFFFFF;

		/// indicates invalid value.
		static const unsigned long INVALID = 0xFFFFFFFF;

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
