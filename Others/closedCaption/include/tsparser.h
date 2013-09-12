/**
	mpeg2 transport stream parser
*/
#ifndef TSPARSER_H_
#define TSPARSER_H_

/** reference: ISO/IEC 13818-1:2000/Cor.2:2002(E) */

#define DMX_FILE_OPEN_READ 	0x1
#define DMX_FILE_OPEN_WRITE 0x2
#define DMX_FILE_SEEK_SET 	0x1
#define DMX_FILE_SEEK_END 	0x2

#define TS_SYNC_BYTE		0x47

typedef struct ts_adaptation_field_
{
	unsigned char	adaptation_field_length;
	/** TODO: need to fill other fields ... */
} ts_adaptation_field;

typedef struct ts_packet_header_
{
	unsigned char	transport_error_indicator;
	unsigned char	payload_unit_start_indicator;
	unsigned char	transport_priority;
	unsigned short	PID;
	unsigned char	transport_scrambling_control;
	unsigned char	adaptation_field_control;
	unsigned char	continuity_counter;
	ts_adaptation_field adaptation_field;
	unsigned char * data_byte;
	unsigned short  data_byte_len;
}ts_packet_header;

typedef struct pes_header_
{
	unsigned char	stream_id;
	unsigned short	PES_packet_length;
	unsigned char	PES_scrambling_control;
	unsigned char	PES_priority;
	unsigned char	data_alignment_indicator;
	unsigned char	copyright;
	unsigned char	original_or_copy;
	unsigned char	PTS_DTS_flags;
	unsigned char	ESCR_flag;
	unsigned char	ES_rate_flag;
	unsigned char	DSM_trick_mode_flag;
	unsigned char	additional_copy_info_flag;
	unsigned char	PES_CRC_flag;
	unsigned char	PES_extension_flag;
	unsigned char	PES_header_data_length;
	__int64			PTS;
	__int64			DTS;
	__int64			ESCR_base;
	unsigned short	ESCR_extension;
	unsigned int	ES_rate;
	unsigned char	trick_mode_control;
	unsigned char	field_id;
	unsigned char	intra_slice_refresh;
	unsigned char	frequency_truncation;
	unsigned char	rep_cntrl;
	unsigned char	additional_copy_info;
	unsigned short	previous_PES_packet_CRC;
	
	unsigned char *	PES_packet_data_byte;
	
} pes_header;

typedef struct dmx_stream_operations_
{
	void *(*open)  (char *filename, int mode);
	void (*close) (void *fp);
	int (*read)  (void *fp, unsigned char *, unsigned int);
	int (*write) (void *fp, const unsigned char *, unsigned int);
	int (*seek) (void *fp, int offset, int where);
	int (*tell) (void *fp);
	int (*size) (void *fp);
}dmx_stream_operations;


typedef struct dmx_stream_
{
	const dmx_stream_operations * ops;
	void * fp;
	unsigned char ch;
	unsigned char available;
}dmx_stream;

typedef struct ts_buffer_
{
	unsigned char * begin;
	unsigned char * end;
	unsigned char * cur;
} ts_buffer;

extern const dmx_stream_operations dmx_buffer_operations;

/** packet_size is 188 but some cases it is bigger than 188, like 204.
*/
unsigned int ts_get_bits(dmx_stream * str, int nbits);
int	ts_get_packet_header(unsigned char * packet, ts_packet_header * packet_header);
int ps_get_pes(dmx_stream * str, pes_header * pes);
int dmx_byte_aligned(dmx_stream * str);

#define STR_READ(str,buf,nbytes) (str)->ops->read((str)->fp,(char *)buf,nbytes)
#define STR_TELL(str) (str)->ops->tell((str)->fp)
		
#endif
