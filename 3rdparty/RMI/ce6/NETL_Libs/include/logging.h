
/*
 *    File: include/logging.h
 *  Author: Eric DeVolder
 * Purpose: Interface to common/logging.c
 *   Notes:
 *
 */

#ifndef _LOGGING_H
#define _LOGGING_H

#include <ctypes.h>

void log_set_subzone_names(int zone, const char **subzone_names);
int log_find_zone (const char *string);
int log_find_level (const char *string);

void log_set_level (int level);
void log_parse(const char *string);
void log_init(const char *string);

int osal_loggable (int level, int zone, uint32 subZoneMask);
void osal_log (int level, int zone, uint32 subZoneMask, const char *fmt, ...);

//////////////////////////////////////////////////////////////////////
// Logging levels

#define ALL_LOGGING_LEVELS  \
    LOGGING_LEVEL(TRACE)    \
    LOGGING_LEVEL(DEBUG)    \
    LOGGING_LEVEL(INFO)     \
    LOGGING_LEVEL(NOTIFY)   \

//////////////////////////////////////////////////////////////////////
// Logging zones

#define MAX_LOG_ZONES       32
extern uint8 logZoneLevel[MAX_LOG_ZONES];

// List zones here...
#define ALL_LOGGING_ZONES   \
    LOGGING_ZONE(misc)      \
    LOGGING_ZONE(buffer)    \
    LOGGING_ZONE(reader)    \
    LOGGING_ZONE(demux)     \
    LOGGING_ZONE(vcodec)    \
    LOGGING_ZONE(acodec)    \
    LOGGING_ZONE(vdec)      \
    LOGGING_ZONE(adec)      \
    LOGGING_ZONE(vrender)   \
    LOGGING_ZONE(arender)   \
    LOGGING_ZONE(vsample)   \
    LOGGING_ZONE(voutput)   \
    LOGGING_ZONE(aoutput)   \
    LOGGING_ZONE(mae)       \
    LOGGING_ZONE(adhd)      \

typedef enum
{
#define LOGGING_ZONE(Z) logZone_##Z,
    ALL_LOGGING_ZONES
#undef LOGGING_ZONE

    logZone_MAX,
} logZones;

typedef enum
{
#define LOGGING_LEVEL(Z) LOG_LEVEL_##Z,
    ALL_LOGGING_LEVELS
#undef LOGGING_LEVEL

} logLevels;

//////////////////////////////////////////////////////////////////////
// Logging subzones

#define MAX_LOG_SUBZONES    32
extern uint32 logSubZoneMask[MAX_LOG_ZONES];

// NOTE: Subzone 0, or (1 << 0), is reserved as an "always enabled" mask for the zone

//////////////////////////////////////////////////////////////////////
// Logging macros

// By intention, LOG_INFO(), LOG_DEBUG() and LOG_TRACE() are
// "compiled out" in a retail/production/release build. LOG_NOTIFY()
// stays as a way to convey important messages.

// LOG_1() is a hack due to #define DEBUG=1

// http://en.wikipedia.org/wiki/Variadic_macro
// GNU cpp MSG... is more elegant, but not portable.
// __VA_ARGS__ is the most portable way to do this.

#define __LOG_NOTIFY(ZONE,SUBMASK,...)   osal_log(LOG_LEVEL_NOTIFY,ZONE,1,__VA_ARGS__)
#define LOGGABLE_NOTIFY(ZONE,SUBMASK)    osal_loggable(LOG_LEVEL_NOTIFY,ZONE,1)

#define __LOG_INFO(ZONE,SUBMASK,...)     osal_log(LOG_LEVEL_INFO, ZONE,SUBMASK,__VA_ARGS__)
#define LOGGABLE_INFO(ZONE,SUBMASK)      osal_loggable(LOG_LEVEL_INFO,ZONE,1)

#ifdef DEBUG
#define __LOG_DEBUG(ZONE,SUBMASK,...)    osal_log(LOG_LEVEL_DEBUG,ZONE,SUBMASK,__VA_ARGS__)
#define __LOG_TRACE(ZONE,SUBMASK,...)    osal_log(LOG_LEVEL_TRACE,ZONE,SUBMASK,__VA_ARGS__)
#define __LOG_1(ZONE,SUBMASK,...)        osal_log(LOG_LEVEL_DEBUG,ZONE,SUBMASK,__VA_ARGS__)

#define LOGGABLE_DEBUG(ZONE,SUBMASK)     osal_loggable(LOG_LEVEL_DEBUG,ZONE,SUBMASK)
#define LOGGABLE_TRACE(ZONE,SUBMASK)     osal_loggable(LOG_LEVEL_TRACE,ZONE,SUBMASK)
#define LOGGABLE_1(ZONE,SUBMASK)         osal_loggable(LOG_LEVEL_DEBUG,ZONE,SUBMASK)

#else
#define __LOG_DEBUG(ZONE,SUBMASK,...)
#define __LOG_TRACE(ZONE,SUBMASK,...)
#define __LOG_1(ZONE,SUBMASK,...)

#define LOGGABLE_DEBUG(ZONE,SUBMASK) 0
#define LOGGABLE_TRACE(ZONE,SUBMASK) 0
#define LOGGABLE_1(ZONE,SUBMASK) 0
#endif

#define LOG(LEVEL,...) __LOG_##LEVEL(logZone_misc, 1, __VA_ARGS__)

//////////////////////////////////////////////////////////////////////

#endif // _LOGGING_H

