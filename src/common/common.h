/** @file
 This file includes common components and header files.

 e.g.\ )exception classes, constants, macros and system dependant common header files.

*/

#ifndef PIPEY_COMMON_H
#define PIPEY_COMMON_H

#include "./exception/exceptions.h"
#include "constant.h"
#include "macro.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#endif

#endif
 
