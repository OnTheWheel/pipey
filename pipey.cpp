// pipey.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//


#include "src/common/common.h"
#include "src/thread/DefaultThread.h"
#include "src/thread/sync/DefaultLock.h"
#include "src/thread/sync/WindowsCritricalSection.h"
#include <stdio.h>

using namespace pipey::common;

#if defined(__linux__) || defined(__unix__)
#include <unistd.h>
#endif

struct THREAD_TEST
{
	unsigned int id;
	pipey::thread::sync::CWindowsCritricalSection *pLock;
};

class CThreadTest : public pipey::thread::IExecutable
{
	void Execute(void *pParam)
	{
		THREAD_TEST *pId = (THREAD_TEST*) pParam;
		for(int i=0; i<5;i++) {
			SYNC_RESULT res = pId->pLock->AcquireLock();
			
			printf("thread %d acquired lock.\n", pId->id);
#if defined(WIN32) || defined(WIN64)
			::Sleep(1000);
#elif defined(__linux__) || defined(__unix__)
			sleep(1);
#endif
			pId->pLock->ReleaseLock();
		}
	}
};

int main(int argc, char* argv[])
{
	pipey::thread::sync::CWindowsCritricalSection lock;
	lock.Init();

	THREAD_TEST ids[5];
	pipey::thread::CDefaultThread thread[5];
	CThreadTest routine;

	for(unsigned int i=0; i<5; i++)
	{
		ids[i].id = i+1;
		ids[i].pLock = &lock;

		pipey::thread::THREAD_INIT init;
		init.pExec = &routine;
		init.pParam = &ids[i];

		thread[i].Init(init);
	}
/*
#if defined(WIN32) || defined(WIN64)
		::Sleep(20*1000);
#elif defined(__linux__) || defined(__unix__)
		sleep(20);
#endif
*/
	for(int i=4; i>=0; i--)
	{
		thread[i].Wait();
	}

	return 0;
}

