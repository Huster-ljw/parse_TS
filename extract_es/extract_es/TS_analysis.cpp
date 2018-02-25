#include "TS_analysis.h"


TS_ANALYSIS::TS_ANALYSIS(int8_t *file)
{
	strcpy(in_ts_file, file);
}

TS_ANALYSIS::~TS_ANALYSIS()
{
}

uint32_t judge_packet_length(FILE *packet, uint32_t position, uint32_t length)
{
	uint32_t i = 0;
	uint32_t sync_byte = 0;

	if (-1 == fseek(packet, position + 1, SEEK_SET))
	{
		printf("packet is end\n");

		return -1;
	}

	//如果连续10次包长均为length，则认为包长为length
	for (i = 0; i < 10; ++i)
	{
		if (-1 == fseek(packet, length - 1, SEEK_CUR))
		{
			return -1;
		}
		if (feof(packet))
		{
			return -1;
		}
		sync_byte = fgetc(packet);

		if (0x47 != sync_byte)
		{
			printf("sync byte %x of the %d times\n", sync_byte, i);
			return -1;
		}
	}
	return length;
}

uint32_t TS_ANALYSIS::get_packet_length(FILE *packet, uint32_t *position, uint32_t length)
{
	uint32_t sync_byte = 0;

	while (!feof(packet))
	{
		sync_byte = fgetc(packet);

		if (0x47 == sync_byte)
		{
			if (PACKET_LENGTH == judge_packet_length(packet, *position, length))
			{
				return PACKET_LENGTH;
			}
		}

		(*position)++;
		if (-1 == fseek(packet, *position, SEEK_SET))
		{
			printf("the file error\n");
			return -1;
		}
	}

	printf("the file if not the transport stream\n");
	return -1;
}

void TS_ANALYSIS::get_detail(std::map<uint32_t, std::vector<STREAM_INFO*>> &info)
{
	std::set<uint32_t> iset;
	int8_t *ts_path = in_ts_file;
	FILE *ts_file = fopen(ts_path, "rb");
	uint8_t *buffer_data = (uint8_t *)malloc(PACKET_LENGTH);
	uint32_t patket_length = 0;
	uint32_t position = 0;
	uint32_t load_position = 0;
	uint32_t pat_flag = 0;		//pat表只需要解析一次即可
	uint32_t prog_size = 0;

	patket_length = get_packet_length(ts_file, &position, PACKET_LENGTH);

	fseek(ts_file, position, SEEK_SET);

	while (!feof(ts_file))
	{
		if (-1 == fread(buffer_data, patket_length, 1, ts_file))
		{
			std::cout << " read buffer data failed\n" << std::endl;
			return;
		}

		position += patket_length;
		if (-1 == fseek(ts_file, position, SEEK_SET))
		{
			std::cout << "fseek failed\n" << std::endl;
			return;
		}

		TS_data = new TS_DATA(buffer_data);

		if (TS_data->adaption_field_control == 0 || TS_data->adaption_field_control == 2)
		{
			//std::cout << "no payload\n" << std::endl;
			continue;
		}
		else if (TS_data->adaption_field_control == 1)
		{
			load_position = 4;
		}
		else
		{
			load_position = 4 + buffer_data[4] + 1;
		}
		
		if (TS_data->payload_unit_start_indicator)
		{
			load_position = load_position + buffer_data[load_position] + 1;
		}

		//pat表只需要解析一次即可
		if (TS_data->PID == 0x0000 && !pat_flag)
		{
			TS_pat = new PAT(buffer_data + load_position);
			TS_pat->get_prog_detail(prog_info);

			prog_size = prog_info.size();
			pat_flag = 1;
		}
		else
		{
			std::vector<PROG_INFO*>::iterator prog_iter;
			std::vector<STREAM_INFO*>::iterator stream_iter;
			TS_pmt = new PMT(buffer_data + load_position);

			if (TS_pmt->table_id == 0x02)
			{
				for (prog_iter = prog_info.begin(); prog_iter != prog_info.end(); ++prog_iter)
				{
					if ((*prog_iter)->program_number == TS_pmt->program_number)
					{
						TS_pmt->get_stream_detail(stream_info);

						for (stream_iter = stream_info.begin(); stream_iter != stream_info.end(); ++stream_iter)
						{
							info.insert(std::pair<uint32_t, std::vector<STREAM_INFO*>>((*prog_iter)->PMT_PID, stream_info));
							iset.insert((*prog_iter)->PMT_PID);
							if (iset.size() == prog_size)
							{
								return;
							}
						}
						stream_info.clear();
					}
				}					
			}
		}
	}	
}

void TS_ANALYSIS::get_pes_es(uint16_t pid, int8_t *pes_path, int8_t *es_path)
{	
	FILE *ts_file = fopen(in_ts_file, "rb");
	FILE *pes_file = fopen(pes_path, "wb");
	FILE *es_file = fopen(es_path, "wb");
	uint8_t buffer_data[188 * 1024] = {NULL};
	uint8_t *es_buffer = (uint8_t *)malloc(MAX_TS_LENGTH);
	uint8_t *payload_pos = NULL;
	uint32_t buffer_length = MAX_TS_LENGTH;
	uint32_t patket_length = 0;	
	uint32_t sz = 0;
	uint32_t start = 0;
	uint32_t position = 0;
	uint32_t available_length = 0;
	uint32_t recv_length = 0;
	uint32_t read_leangth = 0;
	uint32_t recv_flag = 0;
	uint8_t *p = NULL;

	//ts_file = fopen(in_ts_file, "rb");
	//pes_file = fopen(pesfile, "wb");
	if (pes_file == NULL || ts_file == NULL)
	{
		return;
	}

	patket_length = get_packet_length(ts_file, &position, PACKET_LENGTH);

	fseek(ts_file, position, SEEK_SET);

	while (!feof(ts_file))
	{
		sz = fread(buffer_data, 1, sizeof(buffer_data), ts_file);

		p = buffer_data;

		while (p < buffer_data + sz)
		{
			TS_data = new TS_DATA(p);

			if (TS_data->PID == pid)
			{
				switch(TS_data->adaption_field_control)
				{
				case 0:
				case 2:
					payload_pos = NULL;
					break;
				case 1:
					payload_pos = p + 4;
					break;
				case 3:
					payload_pos = p + 4 + p[4] + 1;
					break;
				}

				if (TS_data->payload_unit_start_indicator != 0)
				{
					start = 1;
				}

				if (start && payload_pos && pes_file)
				{
					//save pes
					fwrite(payload_pos, 1, 188 + p - payload_pos, pes_file);
					
					//save es
					if (TS_data->payload_unit_start_indicator != 0)
					{
						if (recv_length > 0)
						{
							TS_pes = new PES(es_buffer);
							TS_pes->Reset();
							TS_pes->get_detail();
							while (TS_pes->packet_start_code_prefix != 0x000001)
							{
								TS_pes->Reset();
								return;
							}

							if (TS_pes->PES_packet_data_length != 0)
							{
								fwrite(TS_pes->PES_packet_data, recv_length, 1, es_file);
							}

							memset(es_buffer, 0, recv_length);
							recv_flag = 0;
							recv_length = 0;
							buffer_length = MAX_TS_LENGTH;
						}
						recv_flag = 1;
					}

					if (recv_flag)
					{
						available_length = 188 + p - payload_pos;
						if (available_length > 0)
						{
							if (available_length + recv_length > buffer_length)
							{
								buffer_length += MAX_TS_LENGTH;
								es_buffer = (uint8_t *)realloc(es_buffer, buffer_length);
							}
							memcpy(es_buffer + recv_length, payload_pos, available_length);
							recv_length += available_length;
						}						
					}
				}
			}			
			p += 188;
		}
	}

	if (es_buffer)
	{
		free(es_buffer);
	}

	if (pes_file)
	{
		fclose(pes_file);
	}

	if (ts_file)
	{
		fclose(ts_file);
	}

	if (es_file)
	{
		fclose(es_file);
	}
	printf("\nGot pes and es! pes in %s, es in %s.\n",pes_path, es_path);
	
}