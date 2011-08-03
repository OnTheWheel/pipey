#ifndef PIPEY_WINDOWSEVENT_H
#define PIPEY_WINDOWSEVENT_H

#include "Condition.h"

namespace pipey {
	namespace thread {
		namespace sync {


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

				  bool bAutoReset;

				  bool bInitialState;

				  LPCTSTR lpName;

			};

			class CWindowsEvent : public ICondition {
			public:
				CWindowsEvent(void );

				virtual ~CWindowsEvent(void );


			protected:
				HANDLE m_hEvent;

				bool m_bAutoReset;

				bool m_bInited;


			public:
				virtual void Init(const CONDITION_INIT * pParam = NULL);

				virtual pipey::common::SYNC_RESULT Wait(unsigned long nMilliSeconds = pipey::common::TIME_INFINITE);

				virtual pipey::common::SYNC_RESULT Test();

				virtual void Awake();

				virtual void Close();

				virtual void ResetEvent();

			};

		}
	}
}
#endif
