#ifndef PIPEY_WINDOWSASYNCSOCKETOPERATOR_H
#define PIPEY_WINDOWSASYNCSOCKETOPERATOR_H

#include "../IOOperator.h"

namespace pipey {
	namespace io {
	
		class CWindowsAsyncSocketReader : public IIOOperator {
		public:
			CWindowsAsyncSocketReader() { };
			virtual ~CWindowsAsyncSocketReader() { };

		public:
			virtual void DoMore(IIOMutableOperation *pOperation);
			virtual bool HasMoreToDo(const IIOOperation *pOperation) const;
		};

		class CWindowsAsyncSocketWriter : public IIOOperator {
		public:
			CWindowsAsyncSocketWriter() { };
			virtual ~CWindowsAsyncSocketWriter() { };

		public:
			virtual void DoMore(IIOMutableOperation *pOperation);
			virtual bool HasMoreToDo(const IIOOperation *pOperation) const;
		};
	}
}

#endif