#ifndef PIPEY_IOTARGET_H
#define PIPEY_IOTARGET_H

#include "../common/common.h"

namespace pipey {
	namespace io {

		/** specifies attributes needed to initiate a I/O target(socket, file, ...).

		IO_TARGET_INIT cannot be used directly. You should instantiate a derived class correspond to a specific I/O target.
		(e.g.\ You need WIN_FILE_INIT to initiate CWindowsFile.\ )
		*/
		class IO_TARGET_INIT {
		public:
			IO_TARGET_INIT() {};
			virtual ~IO_TARGET_INIT() {};
		};

		class IIOTarget	{
		public:
			IIOTarget(void) { }
			virtual ~IIOTarget(void) { }

		public:
			virtual descriptor_t Open(const IO_TARGET_INIT * param) = 0;
			virtual void Close() = 0;

			virtual bool IsValid() const = 0;

			virtual operator bool() const {
				return IsValid();
			}

			virtual IIOTarget *GetParent() const = 0;

			virtual descriptor_t GetDescriptor() const = 0;
		};


	}
}

#endif