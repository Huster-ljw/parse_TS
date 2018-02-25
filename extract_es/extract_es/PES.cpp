#include "PES.h"

const uint32_t PROGRAM_STREAM_MAP = 0xBC;
const uint32_t PADDING_STREAM = 0xBE;
const uint32_t PRIVATE_STREAM_2 = 0xBF;
const uint32_t ECM_STREAM = 0xF0;
const uint32_t EMM_STREAM = 0xF1;
const uint32_t PROGRAM_STREAM_DIRECTORY = 0xFF;
const uint32_t DSMCC_STREAM = 0xF2;
const uint32_t E_STREAM = 0xF8;


PES::PES()
{
	
}

PES::PES(uint8_t *data)
		: raw_data(data),
		  packet_start_code_prefix((raw_data[0] << 16) | (raw_data[1] << 8) | raw_data[2]),
		  stream_id(raw_data[3]),
		  PES_packet_length((raw_data[4] << 8) | raw_data[5])
{
	
}

PES::~PES()
{
	
}

void PES::Reset()
{
	packet_start_code_prefix = 0;
	stream_id = 0;
	PES_packet_length = 0;

	PES_scrambling_control = 0;
	PES_priority = 0;
	data_alignment_indicator = false;
	copyright = false;
	original_or_copy = false;
	PTS_DTS_flags = 0;
	ESCR_flag = false;
	ES_rate_flag = false;
	DSM_trick_mode_flag = false;
	additional_copy_info_flag = false;
	PES_CRC_flag = false;
	PES_extension_flag = false;
	PES_header_data_length = 0;

	//PTS_DTS_flag != 00
	PTS = 0;
	DTS = 0;

	//ESCR_flag == true
	ESCR_base = 0;
	ESCR_extension = 0;

	//ES_rate_flag == true
	ES_rate = 0;

	//DSM_trick_mode_flag
	trick_mode_control = 0;

	field_id = 0;
	intra_slice_refresh = 0;
	frequency_truncation = 0;
	rep_cntrl = 0;

	//additional_copy_info_flag == true
	additional_copy_info = 0;

	//PES_CRC_flag == 1
	previous_PES_packet_CRC = 0;

	//PES_extension_flag == true
	PES_private_data_flag = false;
	pack_header_field_flag = false;
	program_packet_sequence_counter_flag = false;
	P_STD_buffer_flag = false;
	PES_extension_flag_2 = false;

	//PES_private_data_flag == true
	//PES_private_data[16] = {0};

	//pack_header_field_flag
	pack_field_length = 0;
	//pack_header = free();

	//program_packer_sequence_counter_flag == true
	program_packet_sequence_counter = 0;
	MPEG1_MPEG2_indentifier = 0;
	original_stuff_length = 0;

	//p_STD_buffer_flag == true
	P_STD_buffer_scale = 0;
	P_STD_buffer_size = 0;

	//PES_extension_flag_2 == true
	PES_extension_field_length = 0;

	PES_packet_data = NULL;
	PES_packet_data_length = 0;
}


void PES::get_detail()
{
	int idx = 0;
	packet_start_code_prefix = (raw_data[0] << 16) | (raw_data[1] << 8) | raw_data[2];
	stream_id = raw_data[3];
	PES_packet_length = (raw_data[4] << 8) | raw_data[5];
	idx += 6;
	if(stream_id != PROGRAM_STREAM_MAP &&
		stream_id != PADDING_STREAM &&
		stream_id != PRIVATE_STREAM_2 &&
		stream_id != ECM_STREAM &&
		stream_id != EMM_STREAM &&
		stream_id != PROGRAM_STREAM_DIRECTORY &&
		stream_id != DSMCC_STREAM && 
		stream_id != E_STREAM)
	{
		PES_scrambling_control = (raw_data[idx] >> 4) & 0x3;
		PES_priority = (raw_data[idx] >> 3) & 0x1;
		data_alignment_indicator = (raw_data[idx] >> 2) & 0x1;
		copyright = (raw_data[idx] >> 1) & 0x1;
		original_or_copy = raw_data[idx] & 0x1;
		++idx;

		PTS_DTS_flags = raw_data[idx] >> 6;
		ESCR_flag = (raw_data[idx] >> 5) & 0x1;
		ES_rate_flag = (raw_data[idx] >> 4) & 0x1;
		DSM_trick_mode_flag = (raw_data[idx] >> 3) & 0x1;
		additional_copy_info_flag = (raw_data[idx] >> 2) & 0x1;
		PES_CRC_flag = (raw_data[idx] >> 1) & 0x1;
		PES_extension_flag = raw_data[idx] & 0x1;
		++idx;

		PES_header_data_length = raw_data[idx++];
		PES_packet_data = raw_data + idx + PES_header_data_length;

		PES_packet_data_length = PES_packet_data - raw_data;

		if(PTS_DTS_flags == 0x02)
		{
			PTS = ((((int64_t)raw_data[idx] >> 1) & 0x7) << 30) | (raw_data[idx + 1] << 22) | 
				((raw_data[idx + 2] >> 1) << 15) | (raw_data[idx + 3] << 7) | (raw_data[idx + 4] >> 1);
			DTS = PTS;
			idx += 5;
		}

		if(PTS_DTS_flags == 0x03)
		{
			PTS = ((((int64_t)raw_data[idx] >> 1) & 0x7) << 30) | (raw_data[idx + 1] << 22) | 
				((raw_data[idx + 2] >> 1) << 15) | (raw_data[idx + 3] << 7) | (raw_data[idx + 4] >> 1);
			idx += 5;

			DTS = ((((int64_t)raw_data[idx] >> 1) & 0x7) << 30) | (raw_data[idx + 1] << 22) | 
				((raw_data[idx + 2] >> 1) << 15) | (raw_data[idx + 3] << 7) | (raw_data[idx + 4] >> 1);
			idx += 5;
		}

		if(ESCR_flag)
		{
			ESCR_base = ((((int64_t)raw_data[idx] >> 3) & 0x7) << 30) | ((raw_data[idx] & 0x3) << 28) | (raw_data[idx + 1] << 20) | 
				((raw_data[idx + 2] >> 3) << 15) | ((raw_data[idx + 2] & 0x03) << 13) | (raw_data[idx + 3] << 5) |
				(raw_data[idx + 4] >> 3);
			ESCR_extension = ((raw_data[idx + 4] & 0x03) << 7) | (raw_data[idx + 5] >> 1);  
			idx += 6;
		}

		if(ES_rate_flag)
		{
			ES_rate = ((raw_data[idx] & 0x7F) << 15) | (raw_data[idx + 1] << 7) | (raw_data[idx + 2] >>1);
			idx += 3;
		}

		if(DSM_trick_mode_flag)
		{
			trick_mode_control = raw_data[idx] >> 5;
			if(trick_mode_control == 0x0 || trick_mode_control == 0x3)
			{
				field_id = (raw_data[idx] >> 3) & 0x3;
				intra_slice_refresh = (raw_data[idx] >> 2) & 0x1;
				frequency_truncation = raw_data[idx] & 0x3;
			}
			else if(trick_mode_control == 0x1 || trick_mode_control == 0x4)
			{
				rep_cntrl = raw_data[idx] & 0x1F;
			}
			else if(trick_mode_control == 0x2)
			{
				field_id = (raw_data[idx] >> 3) & 0x3;
			}
			++idx;
		}

		if(additional_copy_info_flag)
		{
			additional_copy_info = raw_data[idx++] & 0x7F;
		}

		if(PES_CRC_flag)
		{
			previous_PES_packet_CRC = (raw_data[idx] << 8) | raw_data[idx + 1];
			idx += 2;
		}

		if(PES_extension_flag)
		{
			PES_private_data_flag = raw_data[idx] >> 7;
			pack_header_field_flag = (raw_data[idx] >> 6) & 0x1;
			program_packet_sequence_counter_flag = (raw_data[idx] >> 5) & 0x1;
			P_STD_buffer_flag = (raw_data[idx] >> 4) & 0x1;
			PES_extension_flag_2 = raw_data[idx] & 0x1;
			++idx;

			if(PES_private_data_flag)
				idx += 16;

			if(pack_header_field_flag)
			{
				pack_field_length = raw_data[idx++];
				idx += pack_field_length;
			}

			if(program_packet_sequence_counter_flag)
			{
				program_packet_sequence_counter = raw_data[idx] & 0x7F;
				MPEG1_MPEG2_indentifier = (raw_data[idx + 1] >> 6) & 0x1;
				original_stuff_length = raw_data[idx + 1] & 0x3F;
				idx += 2;
			}

			if(P_STD_buffer_flag)
			{
				P_STD_buffer_scale = (raw_data[idx] >> 5) & 0x1;
				P_STD_buffer_size = ((raw_data[idx] & 0x1F) << 8) | raw_data[idx + 1];
				idx += 2;
			}
		}
	}
	else if(stream_id == PADDING_STREAM)
	{
		PES_packet_data_length = 0;
	}
	else
	{
		PES_packet_data = raw_data + 6;
		PES_packet_data_length = 6;
	}
}