#include "InvalidState.h"

using namespace pipey::common::exception;

static const char * _InvalidStateMsg = "EInvalidState : some resources(objects) are in invalid state";
EInvalidState::EInvalidState(void) : 
ERuntime(_InvalidStateMsg) 
{
}

EInvalidState::EInvalidState(const char * pMsg, bool bAllocatedMemory) :
ERuntime(pMsg, bAllocatedMemory)
{
}

EInvalidState::~EInvalidState(void) throw()
{
}
