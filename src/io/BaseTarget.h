#ifndef PIPEY_BASESOCKET_H
#define PIPEY_BASESOCKET_H

#include "IOTarget.h"

namespace pipey {
	namespace io {

		class IBaseTarget : public IIOTarget {
		public:
			IBaseTarget();
			IBaseTarget(IBaseTarget *pParent, descriptor_t descriptor);

		public:
			virtual descriptor_t Open(const IO_TARGET_INIT * param) = 0;
			virtual void Close() = 0;
			virtual bool IsValid() const;
			virtual operator bool() const;
			virtual IIOTarget *GetParent() const;
			virtual void NullifyParent();
			virtual descriptor_t GetDescriptor() const;

		protected:
			virtual descriptor_t SetDescriptor(descriptor_t descriptor);

		private:
			descriptor_t m_descriptor;
			IBaseTarget *m_pParent;
		};
	}
}
#endif