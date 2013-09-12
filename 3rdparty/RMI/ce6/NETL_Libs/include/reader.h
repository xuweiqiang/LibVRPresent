
/*
 *    File: include/reader.h
 *  Author: Eric DeVolder
 * Purpose: Interface to reader/reader.c
 *   Notes:
 *
 */

#ifndef _READER_H
#define _READER_H

//////////////////////////////////////////////////////////////////////

#include <osal.h>

#include <ctypes.h>
#include <logging.h>
#include <mediaErrors.h>

#define RLOG(LEVEL,...) __LOG_##LEVEL(logZone_reader, 1, __VA_ARGS__)

//////////////////////////////////////////////////////////////////////

typedef enum
{
    reader_reserved = 0,
    reader_file,
    reader_vob,
    reader_rtsp,

    MAX_READERS
} reader_type;

typedef struct
{
    char *name;
    char *description;
    reader_type which;
    int (*init)(const char *filename);
    int (*read)(unsigned int position, uint8 *buffer, int length, int *eos);
    int (*eos)(unsigned int position);
    void (*deinit)(void);
} reader_t;

void readerInit (void);
void readerStart (const char *url);
void readerStop (void);

void reader_start (void);
int reader_read_atfor (unsigned int position, unsigned int length, uint8 *buffer);
int reader_end_of_stream (unsigned int position);
void reader_stop (void);

extern reader_t file_reader;

//////////////////////////////////////////////////////////////////////

#endif // _READER_H

