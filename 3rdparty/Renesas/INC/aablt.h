//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


Module Name:

    aablt.h

Abstract:

    Class declaration for a GPE-derived class containing blt functions for ClearType text rendering.


-------------------------------------------------------------------*/

#ifndef __AABLT_H__
#define __AABLT_H__

#include <gpe.h>


//
//  To cast methods from this class to assign to the pBlt member of another GPE-derived class...
// 
#define AATEXTBLTCAST(basename) \
    (SCODE (GPE::*)(struct GPEBltParms *))AATextBlt::##basename

class AATextBlt : public GPE
{

public:


    /////////////////////////////////////////////
    //                                         //
    //              16bpp Methods              //
    //                                         //
    /////////////////////////////////////////////

    //
    // For rendering ClearType text on 16 bpp surfaces, with pixel format specifed by masks, below.
    //
    SCODE AATextBltDst8  ( GPEBltParms * );
    SCODE AATextBltDst16 ( GPEBltParms * );
    SCODE AATextBltDst24 ( GPEBltParms * );
    SCODE AATextBltDst32 ( GPEBltParms * );
};

//
// Drivers may use this function to initialize pfnGetAAMaskBits.  This
// function will return TRUE if the aablt library is not a stub library,
// and FALSE if the aablt library is a stub.
//
BOOL AATextBltInit(void (*pfn)(ULONG *, ULONG *, ULONG *));

//
// This function may be used to select the AATextBlt to use.
//
SCODE AATextBltSelect( GPEBltParms * pParms );

extern void (*pfnGetAAMaskBits)(ULONG *,ULONG *,ULONG *);
#endif __AABLT_H__
