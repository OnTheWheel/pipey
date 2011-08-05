#include "TriableTimerableLockPtr.h"

using namespace pipey::common;
using namespace pipey::common::exception;
using namespace pipey::thread::sync;

CTriableTimerableLockPtr::CTriableTimerableLockPtr(ITriableTimerableLock * pLock) :
CLockPtr(pLock),
CTriableLockPtr(pLock),
CTimerableLockPtr(pLock)
{

}

CTriableTimerableLockPtr::~CTriableTimerableLockPtr()
{

}

