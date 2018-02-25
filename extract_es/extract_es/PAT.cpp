#include "PAT.h"

PAT::PAT(uint8_t *buffer)
	: raw_data(buffer),
	  table_id(raw_data[0]),
	  section_syntax_indicator(raw_data[1] >> 7),
	  zero((raw_data[1] >> 6) & 0x01),
	  reserved1((raw_data[1] >> 4) & 0x03),
	  section_length((raw_data[1] & 0x0F) << 8 | raw_data[2]),
	  transport_stream_id(raw_data[3] << 8 | raw_data[4]),
	  reserved2(raw_data[5] >> 6),
	  version_number((raw_data[5] >> 1) & 0x1F),
	  current_next_indicator(raw_data[5] & 0x01),
	  section_number(raw_data[6]),
	  last_section_number(raw_data[7])
{
	if (table_id != 0x00)
	{
		throw new PatErr(PatErr::PTID);
	}
}

PAT::~PAT()
{/*
	std::vector<PROG_INFO*>::iterator iter;

	for (iter = prog_info.begin(); iter != prog_info.end(); ++iter)
	{
		delete (*iter);
	}
	prog_info.clear();*/
}

void PAT::get_prog_detail(std::vector<PROG_INFO*> &prog_info)
{
	uint32_t idx = 8;

	if (!raw_data)
	{
		return;
	}
	
	while ( idx < section_length - 4)
	{
		PROG_INFO *info = new PROG_INFO(raw_data + idx);

		prog_info.push_back(info);
		idx += 4;
	}
}

void PAT::get_CRC32()
{
	int len = 0;

	len = 3 + section_length;

	CRC_32 = (raw_data[len - 4] & 0x000000FF) << 24
			| (raw_data[len - 3] & 0x000000FF) << 16
			| (raw_data[len - 3] & 0x000000FF) << 8
			| (raw_data[len - 1] & 0x000000FF);
}