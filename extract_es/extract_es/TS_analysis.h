#ifndef TS_ANALYSIS_
#define TS_ANALYSIS_

#include "def.h"
#include "TS_data.h"
#include "PAT.h"
#include "PMT.h"
#include "PES.h"

#define PACKET_LENGTH 188
#define MAX_TS_LENGTH 65536

class TS_ANALYSIS
{
public:
	TS_ANALYSIS(int8_t *file);
	~TS_ANALYSIS();

	TS_DATA *TS_data;
	PAT *TS_pat;
	PMT *TS_pmt;
	PES *TS_pes;
	ADAPT_FIELDS *TS_adapt;
	
	//��ȡ������
	uint32_t get_packet_length(FILE *packet, uint32_t *position, uint32_t length);

	//map����Ŀ��PMT_PID����Ӧ��ĿPMT_PID������Ϣ
	void get_detail(std::map<uint32_t, std::vector<STREAM_INFO*>> &stream_info);

	//pes es��ȡ
	void get_pes_es(uint16_t es_PID, int8_t *pes_path, int8_t *es_path);

	//��ȡTS������
	void get_bitrate(uint32_t time_interval);	//��λ��s
	
protected:
private:
	std::vector<PROG_INFO*> prog_info;
	std::vector<STREAM_INFO*> stream_info;
	int8_t in_ts_file[128];

};


#endif