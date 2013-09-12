/* <LIC_AMD_STD>
 * Copyright (C) 2003-2005 Advanced Micro Devices, Inc.  All Rights Reserved.
 * 
 * Unless otherwise designated in writing, this software and any related 
 * documentation are the confidential proprietary information of AMD. 
 * THESE MATERIALS ARE PROVIDED "AS IS" WITHOUT ANY
 * UNLESS OTHERWISE NOTED IN WRITING, EXPRESS OR IMPLIED WARRANTY OF ANY 
 * KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
 * NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE AND IN NO 
 * EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER. 
 * 
 * AMD does not assume any responsibility for any errors which may appear 
 * in the Materials nor any responsibility to support or update the
 * Materials.  AMD retains the right to modify the Materials at any time, 
 * without notice, and is not obligated to provide such modified 
 * Materials to you. AMD is not obligated to furnish, support, or make
 * any further information available to you.
 * </LIC_AMD_STD>  */
/* <CTL_AMD_STD>
 * </CTL_AMD_STD>  */
/* <DOC_AMD_STD>
 * </DOC_AMD_STD>  */
#ifndef SFILES_H
#define SFILES_H
#ifdef __linux__
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#elif defined(UNDER_CE)
#include <tchar.h>
#include <stdio.h>
#include <string.h>
#include <Winbase.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


// serial number file names which has been rot13
// The goal of rot13 is to obfuscated the file names.
// ROT13 is in no shape or form secure, it's sole purpose is
// to hide information not encrypt it.

// :{|;} is translated into '/' forward slash

// = is translated into '.'

// file obfuscated hints (so they don't cause attention to themselves:
//	1.  File names should not begin with "." ie: hidden file.
//  2.  file extention should not be such that it would be clear that 
//  		the file is not of that type.  ie: *.a or *.o
//  3.  Serial number files are text files with the serial number and sig on
//			the first line.

// TODO  Change me to better names
static TCHAR    *sfilenames[] = {
	L"\\Hard Disk\\amd\\serial.dat",   
	L"\\Hard Disk\\amd\\serial1.dat",			
	L"\\Hard Disk\\amd\\serial2.dat",	
	L"\\Hard Disk\\amd\\serial3.dat",    
	NULL	
};
	

#ifdef __cplusplus
}
#endif


#endif // SFILES_H