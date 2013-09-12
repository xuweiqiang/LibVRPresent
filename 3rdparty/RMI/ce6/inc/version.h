/*++
Copyright (c) 1999  BSQUARE Corporation.  All rights reserved.

Module Name:

    version.h

Module Description:

    This file contains defines and macros to provide simpler methods for
    building and using OS version based preprocessor conditionals.  It uses
    version information provided by Microsoft in standard header files.

    Note that this versioning technique was started with version 2.11.  It is
    not expected that the source code will maintain all OS versions.  The
    sources will be cleaned out periodically so that only the latest release
    and perhaps a version or two back will be supported.  Older code should
    be removed for clarity.

    In other words, the purpose of this file is to allow multiple versions
    of Windows CE to be supported with the same code base.  It is not meant
    to allow source files to become a nest of twisted preprocessor
    conditionals.

Author:

    Richard Chinn   4-August-1999

Revision History:

--*/

#ifndef __VERSION_H__
#define __VERSION_H__

//
// Include Microsoft's version information header.  This should contain
// the version numbers of the current operating system being built.
//

#include <bldver.h>

//
// This macro concatenates a major version number and minor version number
// into a single integer that may be used for comparisons (==, >, <, etc.).
// It is assumed that the major version number and the minor version number
// are both <= 255.  It is also assumed that the minor version number will
// always be two digits long.  In other words v2.12 has a major version
// number of 2 and a minor version number of 12; v2.10 has a major version
// number of 2 and a minor version number of 10; v3.0 has a major version
// number of 3 and a minor version number of 0.  v2.101 would not be a valid
// version number using this macro.
//
// If either of these assumtions are violated, then it is possible that
// certain comparisons will fail.  In general, the == and != comparisons are
// safest.  These should be used exclusively to prevent problems with <, >,
// >=, and <=.
//

#define CONCATENATE_MAJOR_MINOR_VERSION(MAJOR, MINOR) \
    (((MAJOR) << 8) | (MINOR))

//
// Current Windows CE OS version number.
//

#define __WINCE_OS_VERSION__ \
    CONCATENATE_MAJOR_MINOR_VERSION(CE_MAJOR_VER, CE_MINOR_VER)

//
// The following macros should be used exclusively in code.  They are
// effectively booleans.  An example of proper usage is as follows.
// Note that the starting version is 2.11.
//
//     #if __WINCE_OS_VER_2_11__
//       ...
//     #else if __WINCE_OS_VER_2_12__
//       ...
//     #else
//       ...
//     #endif
//

#define __WINCE_OS_VER_2_11__ \
    (__WINCE_OS_VERSION__ == CONCATENATE_MAJOR_MINOR_VERSION(2, 11))

#define __WINCE_OS_VER_2_12__ \
    (__WINCE_OS_VERSION__ == CONCATENATE_MAJOR_MINOR_VERSION(2, 12))

#define __WINCE_OS_VER_3_00__ \
    (__WINCE_OS_VERSION__ == CONCATENATE_MAJOR_MINOR_VERSION(3, 00))

#endif __VERSION_H__
