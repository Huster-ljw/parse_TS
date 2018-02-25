#ifndef TS_DATA_
#define TS_DATA_

#include "def.h"
#include "PES.h"

class TS_DATA
{
public:
	
	TS_DATA(uint8_t *data);
	~TS_DATA();

	uint8_t *raw_data;			//tsÊý¾Ý
	uint8_t *ts_data;			//es data	

public:
	uint32_t	syc_byte;							//8
	uint32_t	transport_error_indicator;			//1
	uint32_t	payload_unit_start_indicator;		//1
	uint32_t	transport_priority;					//1
	uint16_t	PID;								//13
	uint32_t	transport_scrambling_control;		//2
	uint32_t	adaption_field_control;				//2
	uint32_t	continuity_counter;					//4

};


#endif