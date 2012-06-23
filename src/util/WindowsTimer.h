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

			uint32_t m_nPrev;

			uint32_t m_nStart;

			bool m_bStarted;

			bool m_bSupportHighFreq;


		public:
			virtual bool Start();

			virtual bool Check(uint32_t * pElapsedMilliseconds = NULL);

			virtual bool StartHighFrequency();

			virtual bool CheckHighFrequency(LARGE_INTEGER * pElapsed = NULL);

			virtual bool CheckTotalHighFrequency(LARGE_INTEGER * pTotal);

			virtual bool CheckTotal(uint32_t * pTotalElapsedMilliseconds);

			virtual bool IsHighFrequency() const { return m_bSupportHighFreq; };

		};

	}
}
#endif
