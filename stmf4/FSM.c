#include "FSM.h"
//#include "TraceBuffer.h"

Event const _reservedEvt_[] = {
		{ (unsigned int) _EMPTY_SIG_, (unsigned int) 0 }, {
				(unsigned int) _ENTRY_SIG, (unsigned int) 0 }, {
				(unsigned int) _EXIT_SIG, (unsigned int) 0 }, {
				(unsigned int) _INIT_SIG, (unsigned int) 0 } };

void FSMInit(FSM *me, Event const *e) {
	(*me->state)(me, e); //initial with signal _EMPTY_SIG_--> execute FSMInitilize
	(void) (*me->state)(me, &_reservedEvt_[_ENTRY_SIG]); //exe with signal _ENTRY_SIG--> execute the entry routine of the first state
}

void FSMDispatch(FSM *me, Event const *e) {
	me->latchState = me->state;
	State r = (*me->latchState)(me, e);

	if (r == RET_TRAN) {
		(void) (*me->latchState)(me, &_reservedEvt_[_EXIT_SIG]);
		(void) (*me->state)(me, &_reservedEvt_[_ENTRY_SIG]);

#ifdef TRACE_ENABLE
		TRACE_BEGIN(_STATE_TRANS);
		TRACE_TIMESTAMP();
		TRACE_STATE();
		TRACE_SIGNAL();
		TRACE_END();
#endif
	}
}
