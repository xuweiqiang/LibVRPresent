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

    ctblt.h

Abstract:

    Class declaration for a GPE-derived class containing blt functions for ClearType text rendering.


-------------------------------------------------------------------*/

#ifndef __CTBLT_H__
#define __CTBLT_H__

#include <gpe.h>

#define DEFAULT_CT_GAMMA 1500
extern TCHAR szGamma[]; 
extern TCHAR szGammaValue[];

//
//  To cast methods from this class to assign to the pBlt member of another GPE-derived class...
// 
#define CTBLTCAST(basename) \
    (SCODE (GPE::*)(struct GPEBltParms *))ClearTypeBlt::##basename

class ClearTypeBlt : public GPE
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
    SCODE ClearTypeBltDst8  ( GPEBltParms * );
    SCODE ClearTypeBltDst16 ( GPEBltParms * );
    SCODE ClearTypeBltDst24 ( GPEBltParms * );
    SCODE ClearTypeBltDst32 ( GPEBltParms * );
};

//
// This function will select the pBlt function to use for clear type
//
SCODE ClearTypeBltSelect( GPEBltParms * pParms );

//
// Driver can call this function to load the ClearType registry keys, set
// the default gamma, and set up the pixel format.  This should be used
// instead of the funtions below, not in addition to them.
//
BOOL ClearTypeBltInit( GPEMode * pMode, ULONG * pBitMasks, LPPALETTEENTRY pPalette, UINT uiEntries );

//
// Driver needs to call this once at startup, to tell us the pixel format.
//
void SetClearTypeBltMasks(ULONG fRedMask, ULONG fGreMask, ULONG fBluMask);

//
// Driver needs to call this once at startup, to tell us the gamma of the display (multiplied by 1000, 
// e.g, 1.8 == 1800).
// If you're not sure what this should be, just use 2000.
//
void SetClearTypeBltGamma(ULONG ulGamma);

void SetClearTypeBltPalette(PALETTEENTRY * pSysPalette, int cSysEntry);

#endif __CTBLT_H__

