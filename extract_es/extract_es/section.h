#ifndef SECTION_
#define SECTION_

#include "def.h"

class PROG_INFO
{
public:
	PROG_INFO(uint8_t *data);
	~PROG_INFO();
public:
	uint32_t	program_number;			//16
	uint32_t	reserved;				//3
	uint32_t	PMT_PID;				//13
private:
};

class STREAM_INFO
{
public:
	STREAM_INFO(uint8_t *data);
	~STREAM_INFO();

public:
	uint32_t	stream_type;			//8
	//uint32_t	reserved1;				//3
	uint32_t	elementary_PID;			//13
	//uint32_t	reserved2;				//4
	uint32_t	ES_info_length;			//12
private:
};

#endif