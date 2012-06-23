#ifndef PIPEY_WINDOWSEVENT_H
#define PIPEY_WINDOWSEVENT_H

#include "Condition.h"

namespace pipey {
	namespace thread {
		namespace sync {

			/// specifies attributes needed to initiate a windows event.
			class WIN_EVENT_INIT : public CONDITION_INIT {
			public:
				WIN_EVENT_INIT(void ) :
					bAutoReset(true),
					bInitialState(false),
					lpName(NULL)
				{};

				WIN_EVENT_INIT(bool autoReset, bool initialState, LPCTSTR name) :
					bAutoReset(autoReset),
					bInitialState(initialState),
					lpName(name)
				{};

				  /// Specifies whether an event object is in auto-reset mode or not. Default is true.
				  bool bAutoReset;

				  /// Specifies whether an event object is signaled or not at first. Default is false.
				  bool bInitialState;

				  /// Specifies the unique name of an event object for inter-process synchronization. Default is NULL.
				  LPCTSTR lpName;

			};

			/// represents a windows event.
			class CWindowsEvent : public ICondition {
			public:
				CWindowsEvent(void );

				virtual ~CWindowsEvent(void );


			protected:
				HANDLE m_hEvent;

				bool m_bAutoReset;

				bool m_bInited;


			public:

				/// initiate a condition variable with specified attributes.
				/// if you don't specify pParam, it initiates a condition variable with default attributes.
				/// (bAutoReset == true, bInitialState == false, lpName == NULL)
				/// @warning pParam should be type of pointer to WIN_EVENT_INIT not CONDITION_INIT.
				/// @param pParam a pointer to WIN_EVENT_INIT. Default is NULL.
				virtual void Init(const CONDITION_INIT * pParam = NULL);

				virtual pipey::common::SYNC_RESULT Wait(uint32_t nMilliSeconds = pipey::common::TIME_INFINITE);

				virtual pipey::common::SYNC_RESULT Test();

				virtual void Awake();

				virtual void Close();

				/// reset a windows event.
				virtual void ResetEvent();

			};

		}
	}
}
#endif
