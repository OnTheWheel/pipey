#ifndef PIPEY_SIMPLEJOBQUEUE_H
#define PIPEY_SIMPLEJOBQUEUE_H

#include "JobQueue.h"
#include "job_info.h"

namespace pipey {
	namespace thread {
		namespace pool {

			template <typename T>
			class CSimpleJobQueue : public IJobQueue<T, JOB_INFO<T> >
			{
			public:
				CSimpleJobQueue();
				virtual ~CSimpleJobQueue();

			public:
				virtual bool IsEmpty();
				virtual bool IsPopable();

				virtual JOB_INFO<T>* Push(const T &job, IJobCallback<T> *pCallback);
				virtual JOB_INFO<T>* Pop();

				virtual bool CleanupJob(JOB_INFO<T> *pJobInfo, JOB_STATE eState = JOB_COMPLETE);

				virtual void CleanupAll();

			private:

				template <typename U>
				struct JOB_NODE : public JOB_INFO<U>
				{
					JOB_NODE(const U &job, IJobCallback<T> * pCallback)
					:JOB_INFO<U>(job, pCallback), pPrev(NULL), pNext(NULL)
					{
					}

					JOB_NODE<U> *pPrev;
					JOB_NODE<U> *pNext;
				};

				JOB_NODE<T>* m_pHead;
				JOB_NODE<T>* m_pTail;
			};

			template <typename T>
			CSimpleJobQueue<T>::CSimpleJobQueue()
			: m_pHead(NULL), m_pTail(NULL)
			{
			}

			template <typename T>
			CSimpleJobQueue<T>::~CSimpleJobQueue()
			{
				SAFE_TRY( CleanupAll() );
			}

			template <typename T>
			bool CSimpleJobQueue<T>::IsEmpty()
			{
				return m_pHead == NULL;
			}

			template <typename T>
			bool CSimpleJobQueue<T>::IsPopable()
			{
				return !IsEmpty();
			}

			template <typename T>
			JOB_INFO<T> *CSimpleJobQueue<T>::Push(const T &job, IJobCallback<T> *pCallback)
			{
				JOB_NODE<T> *pNew = new JOB_NODE<T>(job, pCallback);

				if( IsEmpty() )
					m_pHead = m_pTail = pNew;
				else {
					m_pHead->pPrev = pNew;
					pNew->pNext = m_pHead;
					m_pHead = pNew;
				}

				return m_pHead;
			}

			template <typename T>
			JOB_INFO<T> *CSimpleJobQueue<T>::Pop()
			{
				if( IsPopable() ) {
					JOB_NODE<T> *pPop = m_pTail;

					if( m_pHead == m_pTail )
						m_pHead = m_pTail = NULL;
					else {
						m_pTail->pPrev->pNext = NULL;
						m_pTail = m_pTail->pPrev;
					}

					pPop->pPrev = NULL;
					return pPop;
				} else throw ::pipey::common::exception::EOutOfBound("pipey::common::exception::EOutOfBound => CSimpleJobQueue<T>::Pop() - The queue is empty.");
			}

			template <typename T>
			bool CSimpleJobQueue<T>::CleanupJob(JOB_INFO<T> *pJobInfo, JOB_STATE eState)
			{
				if( pJobInfo->nHandle == 0 )
					delete pJobInfo;
				else if( eState == JOB_CANCEL || eState == JOB_TIMEOUT ) {
					JOB_NODE<T> *pNode = (JOB_NODE<T> *)pJobInfo;
					
					if( pNode->pPrev ) pNode->pPrev->pNext = pNode->pNext;
					if( pNode->pNext ) pNode->pNext->pPrev = pNode->pPrev;
					if( pNode == m_pHead ) m_pHead = pNode->pNext;
					if( pNode == m_pTail ) m_pTail = pNode->pPrev;

					pNode->pPrev = pNode->pNext = NULL;
					pNode->eState = eState;

				} else if( eState == JOB_COMPLETE )
					pJobInfo->eState = eState;

				return false;
			}

			template <typename T>
			void CSimpleJobQueue<T>::CleanupAll()
			{
				JOB_NODE<T> *pNext = m_pHead;
				while( pNext ) {
					JOB_NODE<T> *pNode = pNext;
					pNext = pNode->pNext;

					if( pNode->nHandle == 0 )
						delete pNode;
					else {
						pNode->eState = JOB_INVALID;
						pNode->pPrev = pNode->pNext = NULL;
					}
				}

				m_pHead = m_pTail = NULL;
			}

		}
	}
}

#endif
