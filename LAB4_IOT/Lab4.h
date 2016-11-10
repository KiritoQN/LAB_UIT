#ifndef BLINKTORADIO_H

#define BLINKTORADIO_H


enum 

{
	AM_BLINKTORADIO = 6,

	TIMER_PERIOD_MILLI = 250,

};



typedef nx_struct Lab4Msg_t

{
	nx_uint16_t nodeid;
	nx_uint16_t tmprt;
	nx_uint16_t hmdt;
	nx_uint16_t lght;

} Lab4Msg;

#endif
