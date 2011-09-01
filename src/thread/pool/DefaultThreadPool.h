#ifndef PIPEY_DEFAULTTHREADPOOL_H
#define PIPEY_DEFAULTTHREADPOOL_H

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
			class CDefaultThreadPool : public IThreadPool<T, INFO> {
			public:
				virtual ~CDefaultThreadPool(void );

			protected:
				CDefaultThreadPool(IJobQueue<T, INFO> * pQueue);


			private:
				IJobQueue<T, INFO> * m_pQueue;

				CThreadVector m_threads;

				::pipey::thread::sync::CMutableSemaphore m_semaphore;

				::pipey::thread::sync::CDefaultLock m_lock;

				::pipey::thread::sync::CDefaultCondition m_condition;

			public:
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

					unsigned long m_nMinThread;

					unsigned long m_nMaxThread;

					unsigned long m_nActiveThread;

				};
			    

			private:
				DATA m_data;


			protected:
				virtual bool IsThreadReady();

				virtual IJobQueue<T, INFO> *GetJobQueue() const;

			public:
				virtual void Init(unsigned long nMinThread = 0, unsigned long nMaxThread = 0, unsigned long nActiveThread = 0);

				//virtual bool AdjustThreadParam(unsigned long nMinThread = 0, unsigned long nMaxThread = 0, unsigned long nActiveThread = 0) = 0;
				virtual void PushJob(const T &rJob, IJobCallback<T> *pCallback, ::pipey::memory::CObjectHandle<INFO> * pHandle);

				virtual bool PopAndProcess();

				virtual void Close();

				virtual void CancelJob(const ::pipey::memory::CObjectHandle<INFO> & rHandle);
				virtual JOB_STATE GetJobState(const ::pipey::memory::CObjectHandle<INFO> &rHandle) const;

				virtual void CloseHandle(::pipey::memory::CObjectHandle<INFO> &rHandle);

				virtual void DuplicateHandle(const ::pipey::memory::CObjectHandle<INFO> &rSource, ::pipey::memory::CObjectHandle<INFO> &rTarget);

			};

			template <typename T, typename INFO>
			CDefaultThreadPool<T,INFO>::CDefaultThreadPool(IJobQueue<T, INFO> *pQueue) :
			m_pQueue(pQueue),
			m_threads()
			{
				if( pQueue == NULL )
					throw ::pipey::common::exception::EInvalidParameter("::pipey::common::exception::EInvalidParameter => CDefaultThreadPool<T,INFO>::CDefaultThreadPool - pQueue cannot be NULL.");
			}

			template <typename T, typename INFO>
			CDefaultThreadPool<T,INFO>::~CDefaultThreadPool(void)
			{
				SAFE_TRY( Close() );
			}

			template <typename T, typename INFO>
			bool CDefaultThreadPool<T,INFO>::IsThreadReady()
			{
				return m_threads.IsThreadReady();
			}

			template <typename T, typename INFO>
			IJobQueue<T, INFO> *CDefaultThreadPool<T,INFO>::GetJobQueue() const
			{
				if( m_data.m_bInited  )
				{
					return m_pQueue;
				}
				else throw ::pipey::common::exception::EInvalidState("::pipey::common::exception::EInvalidState => CDefaultThreadPool<T,INFO>::GetJobQueue - This thread pool is not properly initiated.");
			}

			template <typename T, typename INFO>
			void CDefaultThreadPool<T,INFO>::Init(unsigned long nMinThread, unsigned long nMaxThread, unsigned long nActiveThread)
			{
				if( m_data.m_bInited ) 
					throw ::pipey::common::exception::EInvalidState("::pipey::common::exception::EInvalidState => CDefaultThreadPool<T,INFO>::Init - This thread pool is not already initiated.");

				if( nMinThread == 0 || nMaxThread == 0 || nActiveThread == 0 )
				{
					nActiveThread = nMinThread = ::pipey::util::GetNumberOfProcessors();
					nMaxThread = (unsigned long)(nActiveThread * 2.5);
					cout<<nMaxThread<<endl;
				}
				
				if( nMinThread <= nMaxThread && nActiveThread <= nMaxThread )
				{
					try
					{
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

					}
					catch(...)
					{
						DATA empty;
						m_data = empty;
						SAFE_TRY( m_semaphore.Close() );
						SAFE_TRY( m_lock.Close() );
						SAFE_TRY( m_condition.Close() );
						SAFE_TRY( m_threads.Close() );
						throw;
					}
				}
				else throw ::pipey::common::exception::EInvalidParameter("::pipey::common::exception::EInvalidParameter => CDefaultThreadPool<T,INFO>::Init - You should meet the following condition ( nMinThread <= nMaxThread && nActiveThread <= nMaxThread ).");
			}
			
			template <typename T, typename INFO>
			void CDefaultThreadPool<T,INFO>::PushJob(const T &rJob, IJobCallback<T> *pCallback, ::pipey::memory::CObjectHandle<INFO> * pHandle)
			{
				if( m_data.m_bInited  )
				{
					::pipey::thread::sync::CLockPtr lockPtr(&m_lock);
					lockPtr.AcquireLock();

					if( pHandle )
					{
						if( pHandle->IsValid() )
							throw ::pipey::common::exception::EInvalidParameter("::pipey::common::exception::EInvalidParameter => CDefaultThreadPool<T,INFO>::PushJob - A job handle specified is already initiated.");
					}

					INFO *pInfo = NULL;

					try
					{
						pInfo = m_pQueue->Push(rJob, pCallback);
						pInfo->eState = JOB_WAIT;

						if( pHandle )
						{
							InitHandle(*pHandle, pInfo);
							pInfo->nHandle = 1;
						} 
						
						m_condition.Awake();
					}
					catch(...)
					{
						if( pInfo )
						{
							SAFE_TRY( NullifyHandle(*pHandle) );
							pInfo->nHandle = 0;
							SAFE_TRY( m_pQueue->CleanupJob(pInfo) );	
						}
						throw;
					}
				}
				else throw ::pipey::common::exception::EInvalidState("::pipey::common::exception::EInvalidState => CDefaultThreadPool<T,INFO>::PushJob - This thread pool is not properly initiated.");
			}

			template <typename T, typename INFO>
			bool CDefaultThreadPool<T,INFO>::PopAndProcess()
			{
				if( m_data.m_bInited  )
				{
					::pipey::thread::sync::CLockPtr ptrSem(&m_semaphore);
					ptrSem.AcquireLock();

					::pipey::thread::sync::CLockPtr lockPtr(&m_lock);
					lockPtr.AcquireLock();

					if( m_data.m_bExit ) 
						return false;

					while( ! m_pQueue->IsPopable() )
					{
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

					JOB_INFO<T> *pInfo = m_pQueue->Pop();
					pInfo->eState = JOB_READY;
					lockPtr.ReleaseLock();

					try
					{
						pInfo->eState = JOB_PROCESSING;
						pInfo->pCallback->ProcessJob(pInfo->job);
					}
					catch(std::exception &e)
					{
						pInfo->pCallback->OnException(pInfo->job, e);
					}
					catch(...)
					{
						pInfo->pCallback->OnException(pInfo->job, ::pipey::common::exception::EPipeyException());
					}
					
					lockPtr.AcquireLock();
					if( m_pQueue->CleanupJob(pInfo) )   
						m_condition.Awake();

					return true;
				}
				else throw ::pipey::common::exception::EInvalidState("::pipey::common::exception::EInvalidState => CDefaultThreadPool<T,INFO>::PopAndProcess - This thread pool is not properly initiated.");
			}

			template <typename T, typename INFO>
			void CDefaultThreadPool<T,INFO>::CancelJob(const ::pipey::memory::CObjectHandle<INFO> & rHandle)
			{
				if( m_data.m_bInited  )
				{
					::pipey::thread::sync::CLockPtr lockPtr(&m_lock);
					lockPtr.AcquireLock();

					if( (!rHandle) || !IsMine(rHandle) )
						throw ::pipey::common::exception::EInvalidParameter("::pipey::common::exception::EInvalidParameter => CDefaultThreadPool<T,INFO>::CancelJob - A job handle specified is not valid.");

					JOB_INFO<T> *pInfo = GetHandleTarget(rHandle);
					if( pInfo->eState != JOB_WAIT )
						throw ::pipey::common::exception::EInvalidState("::pipey::common::exception::EInvalidState => CDefaultThreadPool<T,INFO>::CancelJob - A job handle specified is not in JOB_WAIT state.");

					try
					{
						IJobCallback<T> *pCallback = pInfo->pCallback;
						T job = pInfo->job;
						m_pQueue->CleanupJob(pInfo, JOB_CANCEL);
						lockPtr.ReleaseLock();

						pCallback->OnCancel(job);
					}
					catch(...)
					{
						throw;
					}
				}
				else throw ::pipey::common::exception::EInvalidState("::pipey::common::exception::EInvalidState => CDefaultThreadPool<T,INFO>::CancelJob - This thread pool is not properly initiated.");
			}

			template <typename T, typename INFO>
			JOB_STATE CDefaultThreadPool<T,INFO>::GetJobState(::pipey::memory::CObjectHandle<INFO> const &rHandle) const
			{
				if( (!rHandle) || !IsMine(rHandle) )
						throw ::pipey::common::exception::EInvalidParameter("::pipey::common::exception::EInvalidParameter => CDefaultThreadPool<T,INFO>::GetJobState - A job handle specified is not valid.");
				return GetHandleTarget(rHandle)->eState;
			}

			template <typename T, typename INFO>
			void CDefaultThreadPool<T,INFO>::Close()
			{
				if( m_data.m_bInited )
				{
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
				else throw ::pipey::common::exception::EInvalidState("::pipey::common::exception::EInvalidState => CDefaultThreadPool::Close - This thread pool is not properly initiated.");
			}

			template <typename T, typename INFO>
			void CDefaultThreadPool<T,INFO>::CloseHandle(::pipey::memory::CObjectHandle<INFO> &rHandle)
			{
				if( m_data.m_bInited )
				{
					::pipey::thread::sync::CLockPtr lockPtr(&m_lock);
					lockPtr.AcquireLock();

					if( rHandle )
					{
						if( IsMine(rHandle) )
						{
							JOB_INFO<T> *pInfo = GetHandleTarget(rHandle);
							pInfo->nHandle--;
							if( pInfo->nHandle == 0 &&
								( pInfo->eState == JOB_CANCEL || pInfo->eState == JOB_TIMEOUT ||  pInfo->eState == JOB_INVALID ||  pInfo->eState == JOB_COMPLETE ) )
								m_pQueue->CleanupJob(pInfo, JOB_INVALID);
							NullifyHandle(rHandle);

						}
						else throw ::pipey::common::exception::EInvalidParameter("::pipey::common::exception::EInvalidParameter => CDefaultThreadPool::CloseHandle - Specified job handle is owned by another thread pool.");
					}
					else throw ::pipey::common::exception::EInvalidParameter("::pipey::common::exception::EInvalidParameter => CDefaultThreadPool::CloseHandle - Specified job handle is not properly initiated.");
				}
				else throw ::pipey::common::exception::EInvalidState("::pipey::common::exception::EInvalidState => CDefaultThreadPool::CloseHandle - This thread pool is not properly initiated.");
			}

			template <typename T, typename INFO>
			void CDefaultThreadPool<T,INFO>::DuplicateHandle(::pipey::memory::CObjectHandle<INFO> const &rSource, ::pipey::memory::CObjectHandle<INFO> &rTarget)
			{
				if( m_data.m_bInited )
				{
					if( rSource )
					{
						if( rTarget ) throw ::pipey::common::exception::EInvalidParameter("::pipey::common::exception::EInvalidParameter => CDefaultThreadPool::CloseHandle - Specified job handle(rTarget) is already initiated.");

						if( IsMine(rSource) )
						{
							::pipey::thread::sync::CLockPtr lockPtr(&m_lock);
							lockPtr.AcquireLock();
							JOB_INFO<T> *pInfo = GetHandleTarget(rSource);
							InitHandle(rTarget, pInfo);
							pInfo->nHandle++;
						}
						else throw ::pipey::common::exception::EInvalidParameter("::pipey::common::exception::EInvalidParameter => CDefaultThreadPool::CloseHandle - Specified job handle(rSource) is owned by other thread pool.");
					}
					else throw ::pipey::common::exception::EInvalidParameter("::pipey::common::exception::EInvalidParameter => CDefaultThreadPool::CloseHandle - Specified job handle(rSource) is not properly initiated.");
				}
				else throw ::pipey::common::exception::EInvalidState("::pipey::common::exception::EInvalidState => CDefaultThreadPool::CloseHandle - This thread pool is not properly initiated.");
			}

		}
	}
}

#endif
