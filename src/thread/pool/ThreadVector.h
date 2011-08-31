#ifndef PIPEY_THREADVECTOR_H
#define PIPEY_THREADVECTOR_H

#include <vector>
#include "../DefaultThread.h"
#include "../sync/DefaultCondition.h"
#include "../sync/DefaultLock.h"

namespace pipey {
	namespace thread {
		namespace pool {

			class CThreadVector : public std::vector<pipey::thread::CDefaultThread *> 
			{
			public:
				CThreadVector(void );

				virtual ~CThreadVector(void );


			private:
				bool m_bThreadReady;

				pipey::thread::sync::CDefaultCondition m_threadReady;

#if defined(__linux__) || defined(__unix__) 
				pipey::thread::sync::CDefaultLock m_lock;
				unsigned long m_nNewThread;
#endif		

			public:
				void AddThread(unsigned long nCount, pipey::thread::IExecutable & rExecutable, void * pParam);

				bool IsThreadReady();

				void Close();

			};

		}
	}
}
#endif
