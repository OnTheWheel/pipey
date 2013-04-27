#ifndef PIPEY_WINDOWSCOMPLETIONPORT_H
#define PIPEY_WINDOWSCOMPLETIONPORT_H

#include "../IOModel.h"
#include "../../thread/pool/ThreadVector.h"
#include "../../util/SystemInfo.h"
#include "WindowsIOCPWorkerRoutine.h"

namespace pipey {
	namespace io {

		/** specifies attributes needed to initiate a I/O model(Windows IOCP, Posix AIO, ...).

		IO_MODEL_INIT cannot be used directly. You should instantiate a derived class correspond to a specific I/O model.
		(e.g.\ You need IOCP_INIT to initiate CWindowsIOCP.\ )
		*/
		class IOCP_INIT : public IO_MODEL_INIT {
		public:
			IOCP_INIT()
				:nConcurrent(0), nWorkerThreads(0)
			{}

			IOCP_INIT(int32_t concurrent)
				:nConcurrent(concurrent)
			{}

			virtual ~IOCP_INIT() {}

			uint32_t nConcurrent;
			uint32_t nWorkerThreads;
		};

		class CWindowsCompletionPort : public IIOModel {
		public:
			CWindowsCompletionPort(void);
			virtual ~CWindowsCompletionPort(void);

		public:
			virtual void Init(const IO_MODEL_INIT * pParam = NULL);
			virtual void Close();

			virtual bool IsValid() const;

			virtual bool IsAttatchable(IIOTarget *pTarget) const;
			virtual void Attach(IIOTarget *pTarget);

		private:
			void Init(const IOCP_INIT & param);

		private:
			HANDLE m_hHandle;
			pipey::thread::pool::CThreadVector m_workerThreads;
			CWindowsIOCPWorkerRoutine m_workerRoutine;
		};

	}
}

#endif