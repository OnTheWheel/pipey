// pipey.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//


#include "src/common/common.h"
#include "src/thread/DefaultThread.h"

struct THREAD_TEST
{
	unsigned int id;
};

class CThreadTest : public pipey::thread::IExecutable
{
	void Execute(void *pParam)
	{
		THREAD_TEST *pId = (THREAD_TEST*) pParam;

		::Sleep(pId->id*1000);
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

	for(int i=4; i>=0; i--)
	{
		thread[i].Wait();
	}

	return 0;
}

