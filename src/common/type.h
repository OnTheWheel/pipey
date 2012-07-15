#ifndef PIPEY_COMMON_TYPE_H
#define PIPEY_COMMON_TYPE_H

#if defined(WIN32) || defined(WIN64)
	#include <Winsock2.h>
	typedef __int8 int8_t;	
	typedef __int16 int16_t;
	typedef __int32 int32_t;
	typedef __int64 int64_t;
	typedef unsigned __int8 uint8_t;	
	typedef unsigned __int16 uint16_t;
	typedef unsigned __int32 uint32_t;
	typedef unsigned __int64 uint64_t;
	typedef HANDLE descriptor_t;
	typedef WSABUF SCATTERED_BUFFER;
#elif defined(__linux__) || defined(__unix__)
	#include <stdint.h>
	typedef int32_t descriptor_t;
#endif

#endif
