#ifndef PIPEY_RANDOMPICKER_H
#define PIPEY_RANDOMPICKER_H

#include "../common/exception/exceptions.h"
#include <time.h>
#include <stdlib.h>

//#define RP_INVALID_REFERENCE	0xFFFFFFFF
#define RP_DEFAULT_CAPACITY		100

namespace pipey {
	namespace util {

		template <typename T>
		class CRandomPicker
		{
		public:
			CRandomPicker(unsigned long nCapacity = RP_DEFAULT_CAPACITY);
			virtual ~CRandomPicker();

		public:
			void Put(const T& value, unsigned long *pReference = NULL);
			T& Pick();
			void Delete(unsigned long reference);

			unsigned long GetSize() const;
			bool IsEmpty() const;
			unsigned long GetCapacity() const;
			void GrowCapacity(unsigned long nGrow);

		private:
			unsigned long m_nInitialCapacity;
			unsigned long m_nSize;
			unsigned long m_nCapacity;
			T* m_pValues;
			unsigned long **m_pReferences;
		};

		template <typename T>
		CRandomPicker<T>::CRandomPicker(unsigned long nCapacity) :
		m_nInitialCapacity(nCapacity), m_nCapacity(nCapacity), m_nSize(0), m_pValues(NULL), m_pReferences(NULL)
		{
			if( nCapacity == 0 )
				throw ::pipey::common::exception::EInvalidParameter("::pipey::common::exception::EInvalidParameter => CRandomPicker<T>::CRandomPicker - capacity cannot be zero.");

			m_pValues = (T *)malloc(sizeof(T)*m_nCapacity);
			if( m_pValues == NULL )
				throw ::pipey::common::exception::EMemoryAllocation("::pipey::common::exception::EMemoryAllocation => CRandomPicker<T>::CRandomPicker - cannot allocate memory space for value array.");

			m_pReferences = (unsigned long * *)malloc(sizeof(unsigned long *)*m_nCapacity);
			if( m_pReferences == NULL ) {
				free( m_pValues );
				throw ::pipey::common::exception::EMemoryAllocation("::pipey::common::exception::EMemoryAllocation => CRandomPicker<T>::CRandomPicker - cannot allocate memory space for reference array.");
			}

			srand( (unsigned int)time(NULL) );
		}

		template <typename T>
		CRandomPicker<T>::~CRandomPicker()
		{
			free( m_pValues );
			free( m_pReferences );
		}

		template <typename T>
		void CRandomPicker<T>::Put(const T &value, unsigned long *pReference)
		{
			if( m_nSize == m_nCapacity )
				GrowCapacity( m_nInitialCapacity );

			m_pValues[m_nSize] = value;
			m_pReferences[m_nSize] = pReference;
			if( pReference ) *pReference = m_nSize;

			m_nSize++;
		}

		template <typename T>
		T& CRandomPicker<T>::Pick()
		{
			if( m_nSize == 0) 
				throw ::pipey::common::exception::EOutOfBound("::pipey::common::exception::EOutOfBound => CRandomPicker<T>::Pick() - the picker is empty.");

			int r = rand();

			if( RAND_MAX >= m_nSize )
				return m_pValues[r % m_nSize];
			else {
				unsigned long scaled = (unsigned long)( r*(m_nSize/(double)RAND_MAX) );
				return m_pValues[scaled];
			}
			
		}

		template <typename T>
		void CRandomPicker<T>::Delete(unsigned long reference)
		{
			if( reference >= m_nSize ) 
				throw ::pipey::common::exception::EInvalidParameter("::pipey::common::exception::EInvalidParameter => CRandomPicker<T>::Delete() - reference is invalid.");

			if( m_pReferences[m_nSize-1] )
				*m_pReferences[m_nSize-1] = reference;

			if( m_pReferences[reference] )
				*m_pReferences[reference] = ::pipey::common::INVALID;

			m_pValues[reference] = m_pValues[m_nSize-1];
			m_pReferences[reference] = m_pReferences[m_nSize-1];
			m_nSize--;
		}

		template <typename T>
		unsigned long CRandomPicker<T>::GetSize() const
		{
			return m_nSize;
		}

		template <typename T>
		bool CRandomPicker<T>::IsEmpty() const
		{
			return (m_nSize == 0);
		}

		template <typename T>
		unsigned long CRandomPicker<T>::GetCapacity() const
		{
			return m_nCapacity;
		}

		template <typename T>
		void CRandomPicker<T>::GrowCapacity(unsigned long nGrow)
		{
			if( nGrow == 0 )
				throw ::pipey::common::exception::EInvalidParameter("::pipey::common::exception::EInvalidParameter => CRandomPicker<T>::GrowCapacity() - nGrow cannot be zero.");

			T *pValues = (T *)malloc( sizeof(T)*(m_nCapacity+nGrow) );
			if( pValues == NULL )
				throw ::pipey::common::exception::EMemoryAllocation("::pipey::common::exception::EMemoryAllocation => CRandomPicker<T>::GrowCapacity() - cannot allocate memory space for value array.");

			unsigned long **pReferences = (unsigned long * *)malloc( sizeof(unsigned long *)*(m_nCapacity+nGrow) );
			if( pReferences == NULL ) {
				free( pValues );
				throw ::pipey::common::exception::EMemoryAllocation("::pipey::common::exception::EMemoryAllocation => CRandomPicker<T>::GrowCapacity() - cannot allocate memory space for reference array.");
			}

			for(unsigned long i=0; i<m_nSize; i++) {
				pValues[i] = m_pValues[i];
				pReferences[i] = m_pReferences[i];
			}

			free( m_pValues );
			free( m_pReferences );

			m_pValues = pValues;
			m_pReferences = pReferences;

			m_nCapacity += nGrow;
		}

	}
}

#endif