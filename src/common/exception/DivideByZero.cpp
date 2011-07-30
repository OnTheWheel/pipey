#include "DivideByZero.h"

using namespace pipey::common::exception;

static const char * _DivideByZeroMsg = "EDivideByZero : cannot divide by zero";

EDivideByZero::EDivideByZero(void) : 
ELogical(_DivideByZeroMsg) 
{
}

EDivideByZero::EDivideByZero(const char * pMsg, bool bAllocatedMemory) :
ELogical(pMsg, bAllocatedMemory)
{
}

EDivideByZero::~EDivideByZero(void) throw()
{
}
