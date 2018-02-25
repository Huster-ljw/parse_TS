#include "PMT.h"

PMT::PMT(uint8_t *data)
	  : raw_data(data),
	    table_id(raw_data[0]),
		section_syntax_indicator(raw_data[1] >> 7),
		zero((raw_data[1] >> 6) & 0x01),
		reserved1((raw_data[1] >> 4) & 0x03),
		section_length((raw_data[1] & 0x0F) << 8 | raw_data[2]),
		program_number(raw_data[3] << 8 | raw_data[4]),
		reserved2(raw_data[5] >> 6),
		version_number((raw_data[5] >> 1) & 0x1F),
		current_next_indicator(raw_data[5] & 0x01),
		section_number(raw_data[6]),
		last_section_number(raw_data[7]),
		reserved3(raw_data[8] >> 5),
		PCR_PID((raw_data[8] & 0x1F) << 8 | raw_data[9]),
		reserved4(raw_data[10] >> 4),
		program_info_length((raw_data[10] & 0x0F) << 8 | raw_data[11])

{
	
}

PMT::~PMT()
{

}

uint32_t PMT::check_type(uint32_t type)
{
	if ((type >= 0x00 && type <= 0x1E) || (type == 0x24) || (type == 0x42) || (type == 0x7F)
	  || (type >= 0x80 && type <= 0x86) || (type == 0x90) || (type == 0x92) || (type == 0x93)
	  || (type >= 0x99 && type <= 0x9B) || (type == 0xA1) || (type == 0xA2) || (type == 0xEA)
	  || (type >= 0xF0 && type <= 0xF2) || (type == 0xFF) || (type == 0x20))
	{
		return true;
	}
	return false;
}

void PMT::get_stream_detail(std::vector<STREAM_INFO*> &stream_info)
{
	uint32_t idx = 12 + program_info_length;

	if (!raw_data)
	{
		return;
	}

	while (idx < section_length + 3 - 4)
	{
		STREAM_INFO *info = new STREAM_INFO(raw_data + idx);

		if (check_type(info->stream_type))
		{
			stream_info.push_back(info);
		}
		
		idx += 5 + info->ES_info_length;
	}
}