#ifndef PIPEY_CONTEXTAWARETHREADPOOL_H
#define PIPEY_CONTEXTAWARETHREADPOOL_H

#include "job_info.h"
#include "ContextAwareJobQueue.h"
#include "BaseThreadPool.h"

namespace pipey {
	namespace thread {
		namespace pool {

			template <typename T>
			class CContextAwareThreadPool : public IBaseThreadPool<T, CONTEXT_JOB_INFO<T> >,
											public ::pipey::memory::IHandleManipulator< CONTEXT_INFO<T> >
			{
			public:
				CContextAwareThreadPool(void);
				~CContextAwareThreadPool(void);

			public:
				typedef ::pipey::memory::CObjectHandle< CONTEXT_INFO<T> > CContextHandle;
				typedef ::pipey::memory::CObjectHandle< CONTEXT_JOB_INFO<T> > CJobHandle;

			public:

				virtual void Init(uint32_t nMinThread = 0, uint32_t nMaxThread = 0, uint32_t nActiveThread = 0);
				virtual void Close();

				virtual void PushJob(const T &rJob, IJobCallback<T> *pCallback, ::pipey::memory::CObjectHandle< CONTEXT_JOB_INFO<T> > * pHandle = NULL);
				virtual void PushJob(::pipey::memory::CObjectHandle< CONTEXT_INFO<T> > & rContext, const T &rJob, IJobCallback<T> *pCallback, ::pipey::memory::CObjectHandle< CONTEXT_JOB_INFO<T> > * pHandle = NULL);

				void CreateContext(::pipey::memory::CObjectHandle< CONTEXT_INFO<T> > & rHandle);
				void CloseContext(::pipey::memory::CObjectHandle< CONTEXT_INFO<T> > & rHandle);

				virtual void CloseHandle(::pipey::memory::CObjectHandle< CONTEXT_INFO<T> > & rHandle);
				virtual void DuplicateHandle(const ::pipey::memory::CObjectHandle< CONTEXT_INFO<T> > & rSource, ::pipey::memory::CObjectHandle< CONTEXT_INFO<T> > & rTarget);

				virtual void CloseHandle(::pipey::memory::CObjectHandle< CONTEXT_JOB_INFO<T> > & rHandle);
				virtual void DuplicateHandle(const ::pipey::memory::CObjectHandle< CONTEXT_JOB_INFO<T> > & rSource, ::pipey::memory::CObjectHandle< CONTEXT_JOB_INFO<T> > & rTarget);

			private:
				::pipey::memory::CObjectHandle< CONTEXT_INFO<T> > m_hDefaultContext;
			};

			template <typename T>
			CContextAwareThreadPool<T>::CContextAwareThreadPool(void)
				:IBaseThreadPool<T, CONTEXT_JOB_INFO<T> >( new CContextAwareJobQueue<T>() )
			{	
			}

			template <typename T>
			CContextAwareThreadPool<T>::~CContextAwareThreadPool()
			{
				SAFE_TRY( Close() );
			}

			template <typename T>
			void CContextAwareThreadPool<T>::Init(uint32_t nMinThread, uint32_t nMaxThread, uint32_t nActiveThread)
			{
				IBaseThreadPool<T, CONTEXT_JOB_INFO<T> >::Init(nMinThread, nMaxThread, nActiveThread);

				CreateContext(m_hDefaultContext);
			}

			template <typename T>
			void CContextAwareThreadPool<T>::Close()
			{
				if( IBaseThreadPool<T, CONTEXT_JOB_INFO<T> >::IsInited() ) {
					CloseContext(m_hDefaultContext);
					CloseHandle(m_hDefaultContext);

					IBaseThreadPool<T, CONTEXT_JOB_INFO<T> >::Close();					
				} else throw ::pipey::common::exception::EInvalidState("::pipey::common::exception::EInvalidState => CContextAwareThreadPool::Close - This thread pool is not properly initiated.");
			}

			template <typename T>
			void CContextAwareThreadPool<T>::PushJob(const T &rJob, IJobCallback<T> *pCallback, ::pipey::memory::CObjectHandle< CONTEXT_JOB_INFO<T> > * pHandle)
			{
				PushJob(m_hDefaultContext, rJob, pCallback, pHandle);
			}

			template <typename T>
			void CContextAwareThreadPool<T>::PushJob(pipey::memory::CObjectHandle<CONTEXT_INFO<T> > &rContext, const T &rJob, pipey::thread::pool::IJobCallback<T> *pCallback, pipey::memory::CObjectHandle<CONTEXT_JOB_INFO<T> > *pHandle)
			{
				if( IBaseThreadPool<T, CONTEXT_JOB_INFO<T> >::IsInited() ) {
					::pipey::thread::sync::CLockPtr lockPtr(IBaseThreadPool<T, CONTEXT_JOB_INFO<T> >::GetInternalLock());
					lockPtr.AcquireLock();

					if( rContext ) {
						if( pipey::memory::IHandleManipulator< CONTEXT_INFO<T> >::IsMine(rContext) ) {
							CONTEXT_INFO<T> *pContext = pipey::memory::IHandleManipulator< CONTEXT_INFO<T> >::GetHandleTarget(rContext);

							if( pContext->bValid )
								IBaseThreadPool<T, CONTEXT_JOB_INFO<T> >::PushJob(CONTEXT_JOB_INFO<T>(rJob, pCallback, pContext), pHandle);
							else throw ::pipey::common::exception::EInvalidState("::pipey::common::exception::EInvalidState => CContextAwareThreadPool::PushJob - The context is already closed.");
							
						} else throw ::pipey::common::exception::EInvalidParameter("::pipey::common::exception::EInvalidParameter => CContextAwareThreadPool::PushJob - Specified context handle is owned by another thread pool.");
					} else throw ::pipey::common::exception::EInvalidParameter("::pipey::common::exception::EInvalidParameter => CContextAwareThreadPool::PushJob - Specified context handle is not properly initiated.");
				} else throw ::pipey::common::exception::EInvalidState("::pipey::common::exception::EInvalidState => CContextAwareThreadPool::PushJob - This thread pool is not properly initiated.");
			}

			template <typename T>
			void CContextAwareThreadPool<T>::CreateContext(pipey::memory::CObjectHandle<CONTEXT_INFO<T> > &rHandle)
			{
				if( IBaseThreadPool<T, CONTEXT_JOB_INFO<T> >::IsInited() ) {
					if( rHandle )
						throw ::pipey::common::exception::EInvalidParameter("::pipey::common::exception::EInvalidParameter => CContextAwareThreadPool<T>::CreateContext - A context handle specified is already initiated.");
					::pipey::thread::sync::CLockPtr lockPtr(IBaseThreadPool<T, CONTEXT_JOB_INFO<T> >::GetInternalLock());
					lockPtr.AcquireLock();

					CContextAwareJobQueue<T> *pQueue = (CContextAwareJobQueue<T> *)IBaseThreadPool<T, CONTEXT_JOB_INFO<T> >::GetJobQueue();
					CONTEXT_INFO<T> *pContext = pQueue->CreateContext();
					pContext->nHandle = 1;
					pipey::memory::IHandleManipulator< CONTEXT_INFO<T> >::InitHandle(rHandle, pContext);

				} else throw ::pipey::common::exception::EInvalidState("::pipey::common::exception::EInvalidState => CContextAwareThreadPool<T>::CreateContext - This thread pool is not properly initiated.");
			}

			template <typename T>
			void CContextAwareThreadPool<T>::CloseContext(::pipey::memory::CObjectHandle< CONTEXT_INFO<T> > &rHandle)
			{
				if( IBaseThreadPool<T, CONTEXT_JOB_INFO<T> >::IsInited() ) {
					::pipey::thread::sync::CLockPtr lockPtr(IBaseThreadPool<T, CONTEXT_JOB_INFO<T> >::GetInternalLock());
					lockPtr.AcquireLock();

					if( rHandle ) {
						if( pipey::memory::IHandleManipulator< CONTEXT_INFO<T> >::IsMine(rHandle) ) {
							CONTEXT_INFO<T> *pContext = pipey::memory::IHandleManipulator< CONTEXT_INFO<T> >::GetHandleTarget(rHandle);
							CContextAwareJobQueue<T> *pQueue = (CContextAwareJobQueue<T> *)IBaseThreadPool<T, CONTEXT_JOB_INFO<T> >::GetJobQueue();
							pQueue->CleanupContext(pContext);
						} else throw ::pipey::common::exception::EInvalidParameter("::pipey::common::exception::EInvalidParameter => CContextAwareThreadPool::CloseContext - Specified context handle is owned by another thread pool.");
					} else throw ::pipey::common::exception::EInvalidParameter("::pipey::common::exception::EInvalidParameter => CContextAwareThreadPool::CloseContext - Specified context handle is not properly initiated.");
				} else throw ::pipey::common::exception::EInvalidState("::pipey::common::exception::EInvalidState => CContextAwareThreadPool::CloseContext - This thread pool is not properly initiated.");
			}

			template <typename T>
			void CContextAwareThreadPool<T>::CloseHandle(::pipey::memory::CObjectHandle< CONTEXT_INFO<T> > &rHandle)
			{
				if( IBaseThreadPool<T, CONTEXT_JOB_INFO<T> >::IsInited() ) {
					::pipey::thread::sync::CLockPtr lockPtr(IBaseThreadPool<T, CONTEXT_JOB_INFO<T> >::GetInternalLock());
					lockPtr.AcquireLock();

					if( rHandle ) {
						if( pipey::memory::IHandleManipulator< CONTEXT_INFO<T> >::IsMine(rHandle) ) {
							CONTEXT_INFO<T> *pContext = pipey::memory::IHandleManipulator< CONTEXT_INFO<T> >::GetHandleTarget(rHandle);
							pContext->nHandle--;
							if( pContext->nHandle == 0 ) {
								CContextAwareJobQueue<T> *pQueue = (CContextAwareJobQueue<T> *)IBaseThreadPool<T, CONTEXT_JOB_INFO<T> >::GetJobQueue();
								pQueue->CleanupContext(pContext);
							}
							pipey::memory::IHandleManipulator< CONTEXT_INFO<T> >::NullifyHandle(rHandle);

						} else throw ::pipey::common::exception::EInvalidParameter("::pipey::common::exception::EInvalidParameter => CContextAwareThreadPool::CloseHandle - Specified context handle is owned by another thread pool.");
					} else throw ::pipey::common::exception::EInvalidParameter("::pipey::common::exception::EInvalidParameter => CContextAwareThreadPool::CloseHandle - Specified context handle is not properly initiated.");
				} else throw ::pipey::common::exception::EInvalidState("::pipey::common::exception::EInvalidState => CContextAwareThreadPool::CloseHandle - This thread pool is not properly initiated.");
			}

			template <typename T>
			void CContextAwareThreadPool<T>::DuplicateHandle(::pipey::memory::CObjectHandle< CONTEXT_INFO<T> > const &rSource, ::pipey::memory::CObjectHandle< CONTEXT_INFO<T> > &rTarget)
			{
				if( IBaseThreadPool<T, CONTEXT_JOB_INFO<T> >::IsInited() ) {
					if( rSource ) {
						if( rTarget ) throw ::pipey::common::exception::EInvalidParameter("::pipey::common::exception::EInvalidParameter => CContextAwareThreadPool::CloseHandle - Specified context handle(rTarget) is already initiated.");

						if( pipey::memory::IHandleManipulator< CONTEXT_INFO<T> >::IsMine(rSource) ) {
							::pipey::thread::sync::CLockPtr lockPtr(IBaseThreadPool<T, CONTEXT_JOB_INFO<T> >::GetInternalLock());
							lockPtr.AcquireLock();

							CONTEXT_INFO<T> *pContext = pipey::memory::IHandleManipulator< CONTEXT_INFO<T> >::GetHandleTarget(rSource);
							pipey::memory::IHandleManipulator< CONTEXT_INFO<T> >::InitHandle(rTarget, pContext);
							pContext->nHandle++;
						} else throw ::pipey::common::exception::EInvalidParameter("::pipey::common::exception::EInvalidParameter => CContextAwareThreadPool::DuplicateHandle - Specified context handle(rSource) is owned by other thread pool.");
					} else throw ::pipey::common::exception::EInvalidParameter("::pipey::common::exception::EInvalidParameter => CContextAwareThreadPool::DuplicateHandle - Specified context handle(rSource) is not properly initiated.");
				} else throw ::pipey::common::exception::EInvalidState("::pipey::common::exception::EInvalidState => CContextAwareThreadPool::DuplicateHandle - This thread pool is not properly initiated.");
			}

			template <typename T>
			void CContextAwareThreadPool<T>::CloseHandle(pipey::memory::CObjectHandle<CONTEXT_JOB_INFO<T> > &rHandle)
			{
				IBaseThreadPool<T, CONTEXT_JOB_INFO<T> >::CloseHandle(rHandle);
			}

			template <typename T>
			void CContextAwareThreadPool<T>::DuplicateHandle(const pipey::memory::CObjectHandle<CONTEXT_JOB_INFO<T> > &rSource, pipey::memory::CObjectHandle<CONTEXT_JOB_INFO<T> > &rTarget)
			{
				IBaseThreadPool<T, CONTEXT_JOB_INFO<T> >::DuplicateHandle(rSource, rTarget);
			}

		}
	}
}

#endif