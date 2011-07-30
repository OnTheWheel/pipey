#include "Logical.h"

using namespace pipey::common::exception;

static const char * _LogicalMsg = "ELogical : logical exception";

ELogical::ELogical(void) : 
EPipeyException(_LogicalMsg) 
{
}

ELogical::ELogical(const char * pMsg, bool bAllocatedMemory) :
EPipeyException(pMsg, bAllocatedMemory)
{
}

ELogical::~ELogical(void) throw()
{
}
