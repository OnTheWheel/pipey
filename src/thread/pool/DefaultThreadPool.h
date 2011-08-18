#ifndef PIPEY_DEFAULTTHREADPOOL_H
#define PIPEY_DEFAULTTHREADPOOL_H


#include "JobQueue.h"
#include "ThreadVector.h"
#include "../sync/MutableSemaphore.h"
#include "../sync/DefaultLock.h"
#include "../sync/DefaultCondition.h"
#include "ThreadPool.h"


namespace pipey {
	namespace thread {
		namespace pool {

			template <typename T, typename NODE = JOB_NODE<T>>//, template <typename JOB> class JOB_NODE>
			//template<typename T, typename JOB_NODE<T>>
			class CDefaultThreadPool : public IThreadPool<T, NODE> {
			  public:
				virtual ~CDefaultThreadPool(void );

				CDefaultThreadPool() {};

				CDefaultThreadPool(IJobQueue<T> * pQueue);


			  private:
				//CDefaultThreadPool(IJobQueue<T> *pQueue);
				IJobQueue<T> * m_pQueue;

				CThreadVector m_threads;

				pipey::thread::sync::CMutableSemaphore m_semaphore;

				pipey::thread::sync::CDefaultLock m_lock;

				pipey::thread::sync::CDefaultCondition m_condition;

				CThreadPoolExecutable m_executable;


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

				virtual IJobQueue<T> * GetJobQueue() const;


			  public:
				virtual void Init(unsigned long nMinThread = 0, unsigned long nMaxThread = 0, unsigned long nActiveThread = 0);

				virtual bool AdjustThreadParam(unsigned long nMinThread = 0, unsigned long nMaxThread = 0, unsigned long nActiveThread = 0) = 0;
				virtual void PushJob(const T &rJob, IJobCallback<T> *pCallback, pipey::memory::CObjectHandle<NODE> * pHandle);

				virtual bool PopAndProcess();

				virtual void CancelJob(const pipey::memory::CObjectHandle<NODE> & rHandle);

				virtual void CloseHandle(pipey::memory::CObjectHandle<NODE> &rHandle);

				virtual void DuplicateHandle(const pipey::memory::CObjectHandle<NODE> &rSource, pipey::memory::CObjectHandle<NODE> &rTarget);

			};

			#include "../../util/SystemInfo.h"

			template <typename T, typename NODE>
			CDefaultThreadPool<T,NODE>::CDefaultThreadPool(IJobQueue<T> *pQueue) :
			m_pQueue(pQueue),
			m_threads()
			{
				if( pQueue == NULL )
					throw pipey::common::exception::EInvalidParameter("pipey::common::exception::EInvalidParameter => CDefaultThreadPool<T,NODE>::CDefaultThreadPool - pQueue cannot be NULL.");
			}

			template <typename T, typename NODE>
			CDefaultThreadPool<T,NODE>::~CDefaultThreadPool(void)
			{
				SAFE_TRY( Close() );
			}

			template <typename T, typename NODE>
			bool CDefaultThreadPool<T,NODE>::IsThreadReady()
			{
				return m_threads.IsThreadReady();
			}

			template <typename T, typename NODE>
			IJobQueue<T> *CDefaultThreadPool<T,NODE>::GetJobQueue() const
			{
				if( m_data.m_bInited  )
				{
					return m_pQueue;
				}
				else throw pipey::common::exception::EInvalidState("pipey::common::exception::EInvalidState => CDefaultThreadPool<T,NODE>::GetJobQueue - This thread pool is not properly initiated.");
			}

			template <typename T, typename NODE>
			void CDefaultThreadPool<T,NODE>::Init(unsigned long nMinThread, unsigned long nMaxThread, unsigned long nActiveThread)
			{
				if( m_data.m_bInited ) 
					throw pipey::common::exception::EInvalidState("pipey::common::exception::EInvalidState => CDefaultThreadPool<T,NODE>::Init - This thread pool is not already initiated.");

				if( nMinThread == 0 || nMaxThread == 0 || nActiveThread == 0 )
				{
					nActiveThread = nMinThread = GetNumberOfProcessors();
					nMaxThread = (unsigned long)(nActiveThread * 2.5);
				}
				
				if( nMinThread <= nMaxThread && nActiveThread <= nMaxThread )
				{
					try
					{
						m_semaphore.Init(& MUT_SEM_INIT(nActiveThread, 0));
						m_lock.Init();
						m_condition.Init();
						
						m_threads.AddThread(nMaxThread, m_executable, this);
						
						m_data.m_nActiveThread = nActiveThread;
						m_data.m_nMaxThread = nMaxThread;
						m_data.m_nMinThread = nMinThread;
						m_data.m_bInited = true;
					}
					catch(...)
					{
						SAFE_TRY( m_semaphore.Close() );
						SAFE_TRY( m_lock.Close() );
						SAFE_TRY( m_condition.Close() );
						SAFE_TRY( m_threads.Close() );
						throw;
					}
				}
				else throw pipey::common::exception::EInvalidParameter("pipey::common::exception::EInvalidParameter => CDefaultThreadPool<T,NODE>::Init - You should meet the following condition ( nMinThread <= nMaxThread && nActiveThread <= nMaxThread ).");
			}
			
			template <typename T, typename NODE>
			void CDefaultThreadPool<T,NODE>::PushJob(const T &rJob, IJobCallback<T> *pCallback, ::pipey::memory::CObjectHandle<NODE> * pHandle)
			{
				if( m_data.m_bInited  )
				{
					CLockPtr lockPtr(&m_lock);
					lockPtr.AcquireLock();

					if( pHandle )
					{
						if( pHandle->IsValid() )
							throw pipey::common::exception::EInvalidParameter("pipey::common::exception::EInvalidParameter => CDefaultThreadPool<T,NODE>::PushJob - A job handle specified is already initiated.");
					}

					JOB_NODE<T> *pNode = NULL;

					try
					{
						pNode = m_pQueue->Push(rJob, pCallback);
						pNode->eState = JOB_WAIT;

						if( pHandle )
						{
							InitHandle(*pHandle, pNode);
							pNode->nHandle = 1;
						} 
						
						m_condition.Awake();
					}
					catch(...)
					{
						if( pNode )
						{
							SAFE_TRY( NullifyHandle(*pHandle) );
							pNode->nHandle = 0;
							SAFE_TRY( m_pQueue->CleanupJob(pNode) );	
						}
						throw;
					}
				}
				else throw pipey::common::exception::EInvalidState("pipey::common::exception::EInvalidState => CDefaultThreadPool<T,NODE>::PushJob - This thread pool is not properly initiated.");
			}

			template <typename T, typename NODE>
			bool CDefaultThreadPool<T,NODE>::PopAndProcess()
			{
				if( m_data.m_bInited  )
				{
					CLockPtr ptrSem(&m_semaphore);
					ptrSem.AcquireLock();

					CLockPtr lockPtr(&m_lock);
					lockPtr.AcquireLock();

					while( ! m_pQueue->IsPopable() )
					{
						lockPtr.ReleaseLock();
						m_condition.Wait();

						if(m_data.m_bExit)
						{
							m_condition.Awake();
							return false;
						}

						lockPtr.AcquireLock();
					}

					JOB_NODE<T> *pNode = m_pQueue->Pop();
					pNode->eState = JOB_READY;
					lockPtr.ReleaseLock();

					try
					{
						pNode->eState = JOB_PROCESSING;
						pNode->pCallback->ProcessJob(pNode->job);
					}
					catch(std::exception &e)
					{
						pNode->pCallback->OnException(pNode->job, e);
					}
					catch(...)
					{
						pNode->pCallback->OnException(pNode->job, EPipeyExecption());
					}
					
					lockPtr.AcquireLock();
					if( m_pQueue->CleanupJob(pNode) )   
						m_condition.Awake();

					return true;
				}
				else throw pipey::common::exception::EInvalidState("pipey::common::exception::EInvalidState => CDefaultThreadPool<T,NODE>::PopAndProcess - This thread pool is not properly initiated.");
			}

			template <typename T, typename NODE>
			void CDefaultThreadPool<T,NODE>::CancelJob(const ::pipey::memory::CObjectHandle<NODE> & rHandle)
			{
				if( m_data.m_bInited  )
				{
					if( (!rHandle) || !IsMine(rHandle) )
						throw pipey::common::exception::EInvalidParameter("pipey::common::exception::EInvalidParameter => CDefaultThreadPool<T,NODE>::CancelJob - A job handle specified is not valid.");

					CLockPtr lockPtr(&m_lock);
					lockPtr.AcquireLock();

					JOB_NODE<T> *pNode = GetHandleTarget(rHandle);
					if( pNode->eState != JOB_WAIT )
						throw pipey::common::exception::EInvalidState("pipey::common::exception::EInvalidState => CDefaultThreadPool<T,NODE>::CancelJob - A job handle specified is not in JOB_WAIT state.");

					try
					{
						m_pQueue->CleanupJob(pNode, JOB_CANCEL);
						pNode->pCallback->OnCancel(pNode->job);
					}
					catch(...)
					{
						throw;
					}
				}
				else throw pipey::common::exception::EInvalidState("pipey::common::exception::EInvalidState => CDefaultThreadPool<T,NODE>::CancelJob - This thread pool is not properly initiated.");
			}

			template <typename T, typename NODE>
			JOB_STATE CDefaultThreadPool<T,NODE>::GetJobState(::pipey::memory::CObjectHandle<NODE> const &rHandle) const
			{
				if( (!rHandle) || !IsMine(rHandle) )
						throw pipey::common::exception::EInvalidParameter("pipey::common::exception::EInvalidParameter => CDefaultThreadPool<T,NODE>::GetJobState - A job handle specified is not valid.");
				return GetHandleTarget(rHandle)->eState;
			}

			template <typename T, typename NODE>
			void CDefaultThreadPool<T,NODE>::Close()
			{
				if( m_data.m_bInited )
				{
					CLockPtr lockPtr(&m_lock);
					lockPtr.AcquireLock();

					SAFE_TRY( m_pQueue->Cleanup() );

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
				else throw pipey::common::exception::EInvalidState("pipey::common::exception::EInvalidState => CDefaultThreadPool::Close - This thread pool is not properly initiated.");
			}

			template <typename T, typename NODE>
			void CDefaultThreadPool<T,NODE>::CloseHandle(::pipey::memory::CObjectHandle<NODE> &rHandle)
			{
				if( m_data.m_bInited )
				{
					if( rHandle )
					{
						if( IsMine(rHandle) )
						{
							CLockPtr lockPtr(&m_lock);
							lockPtr.AcquireLock();
							JOB_NODE<T> *pNode = GetHandleTarget(rHandle);
							pNode->nHandle--;
							if( pNode->nHandle == 0 &&
								( pNode->eState == JOB_CANCEL || pNode->eState == JOB_TIMEOUT ||  pNode->eState == JOB_INVALID ) )
								m_pQueue->CleanupJob(pNode, JOB_INVALID);
							NullifyHandle(rHandle);

						}
						else throw pipey::common::exception::EInvalidParameter("pipey::common::exception::EInvalidParameter => CDefaultThreadPool::CloseHandle - Specified job handle is owned by another thread pool.");
					}
					else throw pipey::common::exception::EInvalidParameter("pipey::common::exception::EInvalidParameter => CDefaultThreadPool::CloseHandle - Specified job handle is not properly initiated.");
				}
				else throw pipey::common::exception::EInvalidState("pipey::common::exception::EInvalidState => CDefaultThreadPool::CloseHandle - This thread pool is not properly initiated.");
			}

			template <typename T, typename NODE>
			void CDefaultThreadPool<T,NODE>::DuplicateHandle(::pipey::memory::CObjectHandle<NODE> const &rSource, ::pipey::memory::CObjectHandle<NODE> &rTarget)
			{
				if( m_data.m_bInited )
				{
					if( rSource )
					{
						if( rTarget ) throw pipey::common::exception::EInvalidParameter("pipey::common::exception::EInvalidParameter => CDefaultThreadPool::CloseHandle - Specified job handle(rTarget) is already initiated.");

						if( IsMine(rSource) )
						{
							CLockPtr lockPtr(&m_lock);
							lockPtr.AcquireLock();
							JOB_NODE<T> *pNode = GetHandleTarget(rSource);
							InitHandle(rTarget, pNode);
							pNode->nHandle++;
						}
						else throw pipey::common::exception::EInvalidParameter("pipey::common::exception::EInvalidParameter => CDefaultThreadPool::CloseHandle - Specified job handle(rSource) is owned by other thread pool.");
					}
					else throw pipey::common::exception::EInvalidParameter("pipey::common::exception::EInvalidParameter => CDefaultThreadPool::CloseHandle - Specified job handle(rSource) is not properly initiated.");
				}
				else throw pipey::common::exception::EInvalidState("pipey::common::exception::EInvalidState => CDefaultThreadPool::CloseHandle - This thread pool is not properly initiated.");
			}

		}
	}
}

#endif