#ifndef PIPEY_TIMER_H
#define PIPEY_TIMER_H

namespace pipey {
	namespace util {

		class ITimer {
		public:
			ITimer() { };

			virtual ~ITimer() { };

			virtual bool Start(unsigned long * pCurrentMilliseconds = NULL) = 0;

			virtual bool Check(unsigned long * pElapsedMilliseconds = NULL) = 0;

			virtual bool CheckTotal(unsigned long * pTotalElapsedMilliseconds) = 0;

		};

	}
}
#endif
