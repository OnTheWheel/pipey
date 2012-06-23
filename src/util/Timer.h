#ifndef PIPEY_TIMER_H
#define PIPEY_TIMER_H

namespace pipey {
	namespace util {

		class ITimer {
		public:
			ITimer() { };

			virtual ~ITimer() { };

			virtual bool Start() = 0;

			virtual bool Check(uint32_t * pElapsedMilliseconds = NULL) = 0;

			virtual bool CheckTotal(uint32_t * pTotalElapsedMilliseconds) = 0;

		};

	}
}
#endif
