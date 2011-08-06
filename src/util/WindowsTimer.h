#ifndef PIPEY_WINDOWSTIMER_H
#define PIPEY_WINDOWSTIMER_H

#include "../common/common.h"
#include "Timer.h"

namespace pipey {
	namespace util {
		class CWindowsTimer : public ITimer
		{
		public:
			CWindowsTimer();

			~CWindowsTimer();


		protected:
			LARGE_INTEGER m_liFreq;

			LARGE_INTEGER m_liPrev;

			LARGE_INTEGER m_liStart;

			unsigned long m_nPrev;

			unsigned long m_nStart;

			bool m_bStarted;

			bool m_bSupportHighFreq;


		public:
			virtual bool Start(unsigned long * pCurrentMilliseconds = NULL);

			virtual bool Check(unsigned long * pElapsedMilliseconds = NULL);

			virtual bool StartHighFrequency(LARGE_INTEGER * pCurrent = NULL);

			virtual bool CheckHighFrequency(LARGE_INTEGER * pElapsed = NULL);

			virtual bool CheckTotalHighFrequency(LARGE_INTEGER * pTotal);

			virtual bool CheckTotal(unsigned long * pTotalElapsedMilliseconds);

			virtual bool IsHighFrequency() const { return m_bSupportHighFreq; };

		};

	}
}
#endif
