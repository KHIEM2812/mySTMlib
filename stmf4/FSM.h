#ifndef _FSM_h
#define _FSM_h

#include <stdint.h>
#include "Assert.h"

#define RET_HANDLED ((State)0)
#define RET_IGNORED ((State)1)
#define RET_TRAN ((State)2)

#define HANDLED() (RET_HANDLED)
#define IGNORED() (RET_IGNORED)
#define TRANS(target_) \
	(((FSM *)me)->state = (StateHandler)(target_), RET_TRAN)

typedef struct EventTag {
	unsigned int sig;
	unsigned int dynamic_;
} Event;

typedef struct TickEvtTag{
	Event super;
	uint32_t waitingTime;
	uint32_t lastTimming;
} TickEvt;

typedef uint8_t State;
typedef State (*StateHandler)(void *me, Event const *e);

typedef struct FSMTag {
	StateHandler state;
	StateHandler latchState;
} FSM;

enum {
	_EMPTY_SIG_ = 0, /*!< reserved empty signal for internal use only */

	/*! maximum depth of state nesting in a HSM (including the top level),
	 * must be >= 3
	 */
	_MAX_NEST_DEPTH_ = 6,

	/*! maximum depth of entry levels in a MSM for transition to history. */
	_MAX_ENTRY_DEPTH_ = 4
};

enum ReservedSignals {
	_ENTRY_SIG = 1, /*!< signal for coding entry actions */
	_EXIT_SIG, /*!< signal for coding exit actions */
	_INIT_SIG, /*!< signal for coding initial transitions */
	_USER_SIG /*!< first signal that can be used for user signals */
};

#define FSMConstructor(me_, initial_) ((me_)->state = (initial_))

void FSMInit(FSM *me, Event const *e);
void FSMDispatch(FSM *me, Event const *e);

#endif
