#ifndef PIPEY_OVERLAPPED_EX_H
#define PIPEY_OVERLAPPED_EX_H

#include "../IOOperator.h"

namespace pipey {
	namespace io {

		struct OVERLAPPED_EX : public OVERLAPPED
		{
			IIOOperator *pOperator;
		};
	}
}

#endif