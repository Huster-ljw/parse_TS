#include "section.h"

PROG_INFO::PROG_INFO(uint8_t *data)
			: program_number(data[0] << 8 | data[1]),
			  reserved(data[2] >> 5),
			  PMT_PID((data[2] & 0x1F) << 8 | data[3])
{

}

PROG_INFO::~PROG_INFO()
{

}

STREAM_INFO::STREAM_INFO(uint8_t *data)
	          : stream_type(data[0]),
			    elementary_PID(((data[1] & 0x1F) << 8 | data[2]) & 0x1FFF),
				ES_info_length((data[3] & 0x0F) << 8 | data[4])
{

}

STREAM_INFO::~STREAM_INFO()
{

}