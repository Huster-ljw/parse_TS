#ifndef PES_
#define PES_

#include "def.h"

class PES
{
public:
	enum {INVALID_CC = 0x10};
	enum {MAX_TS_LENGTH = 65536};

	PES();
	PES(uint8_t *data);
	~PES();

	void Reset();
	void get_detail();
	
public:
	uint8_t		*raw_data;
	
public:
	//uint32_t	packet_start_code_prefix;			//24
	//uint32_t	stream_id;							//8
	//uint32_t	PES_packet_length;					//16


	//uint32_t	fixed_value1;						//bit 2:'10'
	//uint32_t	PES_scrambling_control;				//2
	//uint32_t	PES_priority;						//2
	//uint32_t	data_alignment_indicator;			//1
	//uint32_t	copyright;							//1
	//uint32_t	original_or_copy;					//1
	//uint32_t	PTS_DTS_flags;						//2
	//uint32_t	ESCR_flag;							//1
	//uint32_t	ES_rate_flag;						//1
	//uint32_t	DSM_trick_mode_flag;				//1
	//uint32_t	additional_copy_info_flag;			//1
	//uint32_t	PES_CRC_flag;						//1
	//uint32_t	PES_extension_flag;					//1
	//uint32_t	PES_header_data_length;				//8
	//
	////when PTS_DTS_flags == 0x02 or 0x03
	//uint32_t	fixed_value2;						//4:'0010'
	//uint64_t	PTS;								//33
	//uint64_t	DTS;								//33

	////when ESCR_flag == 0x01
	//uint64_t	ESCR_base;							//33
	//uint32_t	ESCR_extension;						//9

	////ES_rate_flag == 0x01
	//uint32_t	ES_rate;							//22

	////......

	////DSM_trick_mode_flag == 1
	//uint32_t	trick_mode_control;					//3

	//uint32_t	field_id;							//2
	//uint32_t	intra_slice_refresh;				//1
	//uint32_t	frequency_truncation;				//2
	//uint32_t	rep_cntrl;							//5

	//additional_copy_info_flag == 1
	//uint32_t	additional_copy_info;				//7

	//PES_CRC_flag == 1
	//uint32_t	previous_PES_packet_CRC;			//16

	////PES_extension_flag == 1
	//uint32_t	PES_private_data_flag;				//1
	//uint32_t	pack_header_field_flag;				//1
	//uint32_t	prog_packet_sequence_counter_flag;	//1
	//uint32_t	PSTD_buffer_flag;					//1
	//uint32_t	PES_extension_flag_2;				//1

	////PES_private_data_flag == 1
	//uint32_t	PES_private_data[4];				//128

	////pack_header_field_flag == 1
	//uint32_t	pack_field_length;					//8

	////prog_packet_sequence_counter_flag == 1
	//uint32_t	prog_packet_sequence_counter;		//7
	//uint32_t	MPEG1_MPEG2_identifier;				//1
	//uint32_t	original_stuff_length;				//6

	////PSTD_buffer_flag == 1
	//uint32_t	PSTD_buffer_scale;					//1
	//uint32_t	PSTD_buffer_size;					//13

	////PES_extension_flag_2 == 1
	//uint32_t	PES_extension_field_length;			//7


	//////////pes ×Ö¶Î
	uint32_t packet_start_code_prefix; //24 bit
	uint32_t stream_id; // 8 bit
	uint32_t PES_packet_length; //16 bit

	
	uint32_t PES_scrambling_control; //2 bit
	uint32_t PES_priority; // 2 bit
	uint32_t data_alignment_indicator; // 1 bit
	uint32_t copyright; // 1 bit
	uint32_t original_or_copy; // 1 bit
	uint32_t PTS_DTS_flags; // 2 bit
	uint32_t ESCR_flag; // 1 bit
	uint32_t ES_rate_flag; // 1 bit
	uint32_t DSM_trick_mode_flag; //1 bit
	uint32_t additional_copy_info_flag; // 1 bit
	uint32_t PES_CRC_flag; //1 bit
	uint32_t PES_extension_flag; // 1 bit
	uint32_t PES_header_data_length; //8 bit

	//PTS_DTS_flag != 00
	int64_t PTS; // 33 bit
	int64_t DTS; // 33 bit

	//ESCR_flag == true
	uint64_t ESCR_base; // 33 bit
	uint32_t ESCR_extension; //9 bit

	//ES_rate_flag == true
	uint32_t ES_rate; // 22 bit

	//DSM_trick_mode_flag
	uint32_t trick_mode_control; //3 bit

	uint32_t field_id; // 2 bit
	uint32_t intra_slice_refresh; // 1 bit
	uint32_t frequency_truncation; // 2 bit
	uint32_t rep_cntrl; //5 bit

	//additional_copy_info_flag == true
	uint32_t additional_copy_info; // 7 bit

	//PES_CRC_flag == 1
	uint32_t previous_PES_packet_CRC;

	//PES_extension_flag == true
	uint32_t PES_private_data_flag; // 1 bit
	uint32_t pack_header_field_flag; // 1 bit
	uint32_t program_packet_sequence_counter_flag; // 1 bit
	uint32_t P_STD_buffer_flag; // 1 bit
	uint32_t PES_extension_flag_2; // 1 bit

	//PES_private_data_flag == true
	uint32_t PES_private_data[4]; // 128 bit

	//pack_header_field_flag
	uint32_t pack_field_length; // 8 bit
	uint32_t* pack_header;

	//program_packer_sequence_counter_flag == true
	uint32_t program_packet_sequence_counter; // 7 bit
	uint32_t MPEG1_MPEG2_indentifier; // 1 bit
	uint32_t original_stuff_length; // 6 bit

	//p_STD_buffer_flag == true
	uint32_t P_STD_buffer_scale; // 1 bit
	uint32_t P_STD_buffer_size; // 13 bit

	//PES_extension_flag_2 == true
	uint32_t PES_extension_field_length;

	uint8_t* PES_packet_data;
	uint32_t PES_packet_data_length;

};


#endif