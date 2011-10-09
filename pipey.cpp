// pipey.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//


#include "src/common/common.h"
#include "src/thread/DefaultThread.h"
#include "src/thread/sync/DefaultLock.h"
#include "src/thread/sync/DefaultCondition.h"
#include "src/thread/sync/MutableSemaphore.h"
#include "src/thread/sync/LockPtr.h"
#include "src/thread/sync/TimerableLockPtr.h"
#include "src/thread/sync/TriableLockPtr.h"
#include "src/thread/sync/TriableTimerableLockPtr.h"
#include "src/thread/pool/SimpleThreadPool.h"
#include "src/thread/pool/SimpleJobQueue.h"
#include "src/thread/pool/ContextAwareThreadPool.h"
//#include "src/thread/sync/WindowsCritricalSection.h"
#include "src/util/DefaultTimer.h"
//#include "src/util/RandomPicker.h"
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace pipey::common;
using namespace pipey::memory;
using namespace pipey::thread::pool;

#if defined(__linux__) || defined(__unix__)
#include <unistd.h>
#endif

bool completed[10];
/*
class CTestCallback : public IJobCallback<int>
{
	virtual void ProcessJob(int & job)
	{
		cout<<job<<endl;
		completed[job] = true;

#if defined(WIN32) || defined(WIN64)
		::Sleep(10000);
#elif defined(__linux__) || defined(__unix__)
		sleep(10);
#endif
	}

	virtual void OnCancel(int & job, bool bTimeout = false) 
	{
		cout<<job<<" canceled"<<endl;
	}

	virtual void OnException(int & job, const std::exception & e) {}
}callback;
*/
struct CTX_JOB
{
	int ctx;
	int job;
};
class CTestCallback : public IJobCallback<CTX_JOB>
{
	virtual void ProcessJob(CTX_JOB & job)
	{
		string ws;
		for(int i=0;i<job.ctx;i++)
			ws+="   ";
		cout<<ws.c_str()<<job.ctx<<"{"<<job.job<<endl;

#if defined(WIN32) || defined(WIN64)
		::Sleep(1000);
#elif defined(__linux__) || defined(__unix__)
		sleep(1);
#endif
		cout<<ws.c_str()<<job.ctx<<"}"<<job.job<<endl;
	}

	virtual void OnCancel(CTX_JOB & job, bool bTimeout = false) 
	{
		cout<<job.ctx<<":"<<job.job<<" canceled"<<endl;
	}

	virtual void OnException(CTX_JOB & job, const std::exception & e) {}
}callback;

int main(int argc, char* argv[])
{
	/*::pipey::thread::pool::CContextAwareJobQueue<int > cq;
	::pipey::thread::pool::CONTEXT_INFO<int> *contexts[5];
	contexts[0]= cq.CreateContext();
	contexts[1]= cq.CreateContext();
	contexts[2]= cq.CreateContext();
	contexts[3]= cq.CreateContext();
	contexts[4]= cq.CreateContext();*/

	CContextAwareThreadPool<CTX_JOB> cap;
	cap.Init(3,5,4);

	CObjectHandle< CONTEXT_INFO<CTX_JOB> > contexts[10];

	int i;
	for(i=0;i<10;i++)
		cap.CreateContext(contexts[i]);

	for(i=0;i<100;i++){
		CTX_JOB job;
		job.ctx = i%10;
		job.job = i/10;
		cap.PushJob(contexts[i%10], job, &callback);
	}

	#if defined(WIN32) || defined(WIN64)
	::Sleep(20000);
#elif defined(__linux__) || defined(__unix__)
	sleep(20);
#endif

	for(i=0;i<10;i++)
		cap.CloseHandle(contexts[i]);

	cap.Close();

	return 0;


	/*
	pipey::util::CRandomPicker<int> picker(20);

	unsigned long ref[400];

	int i;
	for(i=0; i<400;i++)
		picker.Put(i, ref+i);


	int total =0;
	for(i=0; i<400;i++)
	{
		int val = picker.Pick();
		cout<<val<<endl;
		picker.Delete(ref[val]);
		total += val;
	}

	cout<<total<<endl;
	return 0;
	*/
	/*
	CSimpleThreadPool<int> pool;
	CObjectHandle< JOB_INFO<int> > handle[10];
	pool.Init(3,15,4);

	int i;
	for(i=0;i<10;i++)
	{
		pool.PushJob(i, &callback, handle+i);
	}

#if defined(WIN32) || defined(WIN64)
	::Sleep(1000);
#elif defined(__linux__) || defined(__unix__)
	sleep(1);
#endif

	
	for(i=0;i<10;i++)
	{
		if(i%2 == 0 && !completed[i])
			pool.CancelJob(handle[i]);
		handle[i].CloseHandle();
	}


#if defined(WIN32) || defined(WIN64)
	::Sleep(10000);
#elif defined(__linux__) || defined(__unix__)
	sleep(10);
#endif

	pool.Close();
	return 0;*/
}

/*
struct THREAD_TEST
{
	unsigned int id;
	pipey::thread::sync::CMutableSemaphore *pLock;
};

class CThreadTest : public pipey::thread::IExecutable
{
	void Execute(void *pParam)
	{
		THREAD_TEST *pId = (THREAD_TEST*) pParam;
		for(int i=0; i<5;) {
			pipey::thread::sync::CLockPtr ptr(pId->pLock);
			pipey::util::CDefaultTimer timer;
			timer.Start();
			SYNC_RESULT res = ptr.AcquireLock();
			if( res == pipey::common::SYNC_TIMEOUT ) 
			{
#if defined(WIN32) || defined(WIN64)
			//::Sleep(1000);
#elif defined(__linux__) || defined(__unix__)
			//sleep(1);
#endif

				unsigned long elapsed;
				timer.CheckTotal(&elapsed);
				printf("timed out : %u\n", elapsed);
				continue;
			}
			i++;
			//printf("thread %d acquired lock.\n", pId->id);
			printf("there are %d threads in the room.\n", pId->pLock->GetCurrentOwner());
#if defined(WIN32) || defined(WIN64)
			::Sleep(1000);
#elif defined(__linux__) || defined(__unix__)
			sleep(1);
#endif
		}
	}
};

int main(int argc, char* argv[])
{
	pipey::thread::sync::CMutableSemaphore lock;
	lock.Init(&pipey::thread::sync::MUT_SEM_INIT(4,1));

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

	return 0;
}
*/
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
			pipey::thread::sync::CTriableTimerableLockPtr ptr(pId->pLock);
			SYNC_RESULT res = ptr.TryLock();
			if( res == pipey::common::SYNC_BUSY ) 
			{
#if defined(WIN32) || defined(WIN64)
			::Sleep(1000);
#elif defined(__linux__) || defined(__unix__)
			sleep(1);
#endif

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

	return 0;
}*/

/*
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
		for( int i=0; i<5; ){
		SYNC_RESULT res = pId->pCondition->Wait(2500);
		//SYNC_RESULT res = pId->pLock->AcquireTimedLock(2345);
		if( res == pipey::common::SYNC_TIMEOUT ) 
		{
			puts("timed out");
			continue;
		}
		i++;	
		printf("thread %d is Awakeed.\n", pId->id);
#if defined(WIN32) || defined(WIN64)
		::Sleep(1000);
#elif defined(__linux__) || defined(__unix__)
		sleep(1);
#endif
		}
		printf("thread %d is terminated.\n", pId->id);
	}
};

int main()
{
	pipey::thread::sync::CDefaultLock lock;
	lock.Init();

	pipey::thread::sync::CDefaultCondition condition;
	condition.Init(&pipey::thread::sync::POSIX_COND_INIT(false, 0));

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
	for(i=24; i>=0; i--)
	{
#if defined(WIN32) || defined(WIN64)
		::Sleep(2*1000);
#elif defined(__linux__) || defined(__unix__)
		sleep(2);
#endif

		condition.Awake();
	//	lock.ReleaseLock();
	}

	for(i=4; i>=0; i--)
	{
		thread[i].Wait();
	}

	return 0;
}

*/

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
