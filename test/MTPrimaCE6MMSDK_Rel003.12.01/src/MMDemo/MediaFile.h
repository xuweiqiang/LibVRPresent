/***************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. Platform Software               *
 *                                                                         *
 *    Copyright (c) 2008 by SiRF Technology, Inc.  All rights reserved.    *
 *                                                                         *
 *    This Software is protected by United States copyright laws and       *
 *    international treaties.  You may not reverse engineer, decompile     *
 *    or disassemble this Software.                                        *
 *                                                                         *
 *    WARNING:                                                             *
 *    This Software contains SiRF Technology, Inc.'s confidential and      *
 *    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,    *
 *    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED      *
 *    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this      *
 *    Software without SiRF Technology, Inc.'s  express written            *
 *    permission.   Use of any portion of the contents of this Software    *
 *    is subject to and restricted by your written agreement with          *
 *    SiRF Technology, Inc.                                                *
 *                                                                         *
 ***************************************************************************/




// MediaFile.h: interface for the MediaFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEDIAFILE_H__81E6E303_0B86_4E1C_AA49_40F774B826B5__INCLUDED_)
#define AFX_MEDIAFILE_H__81E6E303_0B86_4E1C_AA49_40F774B826B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <string.h>
#include <assert.h>
#include "3rdParty/inc/CEFileSys.h"
#include "3rdParty/inc/avilib.h"
#include "3rdParty/mp4v2/mp4.h"
#include "inc\mm_codec_common.h"


#define PICTURE_START_CODE 0x0100
#define GROUP_START_CODE 0x01b8
#define SEQUENCE_HEADER_CODE 0x01b3


#define VIDEO_OBJECT_START_CODE             0x00000100

#define VOP_START_CODE                      0x000001B6

#define VISUAL_OBJECT_SEQUENCE_START_CODE   0x000001B0


using namespace std;

#define CODEC_MPEG4			0
#define CODEC_WMV			1
#define CODEC_H264			2
#define CODEC_MPEG2			3
#define CODEC_I420			4
#define CODEC_UNKNOWN		255

//interface
class VideoFile  
{
public:
	virtual bool  Open(TCHAR*  pFileName) = 0;
	virtual int Width() = 0;
	virtual int Height() = 0;
	virtual int Codec() = 0;
	virtual int ReadFrame(char** pFrame, int* bKeyFrame ) = 0;					// <0 indicate failed
	virtual int SeekStart() = 0;													// != 0 indicate failed
	virtual void Close() = 0;
	virtual bool IsRawStream() = 0;
	virtual  ~VideoFile(){};
};


inline int * __cdecl _errno(void)
{
	static int  i;
	return &i;
	
}
class VideoFileMp4:public VideoFile, public file_io
{

public:
	bool Open(TCHAR*  pFileName)
	{
		if (m_fp)
			return false;
		
		m_fp = _wfopen(pFileName,  L"rb");;
		if (!m_fp)
			return false;
		
		//get file size
		fseek(m_fp, 0, SEEK_END);
		fpos_t filesize;
		if (fgetpos(m_fp, &filesize) == 0)
			m_ui64FileSize = filesize;
		fseek(m_fp, 0, SEEK_SET);

		m_pMp4File = MP4Link(this);
		if (!m_pMp4File)
		{
			Close();
			return false;
		}


		m_idTrack = MP4FindTrackId(m_pMp4File, 0, MP4_VIDEO_TRACK_TYPE);
		if (m_idTrack == MP4_INVALID_TRACK_ID)
		{
			Close();
			return false;
		}

		const char * media_data_name;
		media_data_name = MP4GetTrackMediaDataName(m_pMp4File, m_idTrack);
		if (_stricmp(media_data_name, "avc1") ==0)
			m_codec =  CODEC_H264;
		else if ((_stricmp(media_data_name, "mp4v") == 0) ||
			(_stricmp(media_data_name, "encv") == 0)) 
		{
			m_codec =  CODEC_MPEG4;
		}
		else
			m_codec = CODEC_UNKNOWN;

		

		return true;

	}
	int Width()
	{
		return MP4GetTrackVideoWidth(m_pMp4File, m_idTrack);

	}
	int Height()
	{
		return MP4GetTrackVideoHeight(m_pMp4File, m_idTrack);

	}
	int Codec()
	{
		return m_codec;
		
	}
	int ReadFrame(char** pFrame, int* bKeyFrame )
	{
		*pFrame = NULL;
		int siz = 0;
		if (!m_idSample)
		{
			//m_idSample == 0, we return config information
			m_idSample++;
			if (!m_pSeqPictHeaders)
			{
				if (m_codec == CODEC_H264)
				{
					BYTE **pSeqHeader = NULL, **pPictHeader = NULL;
					UINT *nSeqHeaderSize = NULL, *nPictHeaderSize = NULL;
					DWORD cbConfig = 0;


					if(MP4GetTrackH264SeqPictHeaders(m_pMp4File, m_idTrack, &pSeqHeader, &nSeqHeaderSize, &pPictHeader, &nPictHeaderSize ))
					{
						m_dwSeqPictHeaders = 0;
						int i = 0;
						for(i=0;nSeqHeaderSize[i];++i)
							m_dwSeqPictHeaders += nSeqHeaderSize[i] + sizeof(DWORD);
						for(i=0;nPictHeaderSize[i];++i)
							m_dwSeqPictHeaders +=  nPictHeaderSize[i] + sizeof(DWORD);
						m_pSeqPictHeaders = malloc(m_dwSeqPictHeaders);
						if (!m_pSeqPictHeaders)
						{
							Close();
							return -1;
						}


						PBYTE pBuf = (PBYTE) m_pSeqPictHeaders;
						DWORD nStartCode = 0x1000000;
						for(i=0;nSeqHeaderSize[i];++i)
						{
							memcpy(pBuf, &nStartCode, sizeof(DWORD));
							pBuf += sizeof(DWORD);
							memcpy(pBuf, pSeqHeader[i], nSeqHeaderSize[i]);
							pBuf += nSeqHeaderSize[i];
						}
						free(*pSeqHeader);
						free(nSeqHeaderSize);
						for(i=0;nPictHeaderSize[i];++i)
						{
							memcpy(pBuf, &nStartCode, sizeof(DWORD));
							pBuf += sizeof(DWORD);
							memcpy(pBuf, pPictHeader[i], nPictHeaderSize[i]);
							pBuf += nPictHeaderSize[i];
						}
						free(*pPictHeader);
						free(nPictHeaderSize);
					}
					
				}
				else if (m_codec == CODEC_MPEG4)
				{
					MP4GetTrackESConfiguration(m_pMp4File, m_idTrack, (u_int8_t**)&m_pSeqPictHeaders, (u_int32_t*)&m_dwSeqPictHeaders);
					
				}
				

			}
			if (m_dwSeqPictHeaders)
			{
				*pFrame = (char*)m_pSeqPictHeaders;
				*bKeyFrame = false;
				return	 (int)m_dwSeqPictHeaders;
			}
			
			
		}
		
		u_int32_t nSize = MP4GetSampleSize(m_pMp4File, m_idTrack, m_idSample);
		if (!nSize)
		{
			return -1;
		}

		if (nSize > m_dwFrame)
		{
			u_int32_t maxSampleSize = MP4GetTrackMaxSampleSize(m_pMp4File, m_idTrack); 
			maxSampleSize += 4; /* +4 for safty*/

			m_pFrame = (u_int8_t*)realloc(m_pFrame, maxSampleSize);
			if (!m_pFrame)
			{
				m_dwFrame = 0;
				return -1;
			}
			m_dwFrame = maxSampleSize;			
		}



		
		if (MP4ReadSample(m_pMp4File, m_idTrack, m_idSample, &m_pFrame, &m_dwFrame))
		{
			*pFrame = (char*)m_pFrame;
			*bKeyFrame = false;
			m_idSample++;
			if (m_codec == CODEC_H264)
			{
				UINT8* pStart = m_pFrame;
				UINT8* pEnd = m_pFrame+nSize;

				DWORD nStartCode = 0x1000000;

				while(pStart < pEnd - 4 )
				{
					LONG tmp  = (pStart[0]<<24)|(pStart[1]<<16)|(pStart[2]<<8)|(pStart[3]);
					memcpy(pStart, &nStartCode, sizeof(DWORD));
					pStart += tmp + sizeof(DWORD);
				}

			}

			
			return (int)m_dwFrame;
		}
		return -1;
	}
	int SeekStart()
	{
		if (!m_pMp4File)
			return -1;
		m_idSample = 0;
		return 0;
	}

	void Close()
	{
		m_idSample = 0;
		m_idTrack = MP4_INVALID_TRACK_ID;
		m_ui64FileSize = 0;
		if (m_pMp4File)
		{
			MP4Close(m_pMp4File);
			m_pMp4File = NULL;
			m_codec = CODEC_UNKNOWN;
		}
		if (m_fp)
		{
			fclose(m_fp);
			m_fp = NULL;
		}
		if (m_pSeqPictHeaders)
		{
			free(m_pSeqPictHeaders);
			m_pSeqPictHeaders = NULL;
			m_dwSeqPictHeaders = 0;
		}
		if (m_pFrame)
		{
			free(m_pFrame);
			m_pFrame = NULL;
			m_dwFrame = 0;
		}

				
	}
	bool IsRawStream() {return false;}
	VideoFileMp4():m_fp(NULL), m_pMp4File(NULL), m_idTrack(MP4_INVALID_TRACK_ID)
		, m_idSample(0), m_ui64FileSize(0),
		m_pSeqPictHeaders(NULL),m_dwSeqPictHeaders(0),
		m_pFrame(NULL), m_dwFrame(0),
		m_codec(CODEC_UNKNOWN)

	{}
	~VideoFileMp4(){Close();}
	







	//file_io interface
	u_int64_t GetFileSize(void)
	{
		return m_ui64FileSize;
	}
	u_int64_t GetPosition(void)
	{
		fpos_t pos;
		if (fgetpos(m_fp, &pos) ==0)
		{
			return (u_int64_t)pos;
		}
		return 0;

	};
	void SetPosition(u_int64_t pos)
	{
		fpos_t  thePos = (fpos_t)pos;

		fsetpos(m_fp, &thePos);
	}
	u_int32_t ReadBytes(u_int8_t* buf, u_int32_t siz)
	{
		return fread(buf, 1, siz, m_fp);


	}
	void WriteBytes(u_int8_t*, u_int32_t) 
	{
		assert(0);
	};



private:
	FILE* m_fp;
	MP4FileHandle m_pMp4File;
	MP4SampleId m_idSample;			//sampe id == 0 is h264 sps and pps
	MP4TrackId m_idTrack;
	u_int64_t m_ui64FileSize;

	void* m_pSeqPictHeaders;
	DWORD m_dwSeqPictHeaders;
	u_int8_t* m_pFrame;
	u_int32_t m_dwFrame;
	DWORD m_codec;

};


class VideoFileRcv:public VideoFile
{
public:
	bool Open(TCHAR* pFileName)
	{
		if (!pFileName)
			return false;
		if (m_fp)
		{
			RETAILMSG(1, (L"rcv parser: close the file first!\r\n"));
			return false;
		}

		FILE* pIn;
		pIn = _wfopen(pFileName, L"rb");
		if (!pIn)
			return false;

		UINT32 i;
		int codec_version;
		int hdrext;

		int width, height;


		if (4 != fread(&m_dwFrame, 1, 4, pIn))
		{
			Close();
			return false;
		}

		m_dwRcvVer = (m_dwFrame >> 30) & 0x1;

		codec_version = m_dwFrame >> 24;
		m_dwFrame &= 0xffffff;
		hdrext = (codec_version >> 7) & 0x1;
		if (m_dwRcvVer == 0) {
			codec_version &= 0x7f;
		} else {
			codec_version &= 0x3f;
		}

		if (codec_version != 5) {
			RETAILMSG(1, (L"rcv parser: coedec version is not WMV3\r\n"));
			return false;
		}

		if (hdrext != 0) {
			if (fread(&m_dwHdrExt, 1, 4, pIn) != 4)
			{
				Close();
				return false;
			}
			m_pHdrExt = malloc (m_dwHdrExt);
			if (m_pHdrExt == NULL)
			{
				Close();
				return false;
			}
			if (fread(m_pHdrExt, 1, m_dwHdrExt, pIn) != m_dwHdrExt)
			{
				Close();
				return false;
			}
		}

		// Get the height of the frames
		if (fread(&m_iHeight, 1, 4, pIn) != 4)
		{
			Close();
			return false;
		}
		else if (m_iHeight <= 0)
		{
			Close();
			return false;
		}

		// Get the width of the frames
		if (fread(&m_iWidth, 1, 4, pIn) != 4)
		{
			Close();
			return false;
		}
		else if (m_iWidth <= 0)
		{
			Close();
			return false;
		}

		if (m_dwRcvVer == 1) {
			fseek(pIn, 4 * 4, SEEK_CUR);	// rcv_additional_header_size, pre_roll, bitrate, framerate
		}
		m_dwStart = ftell(pIn);
		m_fp = pIn;
		return true;
	}


	int Width() { return m_iWidth;}
	int Height() { return m_iHeight;};
	int Codec() { return CODEC_WMV;};
	
	int ReadFrame(char** pFrame, int* bKeyFrame )
	{
		if (!m_fp)
			return false;


		if (!m_dwCurFrame)
		{
			m_dwCurFrame++;
			if (m_dwHdrExt)
			{
				//first time return the sequence header;
				*pFrame = (char*)m_pHdrExt;
				*bKeyFrame = false;
				m_dwCurFrame++;
				return m_dwHdrExt;

			}
			

		}
		if (m_dwCurFrame > m_dwFrame)
		{
			return -1;//reach end;			
		}


		DWORD size, ts;


		if (fread(&size, 1, 4, m_fp) != 4)
			return -1;

		if (m_dwRcvVer == 1) {

			size &= 0x0fffffff;

			if (fread(&ts, 1, 4, m_fp) != 4)
				return -1;;
		}

		if (!m_pTheFrame)
		{
			m_pTheFrame = malloc(m_iWidth*m_iHeight*3/2);
			if (!m_pTheFrame)
				return -1;

		}
		if (fread(m_pTheFrame, 1, size, m_fp) !=  size)
			return -1;
		*pFrame = (char*)m_pTheFrame;
		m_dwCurFrame++;
		return size;
	}
	
	int SeekStart()
	{
		m_dwCurFrame = 0;
		fseek(m_fp, m_dwStart, SEEK_SET);
		return true;
	}
		virtual void Close()
		{
			if (m_pTheFrame)
			{
				free(m_pTheFrame);
				m_pTheFrame = NULL;
			}
			if (m_pHdrExt)
			{
				free(m_pHdrExt);
				m_pHdrExt = NULL;
			}
			if (m_fp)
			{
				fclose(m_fp);
				m_fp = NULL;
			}
			m_dwStart = 0;
			m_dwFrame = 0;
			m_dwCurFrame = 0;
			m_dwRcvVer = 0;
			m_dwHdrExt = 0;
			m_iHeight = -1;
			m_iWidth = -1;

		}
	virtual bool IsRawStream() {return false;}
	

	VideoFileRcv(): m_pTheFrame(NULL), m_pHdrExt(NULL), m_fp(NULL),
		m_dwStart(0), m_dwFrame(0), m_dwCurFrame(0), m_dwRcvVer(0),
		m_dwHdrExt(0), m_iHeight(-1), m_iWidth(-1){}
	~VideoFileRcv() {Close();}


private:
	FILE* m_fp;
	DWORD m_dwStart;
	DWORD m_dwFrame;
	DWORD m_dwCurFrame;
	DWORD m_dwRcvVer;
	void* m_pHdrExt;
	DWORD m_dwHdrExt;
	void* m_pTheFrame;


	

	int m_iWidth;
	int m_iHeight;


};

#define AVI_READ_BUFSIZE (512*1024)
class VideoFileAvi:public VideoFile
{
public:
	VideoFileAvi()
		:m_play_fileop(0),
		m_hAVI(0),
		m_pTheFrame(0)

	{
	
	}

	bool Open(TCHAR* pFileName)
	{
		
		if (m_play_fileop)
			return false;						//open again?

		m_play_fileop = new CEFileSys;
		if (!m_play_fileop)
			return false;

		if(m_play_fileop->Open(pFileName, false) != FERR_NOERROR)
		{
			Close();
			return false;
		}
		
		m_hAVI = AVI_open_input_file(m_play_fileop, 1);//get avilib handle
		if(m_hAVI == NULL)
		{
			Close();
			return false;
		}
		return true;

	}

	int Width()
	{
		if (!m_hAVI)
			return 0;
		return AVI_video_width(m_hAVI);
	}
	int Height()
	{
		if (!m_hAVI)
			return 0;
		return AVI_video_height(m_hAVI);
	}

	int Codec()
	{
		string h264("h264");
		string H264("H264");
		string s(AVI_video_compressor(m_hAVI));
		if(s==h264 || s==H264)
		{
			return CODEC_H264;
		}
		return CODEC_MPEG4;				//guess;
	}
	int ReadFrame(char** ppFrame, int* bKeyFrame )
	{
		if (!m_pTheFrame)
		{
			m_pTheFrame = (char*)malloc(AVI_READ_BUFSIZE);
			if (!m_pTheFrame)
			{
				ppFrame = NULL;
				*bKeyFrame  = 0;
				return -1;
			}
		}
		*ppFrame = m_pTheFrame;
		*bKeyFrame = 0;
		return AVI_read_frame(m_hAVI, m_pTheFrame, bKeyFrame);	
	}

	int SeekStart()
	{
		if (!m_hAVI)
			return -1;
		return AVI_seek_start(m_hAVI);
	}

	virtual ~VideoFileAvi()
	{
		Close();
			
	}

	void Close()
	{
		if (m_hAVI)
		{
			AVI_close(m_hAVI);
			m_hAVI = NULL;
		}
		delete m_play_fileop;
		m_play_fileop = NULL;
		
		if (m_pTheFrame)
		{
			free(m_pTheFrame);
			m_pTheFrame = NULL;
		}
		
	}

	bool IsRawStream()
	{
		return false;
	}

	


private:
	CEFileSys* m_play_fileop;
	avi_t* m_hAVI;
	char* m_pTheFrame;


};

enum 
{
	TOKEN_NUM,
	TOKEN_X,
	TOKEN_OTHER,
	TOKEN_EOS,

};
#define EOS 0
inline bool IsOther(TCHAR c)
{
	return !_istdigit(c) && !(c == _T('x')) && !(c == _T('X')) && !(c == EOS);

}

inline int GetToken(TCHAR* pStart, int* pPos, int* pNum)
{
	
	TCHAR c ;
	TCHAR* pToken; 
	pToken = pStart + *pPos;
	c = *pToken;
	if (_istdigit(c))
	{
		_stscanf(pToken, _T("%d"), pNum);
		while (_istdigit(*pToken))
			pToken++;
		*pPos = pToken-pStart;
		return TOKEN_NUM;
	}
	if ((c == _T('x')) || (c == _T('X')))
	{
		(*pPos)++;
		return TOKEN_X;
	}
	if (c == EOS)
		return TOKEN_EOS;

	while (IsOther(*pToken))
		pToken++;
	*pPos = pToken - pStart;
	return TOKEN_OTHER;
	
}


inline bool GuessResolution(TCHAR*  pFileName, int* pWidth, int* pHeight)
{
	int width;
	*pWidth = 0;
	*pHeight = 0;
	
	int state = 0;
	int token;
	int pos = 0;
	int num;
	while ((token = GetToken(pFileName, &pos, &num)) != TOKEN_EOS)
	{
		//states machine
		switch (state)
		{
			case 0:
				if (token == TOKEN_NUM)
				{
					width = num;
					state = 1;
				}
				break;
			case 1:
				if (token == TOKEN_X)
					state = 2;
				else
					state = 0;
				break;
			case 2:
				if (token == TOKEN_NUM)
				{
					*pWidth = width;
					*pHeight = num;
				}
				state = 0;
				break;
			default:
				assert(0);

		}
	}
	return (*pWidth)&&(*pHeight);

}


#ifndef ROOF
#define ROOF(v, align)  (((v-1)|((align)-1))+1)
#endif 

class VideoFileYuv:public VideoFile
{
public:
	bool  Open(TCHAR*  pFileName)
	{
		if (m_fp)
			return false;
		int width, height;
		if (GuessResolution(pFileName, &width, &height))
		{
			m_iWidth = width;
			m_iHeight = height;
		}

		FILE* fp;
		m_fp = _wfopen(pFileName,  L"rb");;
		if (!m_fp)
			return false;
		
		m_pPrefeched = (char*)malloc(ROOF(m_iWidth, 16)*ROOF(m_iHeight, 16)*BPP());
		if (!m_pPrefeched)
		{
			fclose(m_fp);
			return false;
		}
		return true;

	}
	virtual int Width()
	{
		return m_iWidth;
	}
	virtual int Height()
	{
		return m_iHeight;
	}
	virtual int Codec()
	{
		return CODEC_I420;
	}
	virtual int ReadFrame(char** pFrame, int* bKeyFrame )
	{
		*pFrame = 0;
		*bKeyFrame = 0;
		int frame_size = ROOF(m_iWidth, 16)*ROOF(m_iHeight, 16)*BPP();
		if (fread( m_pPrefeched, 1, frame_size, m_fp) != frame_size)
		{
			return -1;
		}
		*pFrame = m_pPrefeched;
		*bKeyFrame = 1;
		return frame_size;
	}
	virtual int SeekStart()
	{
		if (fseek(m_fp, 0, SEEK_SET))
			return -1;
		return 0;

	}
	virtual void Close()
	{
		m_iWidth = 0;
		m_iHeight = 0;
		if (m_pPrefeched)
			free(m_pPrefeched);
		m_pPrefeched = NULL;
		if (m_fp)
			fclose(m_fp);
		m_fp = NULL;
	}
	virtual bool IsRawStream()
	{
		return false;
	}
	VideoFileYuv():m_fp(0), m_iHeight(240), m_iWidth(320), m_pPrefeched(0){};
	virtual  ~VideoFileYuv(){Close();};
private:
	double BPP() {return 1.5;}
	char* m_pPrefeched;
	FILE* m_fp;
	int m_iHeight;
	int m_iWidth;

};

#define RAW_READ_BUFSIZE (1024*512)
//raw stream
class VideoFileRaw:public VideoFile
{
public:
	bool  Open(TCHAR*  pFileName)
	{
		if (m_fp)
			return false;
		m_fp = _wfopen(pFileName,  L"rb");
		if (!m_fp)
			return false;
		m_pPrefeched = (char*)malloc(RAW_READ_BUFSIZE);
		if (!m_pPrefeched)
		{
			fclose(m_fp);
			return false;
		}
		Init();
		return true;
	}
	virtual int Width() {return 0;}							//get this from xxxx_dec_header
	virtual int Height() {return 0;}						//get this from xxxx_dec_header
	virtual int Codec() = 0;

	
	virtual int ReadFrame(char** pFrame, int* bKeyFrame )					// <0 indicate failed
	{
		int siz;
		siz = PeekFrame(pFrame, bKeyFrame);
		if (siz > 0)
			Skip(siz);
		return siz;
	}
	virtual int SeekStart()													// != 0 indicate failed
	{
		if (fseek(m_fp, 0, SEEK_SET))
			return -1;
		Init();
		return 0;
	}
	virtual void Close()
	{
		fclose(m_fp);
		m_fp = NULL;
		free(m_pPrefeched);
		m_pPrefeched = 0;
		m_pCur = 0;
		m_dwLeft = 0;
	}
	
	
	VideoFileRaw():m_fp(0), m_pPrefeched(0),m_pCur(0), m_dwLeft(0) {}
	virtual  ~VideoFileRaw(){};


	int PeekFrame(char** pFrame, int* bKeyFrame)
	{
		VIDEO_DEC_CONTEXT ctx;
		if (SeekPic((UINT8*)m_pCur,m_dwLeft, &ctx) == CODEC_STATE_OK)
		{
			*pFrame = (char*)ctx.buf_ptr;
			return ctx.buf_len;
		}
		
		//move to front
		memmove(m_pPrefeched, m_pCur, m_dwLeft);
		size_t dwRead = fread( m_pPrefeched + m_dwLeft , 1, RAW_READ_BUFSIZE - m_dwLeft, m_fp);
		if (dwRead == 0)
			return -1;
		
		m_dwLeft += dwRead;
		m_pCur = m_pPrefeched;
		
		//try again.
		if (SeekPic((UINT8*)m_pCur,m_dwLeft, &ctx) == CODEC_STATE_OK)
		{
			*pFrame = (char*)ctx.buf_ptr;
			return ctx.buf_len;
		}
		return -1;
		
		
		
	}
	void Skip(int cnt/*of bytes*/)
	{
		assert(m_pCur + cnt <= m_pCur + m_dwLeft);
		m_pCur+=cnt;
		m_dwLeft -= cnt;
	}

	bool IsRawStream()
	{
		return true;
	}

protected:
	virtual UINT32 SeekPic(UINT8 *pbuf, UINT32 size, VIDEO_DEC_CONTEXT *context_ptr) = 0;



	void Init()
	{
		m_pCur = m_pPrefeched;
		m_dwLeft = 0;
	}
	

private:
	FILE* m_fp;
	char* m_pPrefeched;
	char* m_pCur;
	DWORD m_dwLeft;

	


};

class VideoFileH264:public VideoFileRaw
{
public:
	virtual int Codec() {return CODEC_H264;}
protected:
	virtual UINT32 SeekPic(UINT8 *pbuf, UINT32 size, VIDEO_DEC_CONTEXT *context_ptr)
	{
		UINT8 tmp;
		UINT32 shift,len;
		UINT8 *pStar,*pEnd;
		INT32 iFind;
		iFind =0;
		shift = 0;
		len = 0;
		pStar = pbuf;
		pEnd = pbuf + size;
		while (pStar < pEnd)
		{
			len++;
			tmp = *pStar ++;
			shift = (shift << 8) | tmp;
			
			if((shift & 0x00ffffff) == 0x00000001)
			{
				iFind++;
				if(iFind >1)
				{
					context_ptr->buf_ptr = pbuf;
					context_ptr->buf_len = len - (shift == 0x00000001 ? 4:3);			
					return CODEC_STATE_OK;
				}	
				
			}
		}
		return CODEC_STATE_BUFFER;
	}
	

};


class VideoFileMpeg4:public VideoFileRaw
{
public:
	virtual int Codec() {return CODEC_MPEG4;}
private:
	virtual UINT32 SeekPic(UINT8 *pbuf, UINT32 size, VIDEO_DEC_CONTEXT *context_ptr)
	{
		UINT8 tmp;
		UINT32 shift,len;
		UINT8 *pStar,*pEnd,*pNal;
		
		UINT32 iFind;
		
		iFind =0;
		
		shift = 0;
		len = 0;
		pStar = pbuf;
		pEnd = pbuf + size;
		while (pStar < pEnd)
		{
			len++;
			tmp = *pStar++;       
			shift = (shift << 8) | tmp;
			if((shift== VISUAL_OBJECT_SEQUENCE_START_CODE)||((shift & 0XFFFFFFE0) == VIDEO_OBJECT_START_CODE))
			{
				if(iFind)
				{
					context_ptr->buf_ptr = pbuf;
					context_ptr->buf_len = len - 4;			
					return CODEC_STATE_OK;
				}
			}
			
			if(shift == VOP_START_CODE)
			{
				iFind++;
				if(iFind >1)
				{
					context_ptr->buf_ptr = pbuf;
					context_ptr->buf_len = len - 4;			
					return CODEC_STATE_OK;
				}	
			}
			
			
			
		}
		
		return CODEC_STATE_BUFFER;
	}
};


class VideoFileMpeg2:public VideoFileRaw
{
public:
	virtual int Codec() {return CODEC_MPEG2;}
private:
	virtual UINT32 SeekPic(UINT8 *pbuf, UINT32 size, VIDEO_DEC_CONTEXT *context_ptr)
	{
		UINT8 tmp;
		UINT8 *pStart, *pEnd;
		UINT32 shift = 0, find = 0, len = 0;
		
		pStart = pbuf;
		pEnd = pbuf + size;
		while (pStart < pEnd)
		{
			len++;
			tmp = *pStart++;
			shift = (shift << 8) | tmp;
			
			if (shift == PICTURE_START_CODE)
			{
				find++;
			}
			else if ( ((shift == GROUP_START_CODE) || (shift == SEQUENCE_HEADER_CODE)) && (find == 1)  )
			{
				find++;
			}
			
			if (find == 2)
			{
				context_ptr->buf_ptr = pbuf;
				context_ptr->buf_len = len - 4;			
				return CODEC_STATE_OK;
			}		
		}
		
		return CODEC_STATE_BUFFER;
	}
	
	
};

#endif // !defined(AFX_MEDIAFILE_H__81E6E303_0B86_4E1C_AA49_40F774B826B5__INCLUDED_)
