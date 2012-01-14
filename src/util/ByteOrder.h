#ifndef PIPEY_BYTEORDER_H
#define PIPEY_BYTEORDER_H

namespace pipey {
	namespace util {

		enum BYTE_ENDIAN {
			BYTE_LITTLE_ENDIAN,
			BYTE_BIG_ENDIAN
		};
		
		BYTE_ENDIAN GetSystemByteOrder();
	}
}

#endif
