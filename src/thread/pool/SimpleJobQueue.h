#ifndef PIPEY_SIMPLEJOBQUEUE_H
#define PIPEY_SIMPLEJOBQUEUE_H

#include "JobQueue.h"
#include "job_info.h"

namespace pipey {
	namespace thread {
		namespace pool {

			template <typename T, typename INFO = JOB_INFO<T> >
			class CSimpleJobQueue : public IJobQueue<T, INFO >
			{
			public:
				CSimpleJobQueue();
				virtual ~CSimpleJobQueue();

			public:
				virtual bool IsEmpty() const;
				virtual bool IsPopable() const;

				virtual INFO* Push(const INFO &rInfo);
				virtual INFO* Pop();

				virtual void CleanupJob(INFO *pJobInfo, JOB_STATE eState = JOB_COMPLETE);

				virtual void CleanupAll();

			private:

				//template <typename U>
				struct JOB_NODE : public INFO
				{
					JOB_NODE(const INFO &rInfo)
					:INFO(rInfo), pPrev(NULL), pNext(NULL)
					{
					}

					JOB_NODE *pPrev;
					JOB_NODE *pNext;
				};

				JOB_NODE* m_pHead;
				JOB_NODE* m_pTail;
			};

			template <typename T, typename INFO>
			CSimpleJobQueue<T, INFO>::CSimpleJobQueue()
			: m_pHead(NULL), m_pTail(NULL)
			{
			}

			template <typename T, typename INFO>
			CSimpleJobQueue<T, INFO>::~CSimpleJobQueue()
			{
				SAFE_TRY( CleanupAll() );
			}

			template <typename T, typename INFO>
			bool CSimpleJobQueue<T, INFO>::IsEmpty() const
			{
				return m_pHead == NULL;
			}

			template <typename T, typename INFO>
			bool CSimpleJobQueue<T, INFO>::IsPopable() const
			{
				return !IsEmpty();
			}

			template <typename T, typename INFO>
			INFO *CSimpleJobQueue<T, INFO>::Push(const INFO &rInfo)
			{
				JOB_NODE *pNew = new JOB_NODE(rInfo);

				if( IsEmpty() )
					m_pHead = m_pTail = pNew;
				else {
					m_pHead->pPrev = pNew;
					pNew->pNext = m_pHead;
					m_pHead = pNew;
				}

				return m_pHead;
			}

			template <typename T, typename INFO>
			INFO *CSimpleJobQueue<T, INFO>::Pop()
			{
				if( IsPopable() ) {
					JOB_NODE *pPop = m_pTail;

					if( m_pHead == m_pTail )
						m_pHead = m_pTail = NULL;
					else {
						m_pTail->pPrev->pNext = NULL;
						m_pTail = m_pTail->pPrev;
					}

					pPop->pPrev = NULL;
					return pPop;
				} else throw ::pipey::common::exception::EOutOfBound("pipey::common::exception::EOutOfBound => CSimpleJobQueue<T, INFO>::Pop() - The queue is empty.");
			}

			template <typename T, typename INFO>
			void CSimpleJobQueue<T, INFO>::CleanupJob(INFO *pJobInfo, JOB_STATE eState)
			{
				if( pJobInfo->nHandle == 0 )
					delete pJobInfo;
				else if( eState == JOB_CANCEL || eState == JOB_TIMEOUT ) {
					JOB_NODE *pNode = (JOB_NODE *)pJobInfo;
					
					if( pNode->pPrev ) pNode->pPrev->pNext = pNode->pNext;
					if( pNode->pNext ) pNode->pNext->pPrev = pNode->pPrev;
					if( pNode == m_pHead ) m_pHead = pNode->pNext;
					if( pNode == m_pTail ) m_pTail = pNode->pPrev;

					pNode->pPrev = pNode->pNext = NULL;
					pNode->eState = eState;

				} else if( eState == JOB_COMPLETE )
					pJobInfo->eState = eState;
			}

			template <typename T, typename INFO>
			void CSimpleJobQueue<T, INFO>::CleanupAll()
			{
				JOB_NODE *pNext = m_pHead;
				while( pNext ) {
					JOB_NODE *pNode = pNext;
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
