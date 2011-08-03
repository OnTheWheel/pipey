// pipey.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//


#include "src/common/common.h"
#include "src/thread/DefaultThread.h"
#include "src/thread/sync/DefaultLock.h"
#include "src/thread/sync/DefaultCondition.h"
//#include "src/thread/sync/WindowsCritricalSection.h"
//#include "src/util/PosixTimeHelper.h"
#include <stdio.h>

using namespace pipey::common;

#if defined(__linux__) || defined(__unix__)
#include <unistd.h>
#endif

struct THREAD_TEST
{
	unsigned int id;
	pipey::thread::sync::CDefaultCondition *pCondition;
};

class CThreadTest : public pipey::thread::IExecutable
{
	void Execute(void *pParam)
	{
		THREAD_TEST *pId = (THREAD_TEST*) pParam;

		pId->pCondition->Wait();
		/*SYNC_RESULT res = pId->pLock->AcquireTimedLock(2345);
		if( res == pipey::common::SYNC_TIMEOUT ) 
		{
			puts("timed out");
			continue;
		}*/
	
		printf("thread %d is awakened.\n", pId->id);
#if defined(WIN32) || defined(WIN64)
		::Sleep(1000);
#elif defined(__linux__) || defined(__unix__)
		sleep(1);
#endif

	}
};

int main()
{
	pipey::thread::sync::CDefaultCondition condition;
	condition.Init();

	THREAD_TEST ids[5];
	pipey::thread::CDefaultThread thread[5];
	CThreadTest routine;

	for(unsigned int i=0; i<5; i++)
	{
		ids[i].id = i+1;
		ids[i].pCondition = &condition;

		pipey::thread::THREAD_INIT init;
		init.pExec = &routine;
		init.pParam = &ids[i];

		thread[i].Init(init);
	}

	int i;
	for(i=4; i>=0; i--)
	{
#if defined(WIN32) || defined(WIN64)
		::Sleep(2*1000);
#elif defined(__linux__) || defined(__unix__)
		sleep(2);
#endif

		condition.Awake();
	}

	for(i=4; i>=0; i--)
	{
		thread[i].Wait();
	}

	return 0;
}



//using namespace pipey::util;
/*
struct THREAD_TEST
{
	unsigned int id;
	pipey::thread::sync::CDefaultLock *pLock;
};

class CThreadTest : public pipey::thread::IExecutable
{
	void Execute(void *pParam)
	{
		THREAD_TEST *pId = (THREAD_TEST*) pParam;
		for(int i=0; i<5;) {
			SYNC_RESULT res = pId->pLock->AcquireTimedLock(2345);
			if( res == pipey::common::SYNC_TIMEOUT ) 
			{
				puts("timed out");
				continue;
			}
			i++;
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
	pipey::thread::sync::CDefaultLock lock;
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

#if defined(WIN32) || defined(WIN64)
		::Sleep(20*1000);
#elif defined(__linux__) || defined(__unix__)
		sleep(20);
#endif

	for(int i=4; i>=0; i--)
	{
		thread[i].Wait();
	}

	timespec t, t2;
	RelativeToAbsolute(0, t);
	RelativeToAbsolute(3247, t2);
	printf("%ld   %ld\n", t.tv_sec, t.tv_nsec);
	printf("%ld   %ld\n", t2.tv_sec, t2.tv_nsec);
	return 0;
}

*/