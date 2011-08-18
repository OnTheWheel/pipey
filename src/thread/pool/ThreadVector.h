#ifndef PIPEY_THREADVECTOR_H
#define PIPEY_THREADVECTOR_H

#include <vector>
#include "../DefaultThread.h"
#include "../sync/DefaultCondition.h"

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


			public:
				void AddThread(unsigned long nCount, pipey::thread::IExecutable & rExecutable, void * pParam);

				bool IsThreadReady();

				void Close();

			};

		}
	}
}
#endif
