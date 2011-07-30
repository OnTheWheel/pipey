#include "MemoryAllocation.h"

using namespace pipey::common::exception;

static const char * _MemoryAllocationMsg = "EMemoryAllocation : cannot allocate memory";

EMemoryAllocation::EMemoryAllocation(void) : 
ERuntime(_MemoryAllocationMsg) 
{
}

EMemoryAllocation::EMemoryAllocation(const char * pMsg, bool bAllocatedMemory) :
ERuntime(pMsg, bAllocatedMemory)
{
}

EMemoryAllocation::~EMemoryAllocation(void) throw()
{
}
