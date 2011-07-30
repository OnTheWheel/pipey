// pipey.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//


#include "src/common/common.h"
#include "src/thread/DefaultThread.h"
#include <stdio.h>

#if defined(__linux__) || defined(__unix__)
#include <unistd.h>
#endif

struct THREAD_TEST
{
	unsigned int id;
};

class CThreadTest : public pipey::thread::IExecutable
{
	void Execute(void *pParam)
	{
		THREAD_TEST *pId = (THREAD_TEST*) pParam;
#if defined(WIN32) || defined(WIN64)
		::Sleep(pId->id*1000);
#elif defined(__linux__) || defined(__unix__)
		sleep(pId->id);
#endif
		printf("thread %d terminate.\n", pId->id);
	}
};

int main(int argc, char* argv[])
{
	THREAD_TEST ids[5];
	pipey::thread::CDefaultThread thread[5];
	CThreadTest routine;

	for(unsigned int i=0; i<5; i++)
	{
		ids[i].id = i+1;

		pipey::thread::THREAD_INIT init;
		init.pExec = &routine;
		init.pParam = &ids[i];

		thread[i].Init(init);
	}
	//sleep(10);
	for(int i=4; i>=0; i--)
	{
		thread[i].Wait();
	}

	return 0;
}

