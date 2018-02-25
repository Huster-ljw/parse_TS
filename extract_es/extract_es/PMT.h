#ifndef PMT_
#define PMT_

#include "def.h"
#include "section.h"

class PMT
{
public:
	PMT(uint8_t *data);
	~PMT();
	
	void get_stream_detail(std::vector<STREAM_INFO*> &stream_info);
	
	uint32_t check_type(uint32_t type);
	
private:
	uint8_t *raw_data;
public:
	uint32_t	table_id;					//8bit
	uint32_t	section_syntax_indicator;	//1
	uint32_t	zero;						//1
	uint32_t	reserved1;					//2
	uint32_t	section_length;				//12
	uint32_t	program_number;				//16
	uint32_t	reserved2;					//2
	uint32_t	version_number;				//5
	uint32_t	current_next_indicator;		//1
	uint32_t	section_number;				//8
	uint32_t	last_section_number;		//8
	uint32_t	reserved3;					//3
	uint32_t	PCR_PID;					//13
	uint32_t	reserved4;					//4
	uint32_t	program_info_length;		//12
};


#endif