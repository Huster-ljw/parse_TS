#ifndef PAT_
#define PAT_

#include "def.h"
#include "section.h"

class PAT
{
public:
	PAT(uint8_t *buffer);
	~PAT();

	void get_prog_detail(std::vector<PROG_INFO*> &prog_info);
	void get_CRC32();
	
private:
	uint8_t *raw_data;
public:
	uint32_t	table_id;					//8
	uint32_t	section_syntax_indicator;	//1
	uint32_t	zero;						//1
	uint32_t	reserved1;					//2
	uint32_t	section_length;				//12
	uint32_t	transport_stream_id;		//16
	uint32_t	reserved2;					//2
	uint32_t	version_number;				//5
	uint32_t	current_next_indicator;		//1
	uint32_t	section_number;				//8
	uint32_t	last_section_number;		//8
	uint32_t	CRC_32;						//32
};


#endif