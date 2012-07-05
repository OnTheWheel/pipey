#ifndef PIPEY_HANDLEMANIPULATOR_H
#define PIPEY_HANDLEMANIPULATOR_H

#include "ObjectHandle.h"

namespace pipey {
	namespace memory {

		template<typename T>
		class IHandleManipulator {
		public:
			IHandleManipulator(void ) { }

			virtual ~IHandleManipulator(void ) { }

			virtual void CloseHandle(CObjectHandle<T> & rHandle) = 0;

			virtual void DuplicateHandle(const CObjectHandle<T> & rSource, CObjectHandle<T> & rTarget) = 0;


		protected:
			virtual T * GetHandleTarget(const CObjectHandle<T> & rHandle) const 
			{
				return rHandle.m_pTarget;
			}

			virtual bool InitHandle(CObjectHandle<T> & rHandle, const T * pTarget) 
			{
				if( rHandle ) return false;
				else {
					rHandle.m_pManipulator = this;
					rHandle.m_pTarget =  const_cast<T *>(pTarget);
					return true;
				}
			}

			virtual bool IsMine(const CObjectHandle<T> & rHandle) const 
			{
				return rHandle.m_pManipulator == this;
			}

			virtual bool NullifyHandle(CObjectHandle<T> & rHandle) 
			{
				if( IsMine(rHandle) ) {
					rHandle.m_pManipulator = 0;
					rHandle.m_pTarget = 0;
					return true;
				}
				return false;
			}
		};

	}
}
#endif
