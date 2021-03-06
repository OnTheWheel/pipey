#ifndef PIPEY_BASETHREADPOOL_H
#define PIPEY_BASETHREADPOOL_H

#include "JobQueue.h"
#include "ThreadVector.h"
#include "../sync/MutableSemaphore.h"
#include "../sync/DefaultLock.h"
#include "../sync/DefaultCondition.h"
#include "ThreadPool.h"
#include "../../util/SystemInfo.h"
#include "job_info.h"

namespace pipey {
	namespace thread {
		namespace pool {

			template <typename T, typename INFO >
			class IBaseThreadPool : public IThreadPool<T, INFO> 
			{
			public:
				virtual ~IBaseThreadPool(void );

			protected:
				IBaseThreadPool(IJobQueue<T, INFO> * pQueue);


			private:
				IJobQueue<T, INFO> * m_pQueue;

				CThreadVector m_threads;

				::pipey::thread::sync::CMutableSemaphore m_semaphore;

				::pipey::thread::sync::CDefaultLock m_lock;

				::pipey::thread::sync::CDefaultCondition m_condition;

			private:
				struct DATA {
					DATA() :
        				m_bInited(false),
        				m_bExit(false),
        				m_nActiveThread(0),
        				m_nMaxThread(0),
        				m_nMinThread(0)
        			{ };

					bool m_bInited;

					bool m_bExit;

					uint32_t m_nMinThread;

					uint32_t m_nMaxThread;

					uint32_t m_nActiveThread;

				};

				DATA m_data;


			protected:
				virtual bool IsThreadReady();

				virtual IJobQueue<T, INFO> *GetJobQueue() const;

				virtual void PushJob(const INFO &rInfo, ::pipey::memory::CObjectHandle<INFO> * pHandle);

				virtual ::pipey::thread::sync::CDefaultLock *GetInternalLock();

			public:
				virtual void Init(uint32_t nMinThread = 0, uint32_t nMaxThread = 0, uint32_t nActiveThread = 0);

				virtual bool IsInited() const;

				//virtual bool AdjustThreadParam(uint32_t nMinThread = 0, uint32_t nMaxThread = 0, uint32_t nActiveThread = 0) = 0;
				virtual void PushJob(const T &rJob, IJobCallback<T> *pCallback, ::pipey::memory::CObjectHandle<INFO> * pHandle = NULL) = 0;

				virtual bool PopAndProcess();

				virtual void Close();

				virtual void CancelJob(const ::pipey::memory::CObjectHandle<INFO> & rHandle);
				virtual JOB_STATE GetJobState(const ::pipey::memory::CObjectHandle<INFO> &rHandle) const;

				virtual void CloseHandle(::pipey::memory::CObjectHandle<INFO> &rHandle);

				virtual void DuplicateHandle(const ::pipey::memory::CObjectHandle<INFO> &rSource, ::pipey::memory::CObjectHandle<INFO> &rTarget);

			};

			template <typename T, typename INFO>
			IBaseThreadPool<T,INFO>::IBaseThreadPool(IJobQueue<T, INFO> *pQueue) :
				m_pQueue(pQueue),
				m_threads()
			{
				if( pQueue == NULL )
					throw ::pipey::common::exception::EInvalidParameter("EInvalidParameter => IBaseThreadPool<T,INFO>::IBaseThreadPool - pQueue cannot be NULL.");
			}

			template <typename T, typename INFO>
			IBaseThreadPool<T,INFO>::~IBaseThreadPool(void)
			{
				SAFE_TRY( Close() );
				delete m_pQueue;
			}

			template <typename T, typename INFO>
			bool IBaseThreadPool<T,INFO>::IsThreadReady()
			{
				return m_threads.IsThreadReady();
			}

			template <typename T, typename INFO>
			IJobQueue<T, INFO> *IBaseThreadPool<T,INFO>::GetJobQueue() const
			{
				if( m_data.m_bInited ) 
					return m_pQueue;
				else throw ::pipey::common::exception::EInvalidState("EInvalidState => IBaseThreadPool<T,INFO>::GetJobQueue - This thread pool is not properly initiated.");
			}

			template <typename T, typename INFO>
			void IBaseThreadPool<T,INFO>::Init(uint32_t nMinThread, uint32_t nMaxThread, uint32_t nActiveThread)
			{
				if( m_data.m_bInited ) 
					throw ::pipey::common::exception::EInvalidState("EInvalidState => IBaseThreadPool<T,INFO>::Init - This thread pool is already initiated.");

				if( nMinThread == 0 || nMaxThread == 0 || nActiveThread == 0 ) {
					nActiveThread = nMinThread = ::pipey::util::GetNumberOfProcessors();
					nMaxThread = (uint32_t)(nActiveThread * 2.5);
					cout<<nMaxThread<<endl;
				}
				
				if( nMinThread <= nMaxThread && nActiveThread <= nMaxThread ) {
					try {
						m_semaphore.Init(& ::pipey::thread::sync::MUT_SEM_INIT(nActiveThread, 0));
						m_lock.Init();

#if defined(WIN32) || defined(WIN64)
						m_condition.Init();
#elif defined(__linux__) || defined(__unix__)

#ifdef _POSIX_THREAD_PROCESS_SHARED
						m_condition.Init(& ::pipey::thread::sync::POSIX_COND_INIT(false, &m_lock));	
#else
						m_condition.Init(& ::pipey::thread::sync::POSIX_COND_INIT(&m_lock));	
#endif

#endif
						
						m_data.m_bInited = true;
						m_data.m_nActiveThread = nActiveThread;
						m_data.m_nMaxThread = nMaxThread;
						m_data.m_nMinThread = nMinThread;
						m_threads.AddThread(nMaxThread, IThreadPool<T, INFO>::m_executable, this);

					} catch(...) {
						DATA empty;
						m_data = empty;
						SAFE_TRY( m_semaphore.Close() );
						SAFE_TRY( m_lock.Close() );
						SAFE_TRY( m_condition.Close() );
						SAFE_TRY( m_threads.Close() );
						throw;
					}
				} else throw ::pipey::common::exception::EInvalidParameter("EInvalidParameter => IBaseThreadPool<T,INFO>::Init - You should meet the following condition ( nMinThread <= nMaxThread && nActiveThread <= nMaxThread ).");
			}

			template <typename T, typename INFO>
			bool IBaseThreadPool<T,INFO>::IsInited() const
			{
				return m_data.m_bInited;
			}
			
			template <typename T, typename INFO>
			void IBaseThreadPool<T,INFO>::PushJob(const INFO &rInfo, ::pipey::memory::CObjectHandle<INFO> * pHandle)
			{
				if( m_data.m_bInited ) {
					::pipey::thread::sync::CLockPtr lockPtr(&m_lock);
					lockPtr.AcquireLock();

					if( pHandle ) {
						if( pHandle->IsValid() )
							throw ::pipey::common::exception::EInvalidParameter("EInvalidParameter => IBaseThreadPool<T,INFO>::PushJob - A job handle specified is already initiated.");
					}

					INFO *pInfo = NULL;

					try {
						pInfo = m_pQueue->Push(rInfo);
						pInfo->eState = JOB_WAIT;

						if( pHandle ) {
							InitHandle(*pHandle, pInfo);
							pInfo->nHandle = 1;
						} 
						
						m_condition.Awake();
					} catch(...) {
						if( pInfo ) {
							SAFE_TRY( NullifyHandle(*pHandle) );
							pInfo->nHandle = 0;
							SAFE_TRY( m_pQueue->CleanupJob(pInfo) );	
						}
						throw;
					}
				} else throw ::pipey::common::exception::EInvalidState("EInvalidState => IBaseThreadPool<T,INFO>::PushJob - This thread pool is not properly initiated.");
			}

			template <typename T, typename INFO>
			bool IBaseThreadPool<T,INFO>::PopAndProcess()
			{
				if( m_data.m_bInited  ) {
					::pipey::thread::sync::CLockPtr ptrSem(&m_semaphore);
					ptrSem.AcquireLock();

					::pipey::thread::sync::CLockPtr lockPtr(&m_lock);
					lockPtr.AcquireLock();

					if( m_data.m_bExit ) 
						return false;

					while( ! m_pQueue->IsPopable() ) {
#if defined(WIN32) || defined(WIN64)
						lockPtr.ReleaseLock();
#endif
						
						m_condition.Wait();

						if(m_data.m_bExit) {
							m_condition.Awake();
							return false;
						}

#if defined(WIN32) || defined(WIN64)
						lockPtr.AcquireLock();
#endif
					}

					INFO *pInfo = m_pQueue->Pop();
					pInfo->eState = JOB_READY;
					lockPtr.ReleaseLock();

					try {
						pInfo->eState = JOB_PROCESSING;
						pInfo->pCallback->ProcessJob(pInfo->job);
					} catch(std::exception &e) {
						pInfo->pCallback->OnException(pInfo->job, e);
					} catch(...) {
						pInfo->pCallback->OnException(pInfo->job, ::pipey::common::exception::EPipeyException());
					}
					
					lockPtr.AcquireLock();
					m_pQueue->CleanupJob(pInfo);
					if( m_pQueue->IsPopable() )
						m_condition.Awake();

					return true;
				} else throw ::pipey::common::exception::EInvalidState("EInvalidState => IBaseThreadPool<T,INFO>::PopAndProcess - This thread pool is not properly initiated.");
			}

			template <typename T, typename INFO>
			void IBaseThreadPool<T,INFO>::CancelJob(const ::pipey::memory::CObjectHandle<INFO> & rHandle)
			{
				if( m_data.m_bInited  )
				{
					::pipey::thread::sync::CLockPtr lockPtr(&m_lock);
					lockPtr.AcquireLock();

					if( (!rHandle) || !IsMine(rHandle) )
						throw ::pipey::common::exception::EInvalidParameter("EInvalidParameter => IBaseThreadPool<T,INFO>::CancelJob - A job handle specified is not valid.");

					INFO *pInfo = GetHandleTarget(rHandle);
					if( pInfo->eState != JOB_WAIT )
						throw ::pipey::common::exception::EInvalidState("EInvalidState => IBaseThreadPool<T,INFO>::CancelJob - A job handle specified is not in JOB_WAIT state.");

					try {
						IJobCallback<T> *pCallback = pInfo->pCallback;
						pCallback->OnCancel(pInfo->job);
						m_pQueue->CleanupJob(pInfo, JOB_CANCEL);
						lockPtr.ReleaseLock();
					} catch(...) {
						throw;
					}
				}
				else throw ::pipey::common::exception::EInvalidState("EInvalidState => IBaseThreadPool<T,INFO>::CancelJob - This thread pool is not properly initiated.");
			}

			template <typename T, typename INFO>
			JOB_STATE IBaseThreadPool<T,INFO>::GetJobState(::pipey::memory::CObjectHandle<INFO> const &rHandle) const
			{
				if( (!rHandle) || !IsMine(rHandle) )
						throw ::pipey::common::exception::EInvalidParameter("EInvalidParameter => IBaseThreadPool<T,INFO>::GetJobState - A job handle specified is not valid.");
				return GetHandleTarget(rHandle)->eState;
			}

			template <typename T, typename INFO>
			void IBaseThreadPool<T,INFO>::Close()
			{
				if( m_data.m_bInited ) {
					::pipey::thread::sync::CLockPtr lockPtr(&m_lock);
					lockPtr.AcquireLock();

					SAFE_TRY( m_pQueue->CleanupAll() );

					m_data.m_bExit = true;
					SAFE_TRY( m_condition.Awake() );
					SAFE_TRY( lockPtr.ReleaseLock() );

					SAFE_TRY( m_threads.Close() );
					SAFE_TRY( m_condition.Close() );
					SAFE_TRY( m_semaphore.Close() );
					SAFE_TRY( m_lock.Close() );

					DATA empty;
					m_data = empty;
				}
				else throw ::pipey::common::exception::EInvalidState("EInvalidState => IBaseThreadPool::Close - This thread pool is not properly initiated.");
			}

			template <typename T, typename INFO>
			void IBaseThreadPool<T,INFO>::CloseHandle(::pipey::memory::CObjectHandle<INFO> &rHandle)
			{
				if( m_data.m_bInited )
				{
					::pipey::thread::sync::CLockPtr lockPtr(&m_lock);
					lockPtr.AcquireLock();

					if( rHandle )
					{
						if( IsMine(rHandle) )
						{
							INFO *pInfo = GetHandleTarget(rHandle);
							pInfo->nHandle--;
							if( pInfo->nHandle == 0 &&
								( pInfo->eState == JOB_CANCEL || pInfo->eState == JOB_TIMEOUT ||  pInfo->eState == JOB_INVALID ||  pInfo->eState == JOB_COMPLETE ) )
								m_pQueue->CleanupJob(pInfo, JOB_INVALID);
							NullifyHandle(rHandle);

						}
						else throw ::pipey::common::exception::EInvalidParameter("EInvalidParameter => IBaseThreadPool::CloseHandle - Specified job handle is owned by another thread pool.");
					}
					else throw ::pipey::common::exception::EInvalidParameter("EInvalidParameter => IBaseThreadPool::CloseHandle - Specified job handle is not properly initiated.");
				}
				else throw ::pipey::common::exception::EInvalidState("EInvalidState => IBaseThreadPool::CloseHandle - This thread pool is not properly initiated.");
			}

			template <typename T, typename INFO>
			void IBaseThreadPool<T,INFO>::DuplicateHandle(::pipey::memory::CObjectHandle<INFO> const &rSource, ::pipey::memory::CObjectHandle<INFO> &rTarget)
			{
				if( m_data.m_bInited ) {
					if( rSource ) {
						if( rTarget ) throw ::pipey::common::exception::EInvalidParameter("EInvalidParameter => IBaseThreadPool::CloseHandle - Specified job handle(rTarget) is already initiated.");

						if( IsMine(rSource) ) {
							::pipey::thread::sync::CLockPtr lockPtr(&m_lock);
							lockPtr.AcquireLock();
							INFO *pInfo = GetHandleTarget(rSource);
							InitHandle(rTarget, pInfo);
							pInfo->nHandle++;
						}
						else throw ::pipey::common::exception::EInvalidParameter("EInvalidParameter => IBaseThreadPool::CloseHandle - Specified job handle(rSource) is owned by other thread pool.");
					}
					else throw ::pipey::common::exception::EInvalidParameter("EInvalidParameter => IBaseThreadPool::CloseHandle - Specified job handle(rSource) is not properly initiated.");
				}
				else throw ::pipey::common::exception::EInvalidState("EInvalidState => IBaseThreadPool::CloseHandle - This thread pool is not properly initiated.");
			}

			template <typename T, typename INFO>
			::pipey::thread::sync::CDefaultLock *IBaseThreadPool<T,INFO>::GetInternalLock()
			{
				if( m_data.m_bInited ) {
					return &m_lock;
				}
				else throw ::pipey::common::exception::EInvalidState("EInvalidState => IBaseThreadPool::GetInternalLock - This thread pool is not properly initiated.");
			}

		}
	}
}

#endif
