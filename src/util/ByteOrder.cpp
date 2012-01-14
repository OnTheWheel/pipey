#include "ByteOrder.h"
#include "../common/exception/exceptions.h"

namespace pipey {
	namespace util {
	
		BYTE_ENDIAN GetSystemByteOrder()
		{
			if(sizeof(short) != 2)
				throw ::pipey::common::exception::ERuntime("ERuntime => GetSystemByteOrder - sizeof(short) is not 2");

			union
			{
				short s;
				char c[2];
			} un;

			un.s = 0x0102;

			if(un.c[0] == 1 && un.c[1] == 2)
				return BYTE_BIG_ENDIAN;
			else if(un.c[0] == 2 && un.c[1] == 1)
				return BYTE_LITTLE_ENDIAN;
			else throw ::pipey::common::exception::ERuntime("ERuntime => GetSystemByteOrder - unknown exception");
		}
	}
}
