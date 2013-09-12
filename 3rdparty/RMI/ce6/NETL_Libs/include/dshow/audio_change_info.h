// Copyright (c) 2007 - 2008  RMI Corp.

#ifndef _AUDIOCHANGEINFO_H_
#define _AUDIOCHANGEINFO_H_

typedef struct
{
  int sampling_rate;
  int channels;
  int max_output_buffer_size;
  int bits_per_sample;
  unsigned int TimeStampHi;
  unsigned int TimeStampLo;
} audiodecChangeInfo;

// The following is used to send a format change packet in the audio 
// stream disguised as near silence so that no click will be produced by 
// an audio renderer that does not know how to interpret the audio format 
// change. By using a 4-bit number, the click will be imperceptible in 
// 16-bit PCM formats. We use a 24-bit field for the samplng rate, which 
// accomodates the highest rate of 192000. We also use a 24-bit field for 
// the max_output_buffer_size.

typedef struct /* must be a multiple of 12 because of some 24-bit pcm types */
{
  unsigned int unique_id0;
  unsigned int unique_id1;
  unsigned int unique_id2;
  unsigned int unique_id3;
  unsigned int unique_id4;
  unsigned int unique_id5;
  unsigned int unique_id6;
  unsigned int unique_id7;
  unsigned int channels;
  unsigned int bits_per_sample;
  unsigned int srate0;
  unsigned int srate1;
  unsigned int srate2;
  unsigned int srate3;
  unsigned int srate4;
  unsigned int srate5;
  unsigned int max_output_buffer_size0;
  unsigned int max_output_buffer_size1;
  unsigned int max_output_buffer_size2;
  unsigned int max_output_buffer_size3;
  unsigned int max_output_buffer_size4;
  unsigned int max_output_buffer_size5;
  unsigned int max_output_buffer_size6; // unused
  unsigned int max_output_buffer_size7; // unused
} hidden_audio_change;


#define HAC_UNIQUE_ID0 0x000f
#define HAC_UNIQUE_ID1 0x0007
#define HAC_UNIQUE_ID2 0xfffa
#define HAC_UNIQUE_ID3 0x0003
#define HAC_UNIQUE_ID4 0xfff4
#define HAC_UNIQUE_ID5 0xfff3
#define HAC_UNIQUE_ID6 0xffff
#define HAC_UNIQUE_ID7 0x0008

#define pack_hidden_audio_change(srate, num_channel, bitspersample, max_buf_size, hac) { \
  unsigned int temp; \
  hac.unique_id0 = HAC_UNIQUE_ID0; \
  hac.unique_id1 = HAC_UNIQUE_ID1; \
  hac.unique_id2 = HAC_UNIQUE_ID2; \
  hac.unique_id3 = HAC_UNIQUE_ID3; \
  hac.unique_id4 = HAC_UNIQUE_ID4; \
  hac.unique_id5 = HAC_UNIQUE_ID5; \
  hac.unique_id6 = HAC_UNIQUE_ID6; \
  hac.unique_id7 = HAC_UNIQUE_ID7; \
  hac.bits_per_sample = (bitspersample) >> 1; \
  hac.channels = (num_channel); \
  temp = (unsigned int)(srate); \
  hac.srate0 = temp & 0xf; \
  hac.srate1 = (temp >> 4) & 0xf; \
  hac.srate2 = (temp >> 8) & 0xf; \
  hac.srate3 = (temp >> 12) & 0xf; \
  hac.srate4 = (temp >> 16) & 0xf; \
  hac.srate5 = (temp >> 20) & 0xf; \
  temp = (unsigned int)(max_buf_size); \
  hac.max_output_buffer_size0 = temp & 0xf; \
  hac.max_output_buffer_size1 = (temp >> 4) & 0xf; \
  hac.max_output_buffer_size2 = (temp >> 8) & 0xf; \
  hac.max_output_buffer_size3 = (temp >> 12) & 0xf; \
  hac.max_output_buffer_size4 = (temp >> 16) & 0xf; \
  hac.max_output_buffer_size5 = (temp >> 20) & 0xf; \
}

#define unpack_hidden_audio_change(srate, num_channel, bitspersample, max_buf_size, hac) { \
  num_channel = hac->channels; \
  bitspersample = hac->bits_per_sample << 1; \
  srate  = hac->srate5 << 20; \
  srate |= hac->srate4 << 16; \
  srate |= hac->srate3 << 12; \
  srate |= hac->srate2 << 8; \
  srate |= hac->srate1 << 4; \
  srate |= hac->srate0; \
  max_buf_size  = hac->max_output_buffer_size5 << 20; \
  max_buf_size |= hac->max_output_buffer_size4 << 16; \
  max_buf_size |= hac->max_output_buffer_size3 << 12; \
  max_buf_size |= hac->max_output_buffer_size2 << 8; \
  max_buf_size |= hac->max_output_buffer_size1 << 4; \
  max_buf_size |= hac->max_output_buffer_size0; \
}

#define detect_hidden_audio_change(pb) \
  ((((pb) != NULL) && \
   (((hidden_audio_change *)(pb))->unique_id0 == HAC_UNIQUE_ID0) && \
   (((hidden_audio_change *)(pb))->unique_id1 == HAC_UNIQUE_ID1) && \
   (((hidden_audio_change *)(pb))->unique_id2 == HAC_UNIQUE_ID2) && \
   (((hidden_audio_change *)(pb))->unique_id3 == HAC_UNIQUE_ID3) && \
   (((hidden_audio_change *)(pb))->unique_id4 == HAC_UNIQUE_ID4) && \
   (((hidden_audio_change *)(pb))->unique_id5 == HAC_UNIQUE_ID5) && \
   (((hidden_audio_change *)(pb))->unique_id6 == HAC_UNIQUE_ID6) && \
   (((hidden_audio_change *)(pb))->unique_id7 == HAC_UNIQUE_ID7) \
  ) ? 1 : 0)

#endif /* _AUDIOCHANGEINFO_H_ */

