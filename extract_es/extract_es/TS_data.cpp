#include "ts_data.h"

TS_DATA::TS_DATA(uint8_t *data)
	: raw_data(data),  
	  syc_byte(raw_data[0]),
	  transport_error_indicator(raw_data[1] >> 7),
	  payload_unit_start_indicator((raw_data[1] >> 6) & 0x01),
	  transport_priority((raw_data[1] >> 5) & 0x01),
	  PID(((raw_data[1] & 0x1F) << 8) | raw_data[2]),
	  transport_scrambling_control(raw_data[3] >> 6),
	  adaption_field_control((raw_data[3] >> 4) & 0x03),
	  continuity_counter(raw_data[3] & 0x0F)
{
	if (syc_byte != 0x47)
	{
		throw new SyncErr();
	}
}

TS_DATA::~TS_DATA()
{

}
