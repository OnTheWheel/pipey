#ifndef PIPEY_POSIXTIMER_H
#define PIPEY_POSIXTIMER_H

#include "../common/common.h"
#include "Timer.h"
#include <sys/time.h>

namespace pipey {
	namespace util {
		class CPosixTimer : public ITimer
		{
		public:
			CPosixTimer();

			~CPosixTimer();


		private:

			uint64_t m_nPrev;

			uint64_t m_nStart;

			bool m_bStarted;


		public:
			virtual bool Start();

			virtual bool Check(uint32_t * pElapsedMilliseconds = NULL);

			virtual bool CheckTotal(uint32_t * pTotalElapsedMilliseconds);

		};

	}
}
#endif
