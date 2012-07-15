/** @file
 This file includes common components and header files.

 e.g.\ )exception classes, constants, macros and system dependant common header files.

*/

#ifndef PIPEY_COMMON_H
#define PIPEY_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(WIN32) || defined(WIN64)
#include <Winsock2.h>
#elif defined(__linux__) || defined(__unix__)
#include <sys/uio.h>
#endif

#include "type.h"
#include "./exception/exceptions.h"
#include "constant.h"
#include "macro.h"



#endif
 
