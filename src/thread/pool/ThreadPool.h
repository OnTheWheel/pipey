#ifndef PIPEY_THREADPOOL_H
#define PIPEY_THREADPOOL_H

#include "../../memory/HandleManipulator.h"
#include "../Executable.h"
#include "JobQueue.h"


namespace pipey {
	namespace thread {
		namespace pool {

			template <typename T, typename NODE>
			class IThreadPool : public pipey::memory::IHandleManipulator<NODE>
			{
			public:
				IThreadPool(void) { }
				virtual ~IThreadPool(void) { }

			protected:
				class CThreadPoolExecutable : public IExecutable
				{
				public:
					virtual void Execute(void *pParam)
					{
						if( pParam )
						{
							IThreadPool<T, NODE> *pPool = (IThreadPool<T, NODE> *)pParam;
							if( ! pPool->IsThreadReady() ) return;

							while( pPool->PopAndProcess() ) { }
						}
					}
				};
				virtual bool IsThreadReady() = 0;
				virtual IJobQueue<T> *GetJobQueue() const = 0;

			public:
				virtual void Init(unsigned long nMinThread = 0, unsigned long nMaxThread = 0, unsigned long nActiveThread = 0) = 0;

				//virtual bool AdjustThreadParam(unsigned long nMinThread = 0, unsigned long nMaxThread = 0, unsigned long nActiveThread = 0) = 0;

				virtual void PushJob(const T &rJob, IJobCallback<T> *pCallback, pipey::memory::CObjectHandle<NODE> *pHandle = NULL) = 0;
				virtual bool PopAndProcess() = 0;

				virtual void CancelJob(const pipey::memory::CObjectHandle<NODE> &rHandle) = 0;
				virtual JOB_STATE GetJobState(const pipey::memory::CObjectHandle<NODE> &rHandle) const = 0;

				virtual void Close() = 0;

				virtual void CloseHandle(pipey::memory::CObjectHandle<NODE> &rHandle) = 0;
				virtual void DuplicateHandle(const pipey::memory::CObjectHandle<NODE> &rSource, pipey::memory::CObjectHandle<NODE> &rTarget) = 0;
			};

			/*
			template <typename T>
			class IThreadPool : public CHandleManipulator<JOB_NODE<T>>
			{
			public:
				IThreadPool(void) { }
				virtual ~IThreadPool(void) { }

			protected:
				class CThreadPoolExecutable : public IExecutable
				{
				public:
					virtual void Execute(void *pParam)
					{
						if( pParam )
						{
							IThreadPool<T> *pPool = (IThreadPool<T> *)pParam;
							if( ! pPool->IsThreadReady() ) return;

							while( pPool->PopAndProcess() ) { }
						}
					}
				};

				virtual bool IsThreadReady() = 0;
				virtual IJobQueue<T> *GetJobQueue() const = 0;

			public:
				virtual void Init(unsigned long nMinThread = 0, unsigned long nMaxThread = 0, unsigned long nActiveThread = 0) = 0;

				//virtual bool AdjustThreadParam(unsigned long nMinThread = 0, unsigned long nMaxThread = 0, unsigned long nActiveThread = 0) = 0;

				virtual void PushJob(const T &rJob, IJobCallback<T> *pCallback, pipey::memory::CObjectHandle<JOB_NODE<T>> *pHandle = NULL) = 0;
				virtual bool PopAndProcess() = 0;

				virtual void CancelJob(const pipey::memory::CObjectHandle<JOB_NODE<T>> &rHandle) = 0;
				virtual JOB_STATE GetJobState(const pipey::memory::CObjectHandle<JOB_NODE<T>> &rHandle) const = 0;

				virtual void Close() = 0;
			};*/

		}
	}
}

#endif