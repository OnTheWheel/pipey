#ifndef PIPEY_WINDOWSASYNCSERVERSOCKET_H
#define PIPEY_WINDOWSASYNCSERVERSOCKET_H

#include "../ServerSocket.h"
#include <map>

namespace pipey {
	namespace io {

		class CWindowsAsyncServerSocket : public IServerSocket {
		public:
			CWindowsAsyncServerSocket();
			virtual ~CWindowsAsyncServerSocket();

		public:
			virtual descriptor_t Open(const IO_TARGET_INIT * param);
			virtual void Close();
			virtual void DeleteChild(descriptor_t descriptor);
	
		private:
			std::map<descriptor_t, IIOTarget *> m_children;
			
		};
	}
}

#endif