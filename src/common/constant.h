#ifndef PIPEY_CONSTANT_H
#define PIPEY_CONSTANT_H

namespace pipey {
	namespace common {

		static const unsigned long TIME_INFINITE = 0xFFFFFFFF;

		enum SYNC_RESULT {
			SYNC_SUCCESS,
			SYNC_FAIL,
			SYNC_TIMEOUT,
			SYNC_BUSY,
			SYNC_ABANDONED
		};

	}
}
#endif
