#ifndef PIPEY_COMMON_H
#define PIPEY_COMMON_H

#include "./exception/exceptions.h"
#include "constant.h"
#include "macro.h"

using namespace pipey::common;
using namespace pipey::common::exception;

#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#endif

#endif
 