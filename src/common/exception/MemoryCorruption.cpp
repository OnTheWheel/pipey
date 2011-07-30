#include "MemoryCorruption.h"

using namespace pipey::common::exception;

static const char * _MemoryCorruptionMsg = "EMemoryCorruption : memory corrupted";

EMemoryCorruption::EMemoryCorruption(void) : 
ERuntime(_MemoryCorruptionMsg) 
{
}

EMemoryCorruption::EMemoryCorruption(const char * pMsg, bool bAllocatedMemory) :
ERuntime(pMsg, bAllocatedMemory)
{
}

EMemoryCorruption::~EMemoryCorruption(void) throw()
{
}
