#ifndef PIPEY_CONTEXTAWAREJOBQUEUE_H
#define PIPEY_CONTEXTAWAREJOBQUEUE_H

#include "JobQueue.h"
#include "job_info.h"
#include "../../util/RandomPicker.h"
#include <list>

namespace pipey {
	namespace thread {
		namespace pool {

			template <typename T>
			class CContextAwareJobQueue : public IJobQueue<T, CONTEXT_JOB_INFO<T> >
			{
			public:
				CContextAwareJobQueue();
				virtual ~CContextAwareJobQueue();

			public:
				virtual bool IsEmpty() const;
				virtual bool IsPopable() const;

				virtual CONTEXT_JOB_INFO<T>* Push(const CONTEXT_JOB_INFO<T> &rInfo);
				virtual CONTEXT_JOB_INFO<T>* Pop();

				virtual void CleanupJob(CONTEXT_JOB_INFO<T> *pJobInfo, JOB_STATE eState = JOB_COMPLETE);

				CONTEXT_INFO<T>* CreateContext();
				
				void CleanupContext(CONTEXT_INFO<T> *pContext);

				virtual void CleanupAll();

			private:

				::pipey::util::CRandomPicker< CONTEXT_INFO<T>* > m_runnableContexts;
				::std::list< CONTEXT_INFO<T>* > m_contexts;
			};

			template <typename T>
			CContextAwareJobQueue<T>::CContextAwareJobQueue()
			{
			}

			template <typename T>
			CContextAwareJobQueue<T>::~CContextAwareJobQueue()
			{
				SAFE_TRY(CleanupAll());
			}

			template <typename T>
			bool CContextAwareJobQueue<T>::IsEmpty() const
			{
				if( !m_runnableContexts.IsEmpty() ) return false;

				typename ::std::list< CONTEXT_INFO<T>* >::const_iterator iter = m_contexts.begin();
				while( iter != m_contexts.end() ) {
					
					if( !(*iter)->queue.IsEmpty() ) return false;
					iter++;
				}

				return true;
			}

			template <typename T>
			bool CContextAwareJobQueue<T>::IsPopable() const
			{
				return !(m_runnableContexts.IsEmpty());
			}

			template <typename T>
			CONTEXT_JOB_INFO<T>* CContextAwareJobQueue<T>::Push(const CONTEXT_JOB_INFO<T> &rInfo)
			{
				CONTEXT_INFO<T> *pContext = rInfo.pContext;
				if( pContext ) {
					CONTEXT_JOB_INFO<T>* pResult = pContext->queue.Push(rInfo);

					if( pContext->nPickerReference == ::pipey::common::INVALID && !pContext->pRunningJob )
						m_runnableContexts.Put(pContext, &pContext->nPickerReference);

					return pResult;
				} else throw ::pipey::common::exception::EInvalidParameter("pipey::common::exception::EInvalidParameter => CContextAwareJobQueue<T>::Push() - context cannot be NULL.");
			}

			template <typename T>
			CONTEXT_JOB_INFO<T>* CContextAwareJobQueue<T>::Pop()
			{
				if( IsPopable() ) {
					CONTEXT_INFO<T> *pContext = m_runnableContexts.Pick();
					m_runnableContexts.Delete(pContext->nPickerReference);
					pContext->pRunningJob = pContext->queue.Pop();
					
					return pContext->pRunningJob;
				} else throw ::pipey::common::exception::EOutOfBound("pipey::common::exception::EOutOfBound => CContextAwareJobQueue<T, INFO>::Pop() - The queue is not popable.");
			}

			template <typename T>
			void CContextAwareJobQueue<T>::CleanupJob(CONTEXT_JOB_INFO<T> *pJobInfo, JOB_STATE eState)
			{
				CONTEXT_INFO<T> *pContext = pJobInfo->pContext;
				if( pContext ) {
					pJobInfo->pContext = NULL;
					pContext->queue.CleanupJob(pJobInfo, eState);
					if( eState == JOB_COMPLETE ) {
						pContext->pRunningJob = NULL;
						if( pContext->queue.IsPopable() )
							m_runnableContexts.Put(pContext, &pContext->nPickerReference);
					} else if( eState == JOB_CANCEL && pContext->nPickerReference != ::pipey::common::INVALID && !pContext->queue.IsPopable() )
						m_runnableContexts.Delete(pContext->nPickerReference);
				} else {
					if( pJobInfo->nHandle == 0 )
						delete pJobInfo;
					else pJobInfo->eState = eState;
				}
			}

			template <typename T>
			CONTEXT_INFO<T>* CContextAwareJobQueue<T>::CreateContext()
			{
				CONTEXT_INFO<T>* pContext = new CONTEXT_INFO<T>();
				
				pContext->iter = m_contexts.insert(m_contexts.begin(), pContext);
				return pContext;
			}

			template <typename T>
			void CContextAwareJobQueue<T>::CleanupContext(pipey::thread::pool::CONTEXT_INFO<T> *pContext)
			{
				if( pContext ) {
					if( pContext->pRunningJob )
						pContext->pRunningJob->pContext = NULL;
					pContext->pRunningJob = NULL;

					while( pContext->queue.IsPopable() ) {
						CONTEXT_JOB_INFO<T> *pJobInfo = pContext->queue.Pop();
						pJobInfo->pCallback->OnCancel(pJobInfo->job);
						CleanupJob(pJobInfo, JOB_CANCEL);
					}
					if( pContext->bValid )
						m_contexts.erase(pContext->iter);
					if( pContext->nPickerReference != ::pipey::common::INVALID )
						m_runnableContexts.Delete(pContext->nPickerReference);

					pContext->bValid = false;

					if( pContext->nHandle == 0 )
						delete pContext;

				} else throw ::pipey::common::exception::EInvalidParameter("pipey::common::exception::EInvalidParameter => CContextAwareJobQueue<T>::CleanupContext() - context cannot be NULL.");
			}

			template <typename T>
			void CContextAwareJobQueue<T>::CleanupAll()
			{
				typename ::std::list< CONTEXT_INFO<T>* >::iterator iter = m_contexts.begin();
				while( iter != m_contexts.end() ) {
				
					CleanupContext(*iter);
					iter++;
				}
				m_contexts.clear();
			}

		}
	}
}

#endif
