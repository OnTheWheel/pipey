#ifndef PIPEY_EXCEPTION_H
#define PIPEY_EXCEPTION_H

#include <stdexcept>
#include <stdlib.h>

#define SAFE_TRY(X) try{ (X); } catch(...) { }

namespace pipey {
	namespace common {

		static const char * _PipeyExecptionMsg = "EPipeyExecption : undefined exception";

		class EPipeyExecption : public std::exception {
		public:
			EPipeyExecption() :
			  m_pMsg(_PipeyExecptionMsg),
				  m_bAllocatedMemory(false) 
			  { };

			  EPipeyExecption(const char * pMsg, bool bAllocatedMemory = false) :
			  m_pMsg(pMsg),
				  m_bAllocatedMemory(bAllocatedMemory) 
			  { };

			  virtual ~EPipeyExecption() throw() {
				  if( m_pMsg && m_bAllocatedMemory )
					  free(static_cast<void *>(const_cast<char *>(m_pMsg)));
			  };

			  virtual const char * what() const throw() {
				  return m_pMsg;
			  };

		private:
			const char * m_pMsg;
			bool m_bAllocatedMemory;
		};

		static const char * _LogicalMsg = "ELogical : logical exception";

		class ELogical : public EPipeyExecption {
		public:
			ELogical() : 
			  EPipeyExecption(_LogicalMsg) 
			  { };

			  ELogical(const char * pMsg, bool bAllocatedMemory = false) :
			  EPipeyExecption(pMsg, bAllocatedMemory)
			  { };

			  virtual ~ELogical() throw() { };
		};

		static const char * _OutOfBoundMsg = "EOutOfBound : specified index is out of bound";

		class EOutOfBound : public ELogical {
		public:
			EOutOfBound() :
			  ELogical(_OutOfBoundMsg)
			  { };

			  EOutOfBound(const char * pMsg, bool bAllocatedMemory = false) :
			  ELogical(pMsg, bAllocatedMemory)
			  { };

			  virtual ~EOutOfBound() throw() { };
		};

		static const char * _InvalidParameterMsg = "EInvalidParameter : specified parameter is invalid";

		class EInvalidParameter : public ELogical {
		public:
			EInvalidParameter() :
			  ELogical(_InvalidParameterMsg) 
			  { };

			  EInvalidParameter(const char * pMsg, bool bAllocatedMemory = false) :
			  ELogical(pMsg, bAllocatedMemory)
			  { };

			  EInvalidParameter(const std::invalid_argument & e) :
			  ELogical(e.what())
			  { };

			  virtual ~EInvalidParameter() throw() { };
		};

		static const char * _DivideByZeroMsg = "EDivideByZero : cannot divide by zero";

		class EDivideByZero : public ELogical {
		public:
			EDivideByZero() :
			  ELogical(_DivideByZeroMsg) 
			  { };

			  EDivideByZero(const char * pMsg, bool bAllocatedMemory = false) :
			  ELogical(pMsg, bAllocatedMemory)
			  { };

			  virtual ~EDivideByZero() throw() { };
		};

		static const char * _RuntimeMsg = "ERuntime : runtime exception";

		class ERuntime : public EPipeyExecption {
		public:
			ERuntime() : 
			  EPipeyExecption(_RuntimeMsg) 
			  { };

			  ERuntime(const char * pMsg, bool bAllocatedMemory = false) :
			  EPipeyExecption(pMsg, bAllocatedMemory)
			  { };

			  virtual ~ERuntime() throw() { };
		};

		static const char * _MemoryAllocationMsg = "EMemoryAllocation : cannot allocate memory";

		class EMemoryAllocation : public ERuntime {
		public:
			EMemoryAllocation() :
			  ERuntime(_MemoryAllocationMsg)
			  { };

			  EMemoryAllocation(const char * pMsg, bool bAllocatedMemory = false) :
			  ERuntime(pMsg, bAllocatedMemory)
			  { };

			  virtual ~EMemoryAllocation() throw() { };
		};

		static const char * _MemoryCorruptionMsg = "EMemoryCorruption : memory corrupted";

		class EMemoryCorruption : public ERuntime {
		public:
			EMemoryCorruption() :
			  ERuntime(_MemoryCorruptionMsg)
			  { };

			  EMemoryCorruption(const char * pMsg, bool bAllocatedMemory = false) :
			  ERuntime(pMsg, bAllocatedMemory)
			  { };

			  virtual ~EMemoryCorruption() throw() { };
		};

		static const char * _InvalidStateMsg = "EInvalidState : some resources(objects) are in invalid state";

		class EInvalidState : public ERuntime {
		public:
			EInvalidState() :
			  ERuntime(_InvalidStateMsg)
			  { };

			  EInvalidState(const char * pMsg, bool bAllocatedMemory = false) :
			  ERuntime(pMsg, bAllocatedMemory)
			  { };

			  virtual ~EInvalidState() throw() { };
		};

		static const char * _SyncMsg = "ESync : thread or synchronization related exception";

		class ESync : public ERuntime {
		public:
			ESync() :
			  ERuntime(_SyncMsg)
			  { };

			  ESync(const char * pMsg, bool bAllocatedMemory = false) :
			  ERuntime(pMsg, bAllocatedMemory)
			  { };

			  virtual ~ESync() throw() { };
		};
	}
}
#endif
