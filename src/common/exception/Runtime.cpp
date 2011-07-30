#include "Runtime.h"

using namespace pipey::common::exception;

static const char * _RuntimeMsg = "ERuntime : runtime exception";

ERuntime::ERuntime(void) : 
EPipeyException(_RuntimeMsg) 
{
}

ERuntime::ERuntime(const char * pMsg, bool bAllocatedMemory) :
EPipeyException(pMsg, bAllocatedMemory)
{
}

ERuntime::~ERuntime(void) throw()
{
}
