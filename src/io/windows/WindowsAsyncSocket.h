#ifndef PIPEY_WINDOWSASYNCSOCKET_H
#define PIPEY_WINDOWSASYNCSOCKET_H

#include "../ServerSocket.h"

namespace pipey {
	namespace io {

		class CWindowsAsyncSocket : public IBaseTarget {
		public:
			CWindowsAsyncSocket();
			CWindowsAsyncSocket(IServerSocket *pParent, HANDLE hSocket);
			virtual ~CWindowsAsyncSocket();

		public:
			virtual descriptor_t Open(const IO_TARGET_INIT * param);
			virtual void Close();
	
		private:
			
			
		};
	}
}

#endif