#ifndef PIPEY_IOLISTENER_H
#define PIPEY_IOLISTENER_H

#include "../common/common.h"

namespace pipey {
	namespace io {

		class IIOOperation;

		class IIOListener {
		public:
			IIOListener() { };
			virtual ~IIOListener() { };

		public:
			virtual void OperationDone(IIOOperation *pOperation) = 0;
		};

	}
}

#endif