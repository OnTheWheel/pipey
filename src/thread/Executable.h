#ifndef PIPEY_EXECUTABLE_H
#define PIPEY_EXECUTABLE_H

namespace pipey {
	namespace thread {

		class IExecutable {
		  public:
			IExecutable(void ) { };
			virtual ~IExecutable(void ) { };

			virtual void Execute(void * pParam) = 0;
		};

	}
}
#endif
