#ifndef PIPEY_IOMODEL_H
#define PIPEY_IOMODEL_H

#include "IOTarget.h"

namespace pipey {
	namespace io {

		/** specifies attributes needed to initiate a I/O model(Windows IOCP, Posix AIO, ...).

		IO_MODEL_INIT cannot be used directly. You should instantiate a derived class correspond to a specific I/O model.
		(e.g.\ You need IOCP_INIT to initiate CWindowsIOCP.\ )
		*/
		class IO_MODEL_INIT {
		public:
			IO_MODEL_INIT() {};
			virtual ~IO_MODEL_INIT() {};
		};

		class IIOModel	{
		public:
			IIOModel(void) { }
			virtual ~IIOModel(void) { }

		public:
			virtual void Init(const IO_MODEL_INIT * pParam = NULL) = 0;
			virtual void Close() = 0;

			virtual bool IsValid() const = 0;

			virtual operator bool() const {
				return IsValid();
			}

			virtual bool IsAttatchable(IIOTarget *pTarget) const = 0;

			virtual void Attach(IIOTarget *pTarget) = 0;
		};


	}
}

#endif