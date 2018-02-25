#ifndef DEF_
#define DEF_

#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include "excp.h"


typedef unsigned char uint8_t;
typedef char int8_t;

typedef unsigned short uint16_t;
typedef short int16_t;

typedef unsigned int uint32_t;
typedef int int32_t;

typedef unsigned long long uint64_t;
typedef long long int64_t;

const int TS_PACKET_SIZE = 188;
const int TS_DVHS_PACKET_SIZE = 192;
const int TS_FEC_PACKET_SIZE = 204;
const int TS_MAX_PACKET_SIZE = 204;
const int MAX_PID_NUM = 8192;

const int MAX_PCR_NUM = 100000;
const int MAX_PTS_NUM = MAX_PCR_NUM;
const int MAX_DTS_NUM = MAX_PCR_NUM;

const int TS_TYPE_PES = 1;
const int TS_TYPE_SECTION = 2;

const int INVALIDATE_PCR_PID = 0x1FFF;

/* TS packet type */
#define TS_TMSK_BASE    (0x00FF) /* BIT[7:0]: base type mask */
#define TS_TMSK_PCR     (0x0100) /* BIT[8:8]: 0(without PCR), 1(with PCR) */

#define TS_TYPE_ANY     (0x0000)
#define TS_TYPE_UNO     (0x0000)
#define TS_TYPE_NUL     (0x0001)
#define TS_TYPE_RSV     (0x0002)
#define TS_TYPE_BAD     (0x0003)

#define TS_TYPE_PAT     (0x0010)
#define TS_TYPE_CAT     (0x0011)
#define TS_TYPE_TSDT    (0x0012)
#define TS_TYPE_NIT     (0x0013)
#define TS_TYPE_ST      (0x0014)
#define TS_TYPE_SDT     (0x0015)
#define TS_TYPE_BAT     (0x0016)
#define TS_TYPE_EIT     (0x0017)
#define TS_TYPE_RST     (0x0018)
#define TS_TYPE_TDT     (0x0019)
#define TS_TYPE_TOT     (0x001A)
#define TS_TYPE_NS      (0x001B)
#define TS_TYPE_INB     (0x001C)
#define TS_TYPE_MSU     (0x001D)
#define TS_TYPE_DIT     (0x001E)
#define TS_TYPE_SIT     (0x001F)
#define TS_TYPE_EMM     (0x0020)

#define TS_TYPE_USR     (0x00C0)
#define TS_TYPE_PMT     (0x00C1)
#define TS_TYPE_VID     (0x00C2)
#define TS_TYPE_AUD     (0x00C3)
#define TS_TYPE_ECM     (0x00C4)

#define TS_TYPE_PCR     (TS_TYPE_USR | TS_TMSK_PCR)
#define TS_TYPE_VIDP    (TS_TYPE_VID | TS_TMSK_PCR)
#define TS_TYPE_AUDP    (TS_TYPE_AUD | TS_TMSK_PCR)
#define TS_TYPE_NULP    (TS_TYPE_NUL | TS_TMSK_PCR)

#define WITH_PCR(x)     (((x) & TS_TMSK_PCR ) == TS_TMSK_PCR)
#define IS_TYPE(t, x)   (((x) & TS_TMSK_BASE) == (t))

struct pid_type_table {
	int   type; /* TS_TYPE_xxx */
	char *sdes; /* short description */
	char *ldes; /* long description */
};

const struct pid_type_table PID_TYPE_TABLE[] = {
	{TS_TYPE_PAT , "PAT ", "program association section"},
	{TS_TYPE_CAT , "CAT ", "conditional access section"},
	{TS_TYPE_TSDT, "TSDT", "transport stream description section"},
	{TS_TYPE_RSV , "RSV ", "reserved"},
	{TS_TYPE_NIT , "NIT ", "network information section"},
	{TS_TYPE_ST  , "ST  ", "stuffing section"},
	{TS_TYPE_SDT , "SDT ", "service description section"},
	{TS_TYPE_BAT , "BAT ", "bouquet association section"},
	{TS_TYPE_EIT , "EIT ", "event information section"},
	{TS_TYPE_RST , "RST ", "running status section"},
	{TS_TYPE_TDT , "TDT ", "time data section"},
	{TS_TYPE_TOT , "TOT ", "time offset section"},
	{TS_TYPE_NS  , "NS  ", "network synchroniztion"},
	{TS_TYPE_INB , "INB ", "inband signaling"},
	{TS_TYPE_MSU , "MSU ", "measurement"},
	{TS_TYPE_DIT , "DIT ", "discontinuity information section"},
	{TS_TYPE_SIT , "SIT ", "selection information section"},
	{TS_TYPE_USR , "USR ", "user define"},
	{TS_TYPE_PMT , "PMT ", "program map section"},
	{TS_TYPE_VID , "VID ", "video packet"},
	{TS_TYPE_AUD , "AUD ", "audio packet"},
	{TS_TYPE_PCR , "PCR ", "program counter reference"},
	{TS_TYPE_VIDP, "VIDP", "video packet with PCR"},
	{TS_TYPE_AUDP, "AUDP", "audio packet with PCR"},
	{TS_TYPE_ECM , "ECM ", "entitle control message"},
	{TS_TYPE_EMM , "EMM ", "entitle manage message"},
	{TS_TYPE_NUL , "NUL ", "empty packet"},
	{TS_TYPE_NULP, "NULP", "empty packet with PCR"},
	{TS_TYPE_UNO , "UNO ", "unknown"},
	{TS_TYPE_BAD , "BAD ", "illegal"} /* TS_TYPE_BAD is loop stop condition! */
};

struct stream_type_table {
	uint8_t stream_type;
	int   type; /* TS_TYPE_xxx */
	char *sdes; /* short description */
	char *ldes; /* long description */
};

static const struct stream_type_table STREAM_TYPE_TABLE[] = {
	{0x00, TS_TYPE_RSV, "Reserved", "ITU-T|ISO/IEC Reserved"}, //0
	{0x01, TS_TYPE_VID, "MPEG-1", "ISO/IEC 11172-2 Video"}, //1
	{0x02, TS_TYPE_VID, "MPEG-2", "ITU-T Rec.H.262|ISO/IEC 13818-2 Video or MPEG-1 parameter limited"},//2
	{0x03, TS_TYPE_AUD, "MPEG-1", "ISO/IEC 11172-3 Audio"},//3
	{0x04, TS_TYPE_AUD, "MPEG-2", "ISO/IEC 13818-3 Audio"},//4
	{0x05, TS_TYPE_USR, "Private", "ITU-T Rec.H.222.0|ISO/IEC 13818-1 private_sections"},//5
	{0x06, TS_TYPE_AUD, "AC3|TT|LPCM", "ITU-T Rec.H.222.0|ISO/IEC 13818-1 PES packets containing private data|Dolby Digital DVB|Linear PCM"},//6
	{0x07, TS_TYPE_USR, "MHEG", "ISO/IEC 13522 MHEG"},//7
	{0x08, TS_TYPE_USR, "DSM-CC", "ITU-T Rec.H.222.0|ISO/IEC 13818-1 Annex A DSM-CC"},//8
	{0x09, TS_TYPE_USR, "H.222.1", "ITU-T Rec.H.222.1"},//9
	{0x0A, TS_TYPE_USR, "13818-6 type A", "ISO/IEC 13818-6 type A: Multi-protocol Encapsulation"},//10
	{0x0B, TS_TYPE_USR, "13818-6 type B", "ISO/IEC 13818-6 type B: DSM-CC U-N Messages"},//11
	{0x0C, TS_TYPE_USR, "13818-6 type C", "ISO/IEC 13818-6 type C: DSM-CC Stream Descriptors"},//12
	{0x0D, TS_TYPE_USR, "13818-6 type D", "ISO/IEC 13818-6 type D: DSM-CC Sections or DSM-CC Addressable Sections"},//13
	{0x0E, TS_TYPE_USR, "Auxiliary", "ITU-T Rec.H.222.0|ISO/IEC 13818-1 auxiliary"},//14
	{0x0F, TS_TYPE_AUD, "AAC ADTS", "ISO/IEC 13818-7 Audio with ADTS transport syntax"},//15
	{0x10, TS_TYPE_VID, "MPEG-4", "ISO/IEC 14496-2 Visual"},//16
	{0x11, TS_TYPE_AUD, "AAC LATM", "ISO/IEC 14496-3 Audio with LATM transport syntax"},//17
	{0x12, TS_TYPE_AUD, "MPEG-4", "ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in PES packets"},//18
	{0x13, TS_TYPE_AUD, "MPEG-4", "ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in ISO/IEC 14496_sections"},//19
	{0x14, TS_TYPE_USR, "MPEG-2", "ISO/IEC 13818-6 Synchronized Download Protocol"},//20
	{0x15, TS_TYPE_USR, "MPEG-2", "Metadata carried in PES packets"},//21
	{0x16, TS_TYPE_USR, "MPEG-2", "Metadata carried in metadata_sections"},//22
	{0x17, TS_TYPE_USR, "MPEG-2", "Metadata carried in ISO/IEC 13818-6 Data Carousel"},//23
	{0x18, TS_TYPE_USR, "MPEG-2", "Metadata carried in ISO/IEC 13818-6 Object Carousel"},//24
	{0x19, TS_TYPE_USR, "MPEG-2", "Metadata carried in ISO/IEC 13818-6 Synchronized Dowload Protocol"},//25
	{0x1A, TS_TYPE_USR, "IPMP", "IPMP stream(ISO/IEC 13818-11, MPEG-2 IPMP)"},//26
	{0x1B, TS_TYPE_VID, "H.264", "ITU-T Rec.H.264|ISO/IEC 14496-10 Video"},//27
	{0x1C, TS_TYPE_AUD, "MPEG-4", "ISO/IEC 14496-3 Audio, without using any additional transport syntax, such as DST, ALS and SLS"},//28
	{0x1D, TS_TYPE_USR, "MPEG-4", "ISO/IEC 14496-17 Text"},//29
	{0x1E, TS_TYPE_VID, "MPEG-4", "Auxiliary video stream as defined in ISO/IEC 23002-3"},//30
	{0x20, TS_TYPE_VID, "MVC?", "H.264 MVC?"},//31
	{0x24, TS_TYPE_VID, "H.265", "HEVC(High Efficiency Video Coding)"},//32
	{0x42, TS_TYPE_VID, "AVS", "Advanced Video Standard"},//33
	{0x7F, TS_TYPE_USR, "IPMP", "IPMP stream"},//34
	{0x80, TS_TYPE_VID, "SVAC|LPCM", "SVAC, LPCM of ATSC"},//35
	{0x81, TS_TYPE_AUD, "AC3", "Dolby Digital ATSC"},//36
	{0x82, TS_TYPE_AUD, "DTS", "DTS Audio"},//37
	{0x83, TS_TYPE_AUD, "MLP", "MLP"},//38
	{0x84, TS_TYPE_AUD, "DDP", "Dolby Digital Plus"},//39
	{0x85, TS_TYPE_AUD, "DTSHD", "DTSHD"},//40
	{0x86, TS_TYPE_AUD, "DTSHD_XLL", "DTSHD_XLL"},//41
	{0x90, TS_TYPE_AUD, "G.711", "G.711(A)"},//42
	{0x92, TS_TYPE_AUD, "G.722.1", "G.722.1"},//43
	{0x93, TS_TYPE_AUD, "G.723.1", "G.723.1"},//44
	{0x99, TS_TYPE_AUD, "G.729", "G.729"},//45
	{0x9A, TS_TYPE_AUD, "AMR-NB", "AMR-NB"},//46
	{0x9B, TS_TYPE_AUD, "SVAC", "SVAC"},//47
	{0xA1, TS_TYPE_AUD, "DDP_2", "Dolby Digital Plus"},//48
	{0xA2, TS_TYPE_AUD, "DTSHD_2", "DTSHD_2"},//49
	{0xEA, TS_TYPE_VID, "VC1", "VC1"},//50
	{0xEA, TS_TYPE_AUD, "WMA", "WMA"},//51
	{0xF0, TS_TYPE_EMM, "EMM", "EMM"},//52
	{0xF1, TS_TYPE_ECM, "ECM", "ECM"},//53

	{0xF2, TS_TYPE_AUD, "DRA", "Digital Rise Audio"},// lc add, temp used // 54 
	{0xFF, TS_TYPE_UNO, "UNKNOWN", "Unknown stream"} /* 0xFF is loop stop condition! */ // 55
};
#endif