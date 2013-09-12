/*
 *  avilib.h
 *
 *  Copyright (C) Thomas �streich - June 2001
 *  multiple audio track support Copyright (C) 2002 Thomas �streich
 *
 *  Original code:
 *  Copyright (C) 1999 Rainer Johanni <Rainer@Johanni.de> 
 *
 *  This file is part of transcode, a linux video stream processing tool
 *      
 *  transcode is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *   
 *  transcode is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *   
 *  You should have received a copy of the GNU General Public License
 *  along with GNU Make; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
 *
 */
#ifndef AVILIB_H
#define AVILIB_H

#include <stdio.h>

#include <limits.h>
#include <stdlib.h>
#include <string.h>

/*#ifdef _WINDOWS

#include <windows.h>
//add by XuG for windows compatibility
#include <basetsd.h>

#endif*/
//#include <errno.h>
//#include "mmreg.h"
//file operation support
#include "IFileOps.h"



#define PACKAGE "PALMMICRO DEMO" 
#define VERSION "1.0"
//typedef UINT64 uint64_t;

#define AVI_MAX_TRACKS 8


typedef struct
{
  unsigned long key;
  unsigned long pos;
  unsigned long len;
} video_index_entry;

typedef struct
{
   unsigned long pos;
   unsigned long len;
   unsigned long tot;
} audio_index_entry;

typedef struct track_s
{

    long   a_fmt;             /* Audio format, see #defines below */
    long   a_chans;           /* Audio channels, 0 for no audio */
    long   a_rate;            /* Rate in Hz */
    long   a_bits;            /* bits per audio sample */
    long   mp3rate;           /* mp3 bitrate kbs*/

    long   audio_strn;        /* Audio stream number */
    long   audio_bytes;       /* Total number of bytes of audio data */
    long   audio_chunks;      /* Chunks of audio data in the file */

    char   audio_tag[4];      /* Tag of audio data */
    long   audio_posc;        /* Audio position: chunk */
    long   audio_posb;        /* Audio position: byte within chunk */
 
    long  a_codech_off;       /* absolut offset of audio codec information */ 
    long  a_codecf_off;       /* absolut offset of audio codec information */ 

    audio_index_entry *audio_index;

} track_t;

typedef struct
{
  
  IFileOps* pFileSys;           /* File operator of AVI file */
  long   mode;              /* 0 for reading, 1 for writing */
  
  long   width;             /* Width  of a video frame */
  long   height;            /* Height of a video frame */
  double fps;               /* Frames per second */
  char   compressor[8];     /* Type of compressor, 4 bytes + padding for 0 byte */
  char   compressor2[8];     /* Type of compressor, 4 bytes + padding for 0 byte */
  long   video_strn;        /* Video stream number */
  long   video_frames;      /* Number of video frames */
  char   video_tag[4];      /* Tag of video data */
  long   video_pos;         /* Number of next frame to be read
			       (if index present) */
  
  unsigned long max_len;    /* maximum video chunk present */
  
  track_t track[AVI_MAX_TRACKS];  // up to AVI_MAX_TRACKS audio tracks supported
  
  unsigned long pos;        /* position in file */
  long   n_idx;             /* number of index entries actually filled */
  long   max_idx;           /* number of index entries actually allocated */
  
  long  v_codech_off;       /* absolut offset of video codec (strh) info */ 
  long  v_codecf_off;       /* absolut offset of video codec (strf) info */ 
  
  unsigned char (*idx)[16]; /* index entries (AVI idx1 tag) */
  video_index_entry *video_index;
  
  unsigned long last_pos;          /* Position of last frame written */
  unsigned long last_len;          /* Length of last frame written */
  int must_use_index;              /* Flag if frames are duplicated */
  unsigned long   movi_start;
  
  int anum;            // total number of audio tracks 
  int aptr;            // current audio working track 
  
} avi_t;

#define AVI_MODE_WRITE  0
#define AVI_MODE_READ   1

/* The error codes delivered by avi_open_input_file */

#define AVI_ERR_SIZELIM      1     /* The write of the data would exceed
                                      the maximum size of the AVI file.
                                      This is more a warning than an error
                                      since the file may be closed safely */

#define AVI_ERR_OPEN         2     /* Error opening the AVI file - wrong path
                                      name or file nor readable/writable */

#define AVI_ERR_READ         3     /* Error reading from AVI File */

#define AVI_ERR_WRITE        4     /* Error writing to AVI File,
                                      disk full ??? */

#define AVI_ERR_WRITE_INDEX  5     /* Could not write index to AVI file
                                      during close, file may still be
                                      usable */

#define AVI_ERR_CLOSE        6     /* Could not write header to AVI file
                                      or not truncate the file during close,
                                      file is most probably corrupted */

#define AVI_ERR_NOT_PERM     7     /* Operation not permitted:
                                      trying to read from a file open
                                      for writing or vice versa */

#define AVI_ERR_NO_MEM       8     /* malloc failed */

#define AVI_ERR_NO_AVI       9     /* Not an AVI file */

#define AVI_ERR_NO_HDRL     10     /* AVI file has no has no header list,
                                      corrupted ??? */

#define AVI_ERR_NO_MOVI     11     /* AVI file has no has no MOVI list,
                                      corrupted ??? */

#define AVI_ERR_NO_VIDS     12     /* AVI file contains no video data */

#define AVI_ERR_NO_IDX      13     /* The file has been opened with
                                      getIndex==0, but an operation has been
                                      performed that needs an index */

/* Possible Audio formats */

#if 0//ndef WAVE_FORMAT_PCM
#define WAVE_FORMAT_UNKNOWN             (0x0000)
#define WAVE_FORMAT_PCM                 (0x0001)
#define WAVE_FORMAT_ADPCM               (0x0002)
#define WAVE_FORMAT_IBM_CVSD            (0x0005)
#define WAVE_FORMAT_ALAW                (0x0006)
#define WAVE_FORMAT_MULAW               (0x0007)
#define WAVE_FORMAT_OKI_ADPCM           (0x0010)
#define WAVE_FORMAT_DVI_ADPCM           (0x0011)
#define WAVE_FORMAT_DIGISTD             (0x0015)
#define WAVE_FORMAT_DIGIFIX             (0x0016)
#define WAVE_FORMAT_YAMAHA_ADPCM        (0x0020)
#define WAVE_FORMAT_DSP_TRUESPEECH      (0x0022)
#define WAVE_FORMAT_GSM610              (0x0031)
#define IBM_FORMAT_MULAW                (0x0101)
#define IBM_FORMAT_ALAW                 (0x0102)
#define IBM_FORMAT_ADPCM                (0x0103)
#endif

#if defined(__cplusplus)
extern "C" {
#endif

//open file in output mode
avi_t* AVI_open_output_file(IFileOps* pFileSys);
//set video format info
void AVI_set_video(avi_t *AVI, int width, int height, double fps, char *compressor);
//set audio format info
void AVI_set_audio(avi_t *AVI, int channels, long rate, int bits, int format, long mp3rate);

//write a video frame
int  AVI_write_frame(avi_t *AVI, char *data, long bytes, int keyframe);

//write a frame index pointer to previous frame chunk(ie. duplicate it)
int  AVI_dup_frame(avi_t *AVI);

//write a audio chunk to file
int  AVI_write_audio(avi_t *AVI, char *data, long bytes);

//append a audio chunk to file
int  AVI_append_audio(avi_t *AVI, char *data, long bytes);

long AVI_bytes_remain(avi_t *AVI);

//append index chunk if in write mode, release buffers
//close the file
int  AVI_close(avi_t *AVI);

//calculate composited avi file length
long AVI_bytes_written(avi_t *AVI);

//open file in input mode
//parse index chunk into memory if getIndex = 1
avi_t *AVI_open_input_file(IFileOps* pFileSys, int getIndex);

//establish internal structure for the already opened file(read mode)
avi_t *AVI_open_fd(IFileOps* pFileSys, int getIndex);

//intenal used by AVI_open_input_file
//parse index chunk into memory if getIndex = 1
int avi_parse_input_file(avi_t *AVI, int getIndex);

/////////////////////////////////////////////////////////////////////////
//get format information from input AVI file 
//bitrate
long AVI_audio_mp3rate(avi_t *AVI);
//frame number
long AVI_video_frames(avi_t *AVI);
//width
int  AVI_video_width(avi_t *AVI);
//height
int  AVI_video_height(avi_t *AVI);
//fps
double AVI_frame_rate(avi_t *AVI);
//codec
char* AVI_video_compressor(avi_t *AVI);
//channel number
int  AVI_audio_channels(avi_t *AVI);
//sample bits
int  AVI_audio_bits(avi_t *AVI);
//audio codec
int  AVI_audio_format(avi_t *AVI);
//sample rate
long AVI_audio_rate(avi_t *AVI);
//total audio bytes
long AVI_audio_bytes(avi_t *AVI);
//audio chunks
long AVI_audio_chunks(avi_t *AVI);
//maximum video chunk present 
long AVI_max_video_chunk(avi_t *AVI);
//frame size, must be indexed file
long AVI_frame_size(avi_t *AVI, long frame);
//audio frame size, must be indexed file
long AVI_audio_size(avi_t *AVI, long frame);

//seek file to the start of data region 
int  AVI_seek_start(avi_t *AVI);
//seek to specified frame
int  AVI_set_video_position(avi_t *AVI, long frame);
//get file offset of a frame, must be indexed file
long AVI_get_video_position(avi_t *AVI, long frame);
//read next video frame, keyframe = 1 if it's a keyframe
//return: length of the frame
long AVI_read_frame(avi_t *AVI, char *vidbuf, int *keyframe);

//seek to "bytes" offset of audio
int  AVI_set_audio_position(avi_t *AVI, long byte);

//set bitrate information of audio
int  AVI_set_audio_bitrate(avi_t *AVI, long bitrate);

//read up to "bytes" bytes audio data to buffer
//return: actual bytes being read
long AVI_read_audio(avi_t *AVI, char *audbuf, long bytes);

//get offset of audio codec information
long AVI_audio_codech_offset(avi_t *AVI);
long AVI_audio_codecf_offset(avi_t *AVI);
long AVI_video_codech_offset(avi_t *AVI);
long AVI_video_codecf_offset(avi_t *AVI);

/* AVI_read_data: Special routine for reading the next audio or video chunk
                  without having an index of the file. */
int  AVI_read_data(avi_t *AVI, char *vidbuf, long max_vidbuf,
                               char *audbuf, long max_audbuf,
                               long *len);

void AVI_print_error(char *str);
char *AVI_strerror();
char *AVI_syserror();

int AVI_scan(char *name);
int AVI_dump(char *name, int mode);

//get text description of codec
char *AVI_codec2str(short cc);

//return 1 if import file exist
int AVI_file_check(char *import_file);
//printf file format summary info, no use
void AVI_info(avi_t *avifile);
//uint64_t AVI_max_size();
int avi_update_header(avi_t *AVI);
//set current track in use
int AVI_set_audio_track(avi_t *AVI, int track);
//get current track in use
int AVI_get_audio_track(avi_t *AVI);
//get number of audio tracks
int AVI_audio_tracks(avi_t *AVI);

#if defined(__cplusplus)
}
#endif

struct riff_struct 
{
  unsigned char id[4];   /* RIFF */
  unsigned long len;
  unsigned char wave_id[4]; /* WAVE */
};


struct chunk_struct 
{
	unsigned char id[4];
	unsigned long len;
};

struct common_struct 
{
	unsigned short wFormatTag;
	unsigned short wChannels;
	unsigned long dwSamplesPerSec;
	unsigned long dwAvgBytesPerSec;
	unsigned short wBlockAlign;
	unsigned short wBitsPerSample;  /* Only for PCM */
};

struct wave_header 
{
	struct riff_struct   riff;
	struct chunk_struct  format;
	struct common_struct common;
	struct chunk_struct  data;
};



struct AVIStreamHeader {
  long  fccType;
  long  fccHandler;
  long  dwFlags;
  long  dwPriority;
  long  dwInitialFrames;
  long  dwScale;
  long  dwRate;
  long  dwStart;
  long  dwLength;
  long  dwSuggestedBufferSize;
  long  dwQuality;
  long  dwSampleSize;
};

#endif //AVILIB_H
