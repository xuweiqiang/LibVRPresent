
/*
 *    File: include/mediaerrors.h
 *  Author: Eric DeVolder
 * Purpose: List of common errors
 *   Notes: Keep this file free of external dependencies
 *
 */

#ifndef _MEDIAERRORS_H
#define _MEDIAERRORS_H

//////////////////////////////////////////////////////////////////////

#define ALL_ERRORS \
    ERROR_NAME(NotImpl),        \
    ERROR_NAME(BadNumber),      \
    ERROR_NAME(BadString),      \
    ERROR_NAME(BadZone),        \
    ERROR_NAME(BadParam),       \
    ERROR_NAME(NullParam),      \
    ERROR_NAME(BadContext),     \
    ERROR_NAME(BadFont),        \
    ERROR_NAME(BadGuid),        \
    ERROR_NAME(AllocFailed),    \
    ERROR_NAME(PicStop),        \
    ERROR_NAME(Watchdog),       \
    ERROR_NAME(TimedOut),       \
    ERROR_NAME(UnsupportedMedia),\
    ERROR_NAME(BadReader),      \
    ERROR_NAME(BadDemux),       \
    ERROR_NAME(BadCodec),       \
    ERROR_NAME(BadRenderer),    \
    ERROR_NAME(BadTrack),       \
    ERROR_NAME(BadFourCC),      \
    ERROR_NAME(BadSample),      \
    ERROR_NAME(BadReserved),    \
    ERROR_NAME(NullSample),     \
    ERROR_NAME(FileFailed),     \
    ERROR_NAME(OutOfBounds),    \
    ERROR_NAME(BadColorSpace),  \
    ERROR_NAME(BadGeometry),    \
    ERROR_NAME(WidthTooLarge),  \
    ERROR_NAME(HeightTooLarge), \
    ERROR_NAME(OutOfPictures),  \
    ERROR_NAME(NullRefPic),     \
    ERROR_NAME(MAEFailed),      \
    ERROR_NAME(BadBitsRequest), \
    ERROR_NAME(BadPicture),     \
    ERROR_NAME(BadStartCode),   \
    ERROR_NAME(BadMacroblock),  \
    ERROR_NAME(BadMarkerBit),   \
    ERROR_NAME(BadVLC),         \
    ERROR_NAME(Bad8x8),         \


typedef enum
{
    erNone = 0,

#define ERROR_NAME(E) err##E
    ALL_ERRORS
#undef ERROR_NAME

    MAX_MEDIA_ERRORS

} commonErrorTypes;

//////////////////////////////////////////////////////////////////////

extern const char *mediaErrMsg[MAX_MEDIA_ERRORS];

//////////////////////////////////////////////////////////////////////

#endif // _MEDIAERRORS_H

