#ifndef PIPEY_BYTEORDER_H
#define PIPEY_BYTEORDER_H

namespace pipey {
	namespace util {

		enum BYTE_ORDER {
			LITTLE_ENDIAN,
			BIG_ENDIAN
		};
		
		BYTE_ORDER GetSystemByteOrder();
	}
}

#endif