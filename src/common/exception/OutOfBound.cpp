#include "OutOfBound.h"

using namespace pipey::common::exception;

static const char * _OutOfBoundMsg = "EOutOfBound : specified index is out of bound";

EOutOfBound::EOutOfBound(void) : 
ELogical(_OutOfBoundMsg) 
{
}

EOutOfBound::EOutOfBound(const char * pMsg, bool bAllocatedMemory) :
ELogical(pMsg, bAllocatedMemory)
{
}

EOutOfBound::~EOutOfBound(void)
{
}
