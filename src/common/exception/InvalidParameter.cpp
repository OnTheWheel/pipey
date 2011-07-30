#include "InvalidParameter.h"

using namespace pipey::common::exception;

static const char * _InvalidParameterMsg = "EInvalidParameter : specified parameter is invalid";

EInvalidParameter::EInvalidParameter(void) : 
ELogical(_InvalidParameterMsg) 
{
}

EInvalidParameter::EInvalidParameter(const char * pMsg, bool bAllocatedMemory) :
ELogical(pMsg, bAllocatedMemory)
{
}

EInvalidParameter::~EInvalidParameter(void)
{
}
