
#ifndef PIPEY_COMMON_TYPE_H
#define PIPEY_COMMON_TYPE_H

#if defined(WIN32) || defined(WIN64)
	typedef __int8 int8_t;	
	typedef __int16 int16_t;
	typedef __int32 int32_t;
	typedef __int64 int64_t;
	typedef unsigned __int8 uint8_t;	
	typedef unsigned __int16 uint16_t;
	typedef unsigned __int32 uint32_t;
	typedef unsigned __int64 uint64_t;
#elif defined(__linux__) || defined(__unix__)
	typedef unsigned int8_t uint8_t;	
	typedef unsigned int16_t uint16_t;
	typedef unsigned int32_t uint32_t;
	typedef unsigned int64_t uint64_t;
#endif

#endif
