#ifndef PIPEY_SERVERSOCKET_H
#define PIPEY_SERVERSOCKET_H

#include "../BaseTarget.h"

namespace pipey {
	namespace io {

		class IServerSocket : public IBaseTarget {
		public:
			IServerSocket() {}
			virtual ~IServerSocket() {}

		public:
			virtual descriptor_t Open(const IO_TARGET_INIT * param) = 0;
			virtual void Close() = 0;
			virtual void DeleteChild(descriptor_t descriptor) = 0;
	
		private:
			
			
		};
	}
}

#endif