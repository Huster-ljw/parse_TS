#include "TS_analysis.h"


int main()
{
	int8_t pes_path[] = "pes_file.pes";
	int8_t es_path[] = "es_file.es";
	std::vector<STREAM_INFO*> stream_info;
	std::vector<STREAM_INFO*>::iterator stream_iter;
	std::map<uint32_t, std::vector<STREAM_INFO*>> info;
	std::map<uint32_t, std::vector<STREAM_INFO*>>::iterator iter;	
	char ts_path[] = /*"cc_mepg2.ts";*/"mpeg.ts";
	TS_ANALYSIS *TS_analysis = new TS_ANALYSIS(ts_path);
	uint16_t pid = 0;

	TS_analysis->get_detail(info);

	for (iter = info.begin(); iter != info.end(); ++iter)
	{
		std::cout << "PMT PID " << iter->first << std::endl;
		stream_info = iter->second;
		for (stream_iter = stream_info.begin(); stream_iter != stream_info.end(); ++stream_iter)
		{
			std::cout << "stream type " << (*stream_iter)->stream_type << ", elementary PID " 
					<< (*stream_iter)->elementary_PID << ", ES info length " << (*stream_iter)->ES_info_length << std::endl;				
		}
	}

	std::cout << std::endl;

	std::cout << "Please input the elementary PID you want to save£º";
	std::cin >> pid;

	TS_analysis->get_pes_es(pid, pes_path, es_path);
	
	return 0;
}