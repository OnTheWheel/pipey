#ifndef PIPEY_BYTEORDER_H
#define PIPEY_BYTEORDER_H

namespace pipey {
	namespace util {

		#define REVERSE_ORDER16(value) ( (value >> 8) | (value << 8) )
		#define REVERSE_ORDER32(value) ( (value >> 24) | ((value << 8) & 0x00FF0000) | \
										((value >> 8) & 0x0000FF00) | (value << 24)  )
		#define REVERSE_ORDER64(value) ( (value >> 56) | \
										((value << 40) & 0x00FF000000000000) | \
										((value << 24) & 0x0000FF0000000000) | \
										((value << 8) & 0x000000FF00000000)  | \
										((value >> 8) & 0x00000000FF000000)  | \
										((value >> 24) & 0x0000000000FF0000) | \
										((value >> 40) & 0x000000000000FF00) | \
										(value << 56) )

		enum BYTE_ENDIAN {
			BYTE_LITTLE_ENDIAN,
			BYTE_BIG_ENDIAN
		};
		
		BYTE_ENDIAN GetSystemByteOrder();
	}
}

#endif
