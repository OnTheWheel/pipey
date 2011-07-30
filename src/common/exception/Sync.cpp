#include "Sync.h"

using namespace pipey::common::exception;

static const char * _SyncMsg = "ESync : thread or synchronization related exception";

ESync::ESync(void) : 
ERuntime(_SyncMsg) 
{
}

ESync::ESync(const char * pMsg, bool bAllocatedMemory) :
ERuntime(pMsg, bAllocatedMemory)
{
}

ESync::~ESync(void) throw()
{
}
