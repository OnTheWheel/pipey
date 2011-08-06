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

			unsigned long m_nPrev;

			unsigned long m_nStart;

			bool m_bStarted;


		public:
			virtual bool Start(unsigned long * pCurrentMilliseconds = NULL);

			virtual bool Check(unsigned long * pElapsedMilliseconds = NULL);

			virtual bool CheckTotal(unsigned long * pTotalElapsedMilliseconds);

		};

	}
}
#endif
