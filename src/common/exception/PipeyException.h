#ifndef PIPEY_PIPEYEXCEPTION_H
#define PIPEY_PIPEYEXCEPTION_H

#include <stdexcept>

namespace pipey {
	namespace common {

		/// includes exception classes defined in the pipey.
		namespace exception {

			class EPipeyException : public std::exception {
			public:
				EPipeyException();
				EPipeyException(const char * pMsg, bool bAllocatedMemory = false);
				virtual ~EPipeyException() throw();

				virtual const char * what() const throw();

			private:
				const char * m_pMsg;
				bool m_bAllocatedMemory;
			};

		}
	}
}

#endif