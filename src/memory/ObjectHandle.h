#ifndef PIPEY_OBJECTHANDLE_H
#define PIPEY_OBJECTHANDLE_H

#include "../common/common.h"

namespace pipey {
	namespace memory {

		template<typename T>
		class IHandleManipulator;

		template<typename T>
		class CObjectHandle {
			friend class IHandleManipulator<T>;
		public:
			CObjectHandle(void ) :
			m_pTarget(NULL),
			m_pManipulator(NULL)
			{
				CloseHandle();
			}

			CObjectHandle(const CObjectHandle<T> & rhs)
			{
				if( rhs )
					m_pManipulator->DuplicateHandle(rhs, *this);
			}

			virtual ~CObjectHandle(void) 
			{
				SAFE_TRY(CloseHandle());
			}

		private:
			CObjectHandle(T * pTarget, IHandleManipulator<T> * pManipulator):
			m_pTarget(pTarget),
			m_pManipulator(pManipulator)
			{
			}

			T * m_pTarget;

			IHandleManipulator<T> * m_pManipulator;

		public:
			bool IsValid() const
			{

				return ( m_pTarget &&  m_pManipulator );
			}

			operator bool() const
			{
				return IsValid();
			}

			const CObjectHandle<T> & operator =(const CObjectHandle<T> & rhs)
			{

				if( rhs )
					m_pManipulator->DuplicateHandle(rhs, *this);
				return *this;
			}

			bool CloseHandle()
			{

				if( IsValid() )
				{
					m_pManipulator->CloseHandle(*this);
					return true;
				}
				else return false;
			}

		};
	}
}
#endif
