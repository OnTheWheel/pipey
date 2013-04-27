#ifndef PIPEY_IOOPERATOR_H
#define PIPEY_IOOPERATOR_H

#include "IOTarget.h"
#include "IOOperation.h"

namespace pipey {
	namespace io {
	
		class IIOOperator {
		public:
			IIOOperator() { };
			virtual ~IIOOperator() { };

		public:
			//virtual void DoMore(IIOMutableOperation *pOperation) = 0;
			//virtual bool HasMoreToDo(const IIOOperation *pOperation) const = 0;
			virtual void Process(IIOMutableOperation *pOperation) = 0;
		};
	}
}

#endif