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
/*++ 
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:

    MSIME.H

Abstract:

    Japanese specific definitions of IFECommon, IFELanguage, IFEDictionary,
    and Per IME Interfaces.

Revision History:

    Version 1.0

--*/

#ifndef __MSIME_H__
#define __MSIME_H__

#include <ole2.h>
#include <objbase.h>
#include <imm.h>

#pragma pack(1)         /* Assume byte packing throughout */

///////////////////////////
// HKEY_CLASSES_ROOT values
///////////////////////////

#define imeVersionMin       6
#define imeVersionCur       10
#define imeVersionMax       16
#define Str(x)              TEXT(#x)
#define NumStr(x)           Str(x)

#define szImeJapan98        TEXT("MSIME.Japan.6")
#define szImeJapan8         TEXT("MSIME.Japan.8")     // For MS-IME 8.0
#define szImeJapan9         TEXT("MSIME.Japan.9")     // For MS-IME 8.1
#define szImeJapan10        TEXT("MSIME.Japan.10")    // For MS-IME 9
#define szImeJapan          TEXT("MSIME.Japan")
#define szImeJapanCur       szImeJapan TEXT(".") NumStr(imeVersionCur)

////////////////
// CLSID and IID
////////////////

// Class ID for the current FE COM interfaces (IME 9.5)
// {A64BDBED-14B2-4F56-9C10-3DEBBC13FE7B}
DEFINE_GUID(CLSID_MSIME_JAPANESE,
0x1e2b5062, 0xddee, 0x4dc4, 0xb4, 0xfa, 0xc8, 0xc5, 0xb0, 0xd5, 0xce, 0x97);

// Class ID for the current FE COM interfaces (IME 9)
// {A64BDBED-14B2-4F56-9C10-3DEBBC13FE7B}
DEFINE_GUID(CLSID_MSIME_JAPANESE_10,
0xa64bdbed, 0x14b2, 0x4f56, 0x9c, 0x10, 0x3d, 0xeb, 0xbc, 0x13, 0xfe, 0x7b);

// Class ID for MSIME.9's FE COM interfaces (Satori 8.1)
// {AF9EE286-8CD3-445E-8842-433E5C460775}
DEFINE_GUID(CLSID_MSIME_JAPANESE_9,
0xaf9ee286, 0x8cd3, 0x445e, 0x88, 0x42, 0x43, 0x3e, 0x5c, 0x46, 0x07, 0x75);

// Class ID for MSIME.8's FE COM interfaces (Satori 8.0)
// {462C11AD-BBBE-43E3-9976-FA8E6B0DFF01}
DEFINE_GUID(CLSID_MSIME_JAPANESE_8,
0x462c11ad, 0xbbbe, 0x43e3, 0x99, 0x76, 0xfa, 0x8e, 0x6b, 0x0d, 0xff, 0x01);

// Class ID for MSIME.6's FE COM interfaces 
// {019F7150-E6DB-11d0-83C3-00C04FDDB82E}
DEFINE_GUID(CLSID_MSIME_JAPANESE_6, 
0x19f7150, 0xe6db, 0x11d0, 0x83, 0xc3, 0x0, 0xc0, 0x4f, 0xdd, 0xb8, 0x2e);

// Interface ID for IFECommon
// {019F7151-E6DB-11d0-83C3-00C04FDDB82E}
DEFINE_GUID(IID_IFECommon,
0x19f7151, 0xe6db, 0x11d0, 0x83, 0xc3, 0x0, 0xc0, 0x4f, 0xdd, 0xb8, 0x2e);

// Interface ID for IFELanguage
// {019F7152-E6DB-11d0-83C3-00C04FDDB82E}
DEFINE_GUID(IID_IFELanguage,
0x19f7152, 0xe6db, 0x11d0, 0x83, 0xc3, 0x0, 0xc0, 0x4f, 0xdd, 0xb8, 0x2e);

// Interface ID for IFELanguage2
// {21164102-C24A-11d1-851A-00C04FCC6B14}
DEFINE_GUID(IID_IFELanguage2,
0x21164102, 0xc24a, 0x11d1, 0x85, 0x1a, 0x0, 0xc0, 0x4f, 0xcc, 0x6b, 0x14);

// Interface ID for IFEDictionary
// {019F7153-E6DB-11d0-83C3-00C04FDDB82E}
DEFINE_GUID(IID_IFEDictionary,
0x19f7153, 0xe6db, 0x11d0, 0x83, 0xc3, 0x0, 0xc0, 0x4f, 0xdd, 0xb8, 0x2e);

// Interface ID for IImeIPoint1
// {84E913C1-BA57-11d1-AFEE-00805F0C8B6D}
DEFINE_GUID(IID_IImeIPoint1, 
0x84e913c1, 0xba57, 0x11d1, 0xaf, 0xee, 0x0, 0x80, 0x5f, 0xc, 0x8b, 0x6d);

// Interface ID for IImeIPoint2
// {9A8F2C69-0BC0-4c38-A27D-D8F5C2303553}
DEFINE_GUID(IID_IImeIPoint2, 
0x9a8f2c69, 0xbc0, 0x4c38, 0xa2, 0x7d, 0xd8, 0xf5, 0xc2, 0x30, 0x35, 0x53);

// Interface ID for IID_IImeIPointNotifySink
// {BE2DF62A-8993-4204-BE53-138E3A8D0FE3}
DEFINE_GUID(IID_IImeIPointNotifySink,
0xbe2df62a, 0x8993, 0x4204, 0xbe, 0x53, 0x13, 0x8e, 0x3a, 0x8d, 0xf, 0xe3);

////////////////////////////
// Definitions for IFECommon
////////////////////////////

#undef  INTERFACE
#define INTERFACE   IFEClassFactory

////////////////////////////////
// The IFEClassFactory Interface
////////////////////////////////

DECLARE_INTERFACE_(IFEClassFactory, IClassFactory)
{
    // IUnknown members
    STDMETHOD(QueryInterface)   (THIS_ REFIID refiid, VOID **ppv) PURE;
    STDMETHOD_(ULONG,AddRef)    (THIS) PURE;
    STDMETHOD_(ULONG,Release)   (THIS) PURE;

    // IFEClassFactory members
    STDMETHOD(CreateInstance)   (THIS_ LPUNKNOWN, REFIID, void **) PURE;
    STDMETHOD(LockServer)       (THIS_ BOOL) PURE;
};


#undef  INTERFACE
#define INTERFACE   IFECommon

//////////////////////////
// The IFECommon Interface
//////////////////////////

#define IFEC_S_ALREADY_DEFAULT          MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, 0x7400)

typedef struct _IMEDLG
{
    int         cbIMEDLG;               //size of this structure
    HWND        hwnd;                   //parent window handle
    LPWSTR      lpwstrWord;             //optional string
    int         nTabId;                 //specifies a tab in dialog
} IMEDLG;

DECLARE_INTERFACE_(IFECommon, IUnknown)
{
    // IUnknown members
    STDMETHOD(QueryInterface)   (THIS_ REFIID refiid, VOID **ppv) PURE;
    STDMETHOD_(ULONG,AddRef)    (THIS) PURE;
    STDMETHOD_(ULONG,Release)   (THIS) PURE;

    // IFECommon members
    STDMETHOD(IsDefaultIME) (THIS_
                            CHAR *szName,               //(out) name of MS-IME
                            INT cszName                 //(in) size of szName
                            ) PURE;
    STDMETHOD(SetDefaultIME)    (THIS) PURE;
    STDMETHOD(InvokeWordRegDialog) (THIS_
                            IMEDLG *pimedlg             //(in) parameters
                            ) PURE;
    STDMETHOD(InvokeDictToolDialog) (THIS_
                            IMEDLG *pimedlg             //(in) parameters
                            ) PURE;
};


///////////////////////////
// The IFELanguage interface
///////////////////////////

// Word Descriptor
typedef struct tagWDD{
    WORD        wDispPos;   // Offset of Output string
    union {
        WORD    wReadPos;   // Offset of Reading string
        WORD    wCompPos;
    };

    WORD        cchDisp;    //number of ptchDisp
    union {
        WORD    cchRead;    //number of ptchRead
        WORD    cchComp;
    };

    DWORD       nReserve;   //reserved

    WORD        nPos;       //part of speech

                            // implementation-defined
    WORD        fPhrase : 1;//start of phrase
    WORD        fAutoCorrect: 1;//auto-corrected
    WORD        fNumericPrefix: 1;//kansu-shi expansion(JPN)
    WORD        fUserRegistered: 1;//from user dictionary
    WORD        fUnknown: 1;//unknown word (duplicated information with nPos.)
    WORD        fRecentUsed: 1; //used recently flag
    WORD        :10;        //

    VOID        *pReserved; //points directly to WORDITEM
} WDD;

#pragma warning(disable:4200) // zero-size array in structure
typedef struct tagMORRSLT {
    DWORD       dwSize;             // total size of this block.
    WCHAR       *pwchOutput;        // conversion result string.
    WORD        cchOutput;          // lengh of result string.
    union {
        WCHAR   *pwchRead;          // reading string
        WCHAR   *pwchComp;
    };
    union {
        WORD    cchRead;            // length of reading string.
        WORD    cchComp;
    };
    WORD        *pchInputPos;       // index array of reading to input character.
    WORD        *pchOutputIdxWDD;   // index array of output character to WDD
    union {
        WORD    *pchReadIdxWDD;     // index array of reading character to WDD
        WORD    *pchCompIdxWDD;
    };
    WORD        *paMonoRubyPos;     // array of position of monoruby
    WDD         *pWDD;              // pointer to array of WDD
    INT         cWDD;               // number of WDD
    VOID        *pPrivate;          // pointer of private data area
    WCHAR       BLKBuff[];          // area for stored above members.
                                    //  WCHAR   wchOutput[cchOutput];
                                    //  WCHAR   wchRead[cchRead];
                                    //  CHAR    chInputIdx[cwchInput];
                                    //  CHAR    chOutputIdx[cchOutput];
                                    //  CHAR    chReadIndx[cchRead];
                                    //  ????    Private
                                    //  WDD     WDDBlk[cWDD];
}MORRSLT;
#pragma warning(default:4200) // zero-size array in structure

// request for conversion (dwRequest)
#define FELANG_REQ_CONV         0x00010000
#define FELANG_REQ_RECONV       0x00020000
#define FELANG_REQ_REV          0x00030000


// Conversion mode (dwCMode)
#define FELANG_CMODE_MONORUBY       0x00000002  //mono-ruby
#define FELANG_CMODE_NOPRUNING      0x00000004  //no pruning
#define FELANG_CMODE_KATAKANAOUT    0x00000008  //katakana output
#define FELANG_CMODE_HIRAGANAOUT    0x00000000  //default output
#define FELANG_CMODE_HALFWIDTHOUT   0x00000010  //half-width output
#define FELANG_CMODE_FULLWIDTHOUT   0x00000020  //full-width output
#define FELANG_CMODE_BOPOMOFO       0x00000040  //
#define FELANG_CMODE_HANGUL         0x00000080  //
#define FELANG_CMODE_PINYIN         0x00000100  //
#define FELANG_CMODE_PRECONV        0x00000200  //do conversion as follows:
                                                // - roma-ji to kana
                                                // - autocorrect before conversion
                                                // - periods, comma, and brackets
#define FELANG_CMODE_RADICAL        0x00000400  //
#define FELANG_CMODE_UNKNOWNREADING 0x00000800  //
#define FELANG_CMODE_MERGECAND      0x00001000  // merge display with same candidate
#define FELANG_CMODE_ROMAN          0x00002000  //
#define FELANG_CMODE_BESTFIRST      0x00004000  // only make 1st best
#define FELANG_CMODE_USENOREVWORDS  0x00008000  // use invalid revword on REV/RECONV.

#define FELANG_CMODE_NONE           0x01000000  // IME_SMODE_NONE
#define FELANG_CMODE_PLAURALCLAUSE  0x02000000  // IME_SMODE_PLAURALCLAUSE
#define FELANG_CMODE_SINGLECONVERT  0x04000000  // IME_SMODE_SINGLECONVERT
#define FELANG_CMODE_AUTOMATIC      0x08000000  // IME_SMODE_AUTOMATIC
#define FELANG_CMODE_PHRASEPREDICT  0x10000000  // IME_SMODE_PHRASEPREDICT
#define FELANG_CMODE_CONVERSATION   0x20000000  // IME_SMODE_CONVERSATION
#define FELANG_CMODE_NAME           FELANG_CMODE_PHRASEPREDICT  // Name mode
#define FELANG_CMODE_NOINVISIBLECHAR 0x40000000 // remove invisible chars (e.g. tone mark)


// Error message
#define E_NOCAND            0x30    //not enough candidates
#define E_NOTENOUGH_BUFFER  0x31    //out of string buffer
#define E_NOTENOUGH_WDD     0x32    //out of WDD buffer
#define E_LARGEINPUT        0x33    //large input string


//Morphology Info
#define FELANG_CLMN_WBREAK      0x01
#define FELANG_CLMN_NOWBREAK    0x02
#define FELANG_CLMN_PBREAK      0x04
#define FELANG_CLMN_NOPBREAK    0x08
#define FELANG_CLMN_FIXR        0x10
#define FELANG_CLMN_FIXD        0x20    // fix display of word

#define FELANG_INVALD_PO        0xFFFF  // unmatched position for input string

#undef INTERFACE
#define INTERFACE       IFELanguage

//IFELanguage template
DECLARE_INTERFACE_(IFELanguage,IUnknown)
{
    // IUnknown members
    STDMETHOD(QueryInterface)(THIS_ REFIID refiid, VOID **ppv) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    // Ijconv members.  must be virtual functions
    STDMETHOD(Open)(THIS) PURE;
    STDMETHOD(Close)(THIS) PURE;

    STDMETHOD(GetJMorphResult)(THIS_
                        DWORD   dwRequest,          // [in]
                        DWORD   dwCMode,            // [in]
                        INT     cwchInput,          // [in]
                        WCHAR   *pwchInput,         // [in, size_is(cwchInput)]
                        DWORD   *pfCInfo,           // [in, size_is(cwchInput)]
                        MORRSLT **ppResult ) PURE;  // [out]

    STDMETHOD(GetConversionModeCaps)(THIS_ DWORD *pdwCaps) PURE;

    STDMETHOD(GetPhonetic)(THIS_
                        BSTR    string,             // [in]
                        LONG    start,              // [in]
                        LONG    length,             // [in]
                        BSTR *  phonetic ) PURE;    // [out, retval]

    STDMETHOD(GetConversion)(THIS_
                        BSTR    string,             // [in]
                        LONG    start,              // [in]
                        LONG    length,             // [in]
                        BSTR *  result ) PURE;      // [out, retval]
};


////////////////////////////////
// Definitions for IFEDictionary
////////////////////////////////

// Part Of Speach
#define IFED_POS_NONE                   0x00000000
#define IFED_POS_NOUN                   0x00000001
#define IFED_POS_VERB                   0x00000002
#define IFED_POS_ADJECTIVE              0x00000004
#define IFED_POS_ADJECTIVE_VERB         0x00000008
#define IFED_POS_ADVERB                 0x00000010
#define IFED_POS_ADNOUN                 0x00000020
#define IFED_POS_CONJUNCTION            0x00000040
#define IFED_POS_INTERJECTION           0x00000080
#define IFED_POS_INDEPENDENT            0x000000ff
#define IFED_POS_INFLECTIONALSUFFIX     0x00000100
#define IFED_POS_PREFIX                 0x00000200
#define IFED_POS_SUFFIX                 0x00000400
#define IFED_POS_AFFIX                  0x00000600
#define IFED_POS_TANKANJI               0x00000800
#define IFED_POS_IDIOMS                 0x00001000
#define IFED_POS_SYMBOLS                0x00002000
#define IFED_POS_PARTICLE               0x00004000
#define IFED_POS_AUXILIARY_VERB         0x00008000
#define IFED_POS_SUB_VERB               0x00010000
#define IFED_POS_DEPENDENT              0x0001c000
#define IFED_POS_ALL                    0x0001ffff

// GetWord Selection Type
#define IFED_SELECT_NONE                0x00000000
#define IFED_SELECT_READING             0x00000001
#define IFED_SELECT_DISPLAY             0x00000002
#define IFED_SELECT_POS                 0x00000004
#define IFED_SELECT_COMMENT             0x00000008
#define IFED_SELECT_ALL                 0x0000000f

// Registered Word Type
#define IFED_REG_NONE                   0x00000000
#define IFED_REG_USER                   0x00000001
#define IFED_REG_AUTO                   0x00000002
#define IFED_REG_GRAMMAR                0x00000004
#define IFED_REG_ALL                    0x00000007

// Dictionary Type
#define IFED_TYPE_NONE                  0x00000000
#define IFED_TYPE_GENERAL               0x00000001
#define IFED_TYPE_NAMEPLACE             0x00000002
#define IFED_TYPE_SPEECH                0x00000004
#define IFED_TYPE_REVERSE               0x00000008
#define IFED_TYPE_ENGLISH               0x00000010
#define IFED_TYPE_ALL                   0x0000001f

//Dictionary Category
#define dicatNone                       0x00000000
#define dicatGeneral                    0x00000001
#define dicatNamePlace                  0x00000002
#define dicatSpeech                     0x00000004
#define dicatReverse                    0x00000008
#define dicatEnglish                    0x00000010
#define dicatALL                        0x0000001f

// HRESULTS for IFEDictionary interface

//no more entries in the dictionary
#define IFED_S_MORE_ENTRIES             MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, 0x7200)
//dictionary is empty, no header information is returned
#define IFED_S_EMPTY_DICTIONARY         MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, 0x7201)
//word already exists in dictionary
#define IFED_S_WORD_EXISTS              MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, 0x7202)
//word already exists in dictionary but only comment data is updated
#define IFED_S_COMMENT_CHANGED          MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, 0x7203)

//dictionary is not found
#define IFED_E_NOT_FOUND                MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x7300)
//invalid dictionary format
#define IFED_E_INVALID_FORMAT           MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x7301)
//failed to open file
#define IFED_E_OPEN_FAILED              MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x7302)
//failed to write to file
#define IFED_E_WRITE_FAILED             MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x7303)
//no entry found in dictionary
#define IFED_E_NO_ENTRY                 MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x7304)
//this routines doesn't support the current dictionary
#define IFED_E_REGISTER_FAILED          MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x7305)
//not a user dictionary
#define IFED_E_NOT_USER_DIC             MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x7306)
//not supported
#define IFED_E_NOT_SUPPORTED            MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x7307)
//failed to insert user comment
#define IFED_E_USER_COMMENT             MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x7308)
//improper POS is to be registered
#define IFED_E_REGISTER_ILLEGAL_POS     MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x7309)
//improper word is to be registered
#define IFED_E_REGISTER_IMPROPER_WORD   MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x730a)

#define cbCommentMax            256

//Private Unicode Character
#define wchPrivate1             0xE000

// Where to place registring word
typedef enum
{
    IFED_REG_HEAD,
    IFED_REG_TAIL,
    IFED_REG_DEL,
} IMEREG;


// Relation type
typedef enum
{
    IFED_REL_NONE,
    IFED_REL_NO,
    IFED_REL_GA,
    IFED_REL_WO,
    IFED_REL_NI,
    IFED_REL_DE,
    IFED_REL_YORI,
    IFED_REL_KARA,
    IFED_REL_MADE,
    IFED_REL_HE,
    IFED_REL_TO,
    IFED_REL_IDEOM,
    IFED_REL_FUKU_YOUGEN,       //p2_1
    IFED_REL_KEIYOU_YOUGEN,     //p2_2
    IFED_REL_KEIDOU1_YOUGEN,    //p2_3
    IFED_REL_KEIDOU2_YOUGEN,    //p2_4
    IFED_REL_TAIGEN,            //p2_5
    IFED_REL_YOUGEN,            //p2_6
    IFED_REL_RENTAI_MEI,        //p3_1
    IFED_REL_RENSOU,            //p3_2
    IFED_REL_KEIYOU_TO_YOUGEN,  //p3_3
    IFED_REL_KEIYOU_TARU_YOUGEN,//p3_4
    IFED_REL_UNKNOWN1,          //p3_5
    IFED_REL_UNKNOWN2,          //p3_6
    IFED_REL_ALL,               //any type
} IMEREL;

// Type of IME dictionary
typedef enum
{
    IFED_UNKNOWN,
    IFED_MSIME2_BIN_SYSTEM,
    IFED_MSIME2_BIN_USER,
    IFED_MSIME2_TEXT_USER,
    IFED_MSIME95_BIN_SYSTEM,
    IFED_MSIME95_BIN_USER,
    IFED_MSIME95_TEXT_USER,
    IFED_MSIME97_BIN_SYSTEM,
    IFED_MSIME97_BIN_USER,
    IFED_MSIME97_TEXT_USER,
    IFED_MSIME98_BIN_SYSTEM,
    IFED_MSIME98_BIN_USER,
    IFED_MSIME98_TEXT_USER,
    IFED_ACTIVE_DICT,
    IFED_ATOK9,
    IFED_ATOK10,
    IFED_NEC_AI_,
    IFED_WX_II,
    IFED_WX_III,
    IFED_VJE_20,
    IFED_MSIME98_SYSTEM_CE,
    IFED_MSIME_BIN_SYSTEM,
    IFED_MSIME_BIN_USER,
    IFED_MSIME_TEXT_USER,
    IFED_PIME2_BIN_USER,
    IFED_PIME2_BIN_SYSTEM,
    IFED_PIME2_BIN_STANDARD_SYSTEM,
} IMEFMT;

// Type of User Comment
typedef enum
{
    IFED_UCT_NONE,
    IFED_UCT_STRING_SJIS,
    IFED_UCT_STRING_UNICODE,
    IFED_UCT_USER_DEFINED,
    IFED_UCT_MAX,
} IMEUCT;


#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable:4201)
#endif

// WoRD found in a dictionary
typedef struct _IMEWRD
{
    WCHAR       *pwchReading;
    WCHAR       *pwchDisplay;
    union {
        ULONG ulPos;
        struct {
            WORD        nPos1;      //hinshi
            WORD        nPos2;      //extended hinshi
        } ;
    };
    ULONG       rgulAttrs[2];       //attributes
    INT         cbComment;          //size of user comment
    IMEUCT      uct;                //type of user comment
    VOID        *pvComment;         //user comment
} IMEWRD, *PIMEWRD;

#if _MSC_VER >= 1200
#pragma warning(pop)
#endif

// DePendency Info
typedef struct _IMEDP
{
    IMEWRD      wrdModifier;    //kakari-go
    IMEWRD      wrdModifiee;    //uke-go
    IMEREL      relID;
} IMEDP, *PIMEDP;

// Shared Header dictionary File
typedef struct _IMESHF
{
    WORD        cbShf;              //size of this struct
    WORD        verDic;             //dictionary version
    CHAR        szTitle[48];        //dictionary title
    CHAR        szDescription[256]; //dictionary description
    CHAR        szCopyright[128];   //dictionary copyright info
} IMESHF;


#define POS_UNDEFINED      0
#define JPOS_UNDEFINED      POS_UNDEFINED
#define JPOS_MEISHI_FUTSU       100     //ñºéå
#define JPOS_MEISHI_SAHEN       101     //Ç≥ïœñºéå
#define JPOS_MEISHI_ZAHEN       102     //Ç¥ïœñºéå
#define JPOS_MEISHI_KEIYOUDOUSHI        103     //å`ìÆñºéå
#define JPOS_HUKUSIMEISHI       104     //ïõéåìIñºéå
#define JPOS_MEISA_KEIDOU       105     //Ç≥ïœå`ìÆ
#define JPOS_JINMEI     106     //êlñº
#define JPOS_JINMEI_SEI     107     //ê©
#define JPOS_JINMEI_MEI     108     //ñº
#define JPOS_CHIMEI     109     //ínñº
#define JPOS_CHIMEI_KUNI        110     //çë
#define JPOS_CHIMEI_KEN     111     //åß
#define JPOS_CHIMEI_GUN     112     //åS
#define JPOS_CHIMEI_KU      113     //ãÊ
#define JPOS_CHIMEI_SHI     114     //és
#define JPOS_CHIMEI_MACHI       115     //í¨
#define JPOS_CHIMEI_MURA        116     //ë∫
#define JPOS_CHIMEI_EKI     117     //âw
#define JPOS_SONOTA     118     //å≈óLñºéå
#define JPOS_SHAMEI     119     //é–ñº
#define JPOS_SOSHIKI        120     //ëgêD
#define JPOS_KENCHIKU       121     //åöízï®
#define JPOS_BUPPIN     122     //ï®ïi
#define JPOS_DAIMEISHI      123     //ë„ñºéå
#define JPOS_DAIMEISHI_NINSHOU      124     //êlèÃë„ñºéå
#define JPOS_DAIMEISHI_SHIJI        125     //éwé¶ë„ñºéå
#define JPOS_KAZU       126     //êî
#define JPOS_KAZU_SURYOU        127     //êîó 
#define JPOS_KAZU_SUSHI     128     //êîéå
#define JPOS_5DAN_AWA       200     //Ç†ÇÌçs
#define JPOS_5DAN_KA        201     //Ç©çs
#define JPOS_5DAN_GA        202     //Ç™çs
#define JPOS_5DAN_SA        203     //Ç≥çs
#define JPOS_5DAN_TA        204     //ÇΩçs
#define JPOS_5DAN_NA        205     //Ç»çs
#define JPOS_5DAN_BA        206     //ÇŒçs
#define JPOS_5DAN_MA        207     //Ç‹çs
#define JPOS_5DAN_RA        208     //ÇÁçs
#define JPOS_5DAN_AWAUON        209     //Ç†ÇÌçsÇ§âπï÷
#define JPOS_5DAN_KASOKUON      210     //Ç©çsë£âπï÷
#define JPOS_5DAN_RAHEN     211     //ÇÁçsïœäi
#define JPOS_4DAN_HA        212     //ÇÕçsélíi
#define JPOS_1DAN       213     //àÍíiìÆéå
#define JPOS_TOKUSHU_KAHEN      214     //Ç©ïœìÆéå
#define JPOS_TOKUSHU_SAHENSURU      215     //Ç≥ïœìÆéå
#define JPOS_TOKUSHU_SAHEN      216     //Ç≥çsïœäi
#define JPOS_TOKUSHU_ZAHEN      217     //Ç¥çsïœäi
#define JPOS_TOKUSHU_NAHEN      218     //Ç»çsïœäi
#define JPOS_KURU_KI        219     //óà
#define JPOS_KURU_KITA      220     //óàÇΩ
#define JPOS_KURU_KITARA        221     //óàÇΩÇÁ
#define JPOS_KURU_KITARI        222     //óàÇΩÇË
#define JPOS_KURU_KITAROU       223     //óàÇΩÇÎÇ§
#define JPOS_KURU_KITE      224     //óàÇƒ
#define JPOS_KURU_KUREBA        225     //óàÇÍÇŒ
#define JPOS_KURU_KO        226     //óàÅiÇ»Ç¢Åj
#define JPOS_KURU_KOI       227     //óàÇ¢
#define JPOS_KURU_KOYOU     228     //óàÇÊÇ§
#define JPOS_SURU_SA        229     //Ç≥
#define JPOS_SURU_SI        230     //Çµ
#define JPOS_SURU_SITA      231     //ÇµÇΩ
#define JPOS_SURU_SITARA        232     //ÇµÇΩÇÁ
#define JPOS_SURU_SIATRI        233     //ÇµÇΩÇË
#define JPOS_SURU_SITAROU       234     //ÇµÇΩÇÎÇ§
#define JPOS_SURU_SITE      235     //ÇµÇƒ
#define JPOS_SURU_SIYOU     236     //ÇµÇÊÇ§
#define JPOS_SURU_SUREBA        237     //Ç∑ÇÍÇŒ
#define JPOS_SURU_SE        238     //Çπ
#define JPOS_SURU_SEYO      239     //ÇπÇÊÅ^ÇµÇÎ
#define JPOS_KEIYOU     300     //å`óeéå
#define JPOS_KEIYOU_GARU        301     //å`óeéå∂ﬁŸ
#define JPOS_KEIYOU_GE      302     //å`óeéåπﬁ
#define JPOS_KEIYOU_ME      303     //å`óeéå“
#define JPOS_KEIYOU_YUU     304     //å`óeéå≠≥
#define JPOS_KEIYOU_U       305     //å`óeéå≥
#define JPOS_KEIDOU     400     //å`óeìÆéå
#define JPOS_KEIDOU_NO      401     //å`óeìÆéå…
#define JPOS_KEIDOU_TARU        402     //å`óeìÆéå¿Ÿ
#define JPOS_KEIDOU_GARU        403     //å`óeìÆéå∂ﬁŸ
#define JPOS_FUKUSHI        500     //ïõéå
#define JPOS_FUKUSHI_SAHEN      501     //Ç≥ïœïõéå
#define JPOS_FUKUSHI_NI     502     //ïõéå∆
#define JPOS_FUKUSHI_NANO       503     //ïõéå≈
#define JPOS_FUKUSHI_DA     504     //ïõéå¿ﬁ
#define JPOS_FUKUSHI_TO     505     //ïõéåƒ
#define JPOS_FUKUSHI_TOSURU     506     //ïõéåƒÇ≥ïœ
#define JPOS_RENTAISHI      600     //òAëÃéå
#define JPOS_RENTAISHI_SHIJI        601     //éwé¶òAëÃéå
#define JPOS_SETSUZOKUSHI       650     //ê⁄ë±éå
#define JPOS_KANDOUSHI      670     //ä¥ìÆéå
#define JPOS_SETTOU     700     //ê⁄ì™åÍ
#define JPOS_SETTOU_KAKU        701     //çÇåãÇPê⁄ì™åÍ
#define JPOS_SETTOU_SAI     702     //çÇåãÇQê⁄ì™åÍ
#define JPOS_SETTOU_FUKU        703     //çÇåãÇRê⁄ì™åÍ
#define JPOS_SETTOU_MI      704     //çÇåãÇSê⁄ì™åÍ
#define JPOS_SETTOU_DAISHOU     705     //çÇåãÇTê⁄ì™åÍ
#define JPOS_SETTOU_KOUTEI      706     //çÇåãÇUê⁄ì™åÍ
#define JPOS_SETTOU_CHOUTAN     707     //çÇåãÇVê⁄ì™åÍ
#define JPOS_SETTOU_SHINKYU     708     //çÇåãÇWê⁄ì™åÍ
#define JPOS_SETTOU_JINMEI      709     //êlñºê⁄ì™åÍ
#define JPOS_SETTOU_CHIMEI      710     //ínñºê⁄ì™åÍ
#define JPOS_SETTOU_SONOTA      711     //å≈óLê⁄ì™åÍ
#define JPOS_SETTOU_JOSUSHI     712     //ëOíuèïêîéå
#define JPOS_SETTOU_TEINEI_O        713     //íöîJÇPê⁄ì™åÍ
#define JPOS_SETTOU_TEINEI_GO       714     //íöîJÇQê⁄ì™åÍ
#define JPOS_SETTOU_TEINEI_ON       715     //íöîJÇRê⁄ì™åÍ
#define JPOS_SETSUBI        800     //ê⁄îˆåÍ
#define JPOS_SETSUBI_TEKI       801     //çÇåãÇPê⁄îˆåÍ
#define JPOS_SETSUBI_SEI        802     //çÇåãÇQê⁄îˆåÍ
#define JPOS_SETSUBI_KA     803     //çÇåãÇRê⁄îˆåÍ
#define JPOS_SETSUBI_CHU        804     //çÇåãÇSê⁄îˆåÍ
#define JPOS_SETSUBI_FU     805     //çÇåãÇTê⁄îˆåÍ
#define JPOS_SETSUBI_RYU        806     //çÇåãÇUê⁄îˆåÍ
#define JPOS_SETSUBI_YOU        807     //çÇåãÇVê⁄îˆåÍ
#define JPOS_SETSUBI_KATA       808     //çÇåãÇWê⁄îˆåÍ
#define JPOS_SETSUBI_MEISHIRENDAKU      809     //ñºéåòAë˜
#define JPOS_SETSUBI_JINMEI     810     //êlñºê⁄îˆåÍ
#define JPOS_SETSUBI_CHIMEI     811     //ínñºê⁄îˆåÍ
#define JPOS_SETSUBI_KUNI       812     //çëê⁄îˆåÍ
#define JPOS_SETSUBI_KEN        813     //åßê⁄îˆåÍ
#define JPOS_SETSUBI_GUN        814     //åSê⁄îˆåÍ
#define JPOS_SETSUBI_KU     815     //ãÊê⁄îˆåÍ
#define JPOS_SETSUBI_SHI        816     //ésê⁄îˆåÍ
#define JPOS_SETSUBI_MACHI      817     //í¨ÇPê⁄îˆåÍ
#define JPOS_SETSUBI_CHOU       818     //í¨ÇQê⁄îˆåÍ
#define JPOS_SETSUBI_MURA       819     //ë∫ÇPê⁄îˆåÍ
#define JPOS_SETSUBI_SON        820     //ë∫ÇQê⁄îˆåÍ
#define JPOS_SETSUBI_EKI        821     //âwê⁄îˆåÍ
#define JPOS_SETSUBI_SONOTA     822     //å≈óLê⁄îˆåÍ
#define JPOS_SETSUBI_SHAMEI     823     //é–ñºê⁄îˆåÍ
#define JPOS_SETSUBI_SOSHIKI        824     //ëgêDê⁄îˆåÍ
#define JPOS_SETSUBI_KENCHIKU       825     //åöízï®ê⁄îˆåÍ
#define JPOS_RENYOU_SETSUBI     826     //òAópê⁄îˆåÍ
#define JPOS_SETSUBI_JOSUSHI        827     //å„íuèïêîéå
#define JPOS_SETSUBI_JOSUSHIPLUS        828     //å„íuèïêîéåÅ{
#define JPOS_SETSUBI_JIKAN      829     //éûä‘èïêîéå
#define JPOS_SETSUBI_JIKANPLUS      830     //éûä‘èïêîéåÅ{
#define JPOS_SETSUBI_TEINEI     831     //íöîJê⁄îˆåÍ
#define JPOS_SETSUBI_SAN        832     //íöîJÇPê⁄îˆåÍ
#define JPOS_SETSUBI_KUN        833     //íöîJÇQê⁄îˆåÍ
#define JPOS_SETSUBI_SAMA       834     //íöîJÇRê⁄îˆåÍ
#define JPOS_SETSUBI_DONO       835     //íöîJÇSê⁄îˆåÍ
#define JPOS_SETSUBI_FUKUSU     836     //ï°êîê⁄îˆåÍ
#define JPOS_SETSUBI_TACHI      837     //ï°êîÇPê⁄îˆåÍ
#define JPOS_SETSUBI_RA     838     //ï°êîÇQê⁄îˆåÍ
#define JPOS_TANKANJI       900     //íPäøéö
#define JPOS_TANKANJI_KAO       901     //äÁ
#define JPOS_KANYOUKU       902     //äµópãÂ
#define JPOS_DOKURITSUGO        903     //ì∆óßåÍ
#define JPOS_FUTEIGO        904     //ïsíËåÍ
#define JPOS_KIGOU      905     //ãLçÜ
#define JPOS_EIJI       906     //âpéö
#define JPOS_KUTEN      907     //ãÂì_
#define JPOS_TOUTEN     908     //ì«ì_
#define JPOS_KANJI      909     //âêÕïsî\ï∂éö
#define JPOS_OPENBRACE      910     //äJÇ´äáå 
#define JPOS_CLOSEBRACE     911     //ï¬Ç∂äáå 


//POS table data structure
typedef struct _POSTBL
{
    WORD        nPos;                   //pos number
    BYTE        *szName;                //name of pos
} POSTBL;


//////////////////////////////
// The IFEDictionary interface
//////////////////////////////

#undef  INTERFACE
#define INTERFACE   IFEDictionary

DECLARE_INTERFACE_(IFEDictionary, IUnknown)
{
    // IUnknown members
    STDMETHOD(QueryInterface)(THIS_ REFIID refiid, VOID **ppv) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    // IFEDictionary members
    STDMETHOD(Open)         (THIS_
                            CHAR *pchDictPath,          //(in) dictionary path
                            IMESHF *pshf                //(out) dictionary header
                            ) PURE;
    STDMETHOD(Close)        (THIS) PURE;
    STDMETHOD(GetHeader)    (THIS_
                            CHAR *pchDictPath,          //(in) dictionary path
                            IMESHF *pshf,               //(out) dictionary header
                            IMEFMT *pjfmt,              //(out) dictionary format
                            ULONG *pulType              //(out) dictionary type
                            ) PURE;
    STDMETHOD(DisplayProperty) (THIS_
                            HWND hwnd                   //(in) parent window handle
                            ) PURE;
    STDMETHOD(GetPosTable)  (THIS_
                            POSTBL **prgPosTbl,         //(out) pos table pointer
                            int *pcPosTbl               //(out) pos table count pointer
                            ) PURE;
    STDMETHOD(GetWords)     (THIS_
                            WCHAR *pwchFirst,           //(in) starting range of reading
                            WCHAR *pwchLast,            //(in) ending range of reading
                            WCHAR *pwchDisplay,         //(in) display
                            ULONG ulPos,                //(in) part of speech (IFED_POS_...)
                            ULONG ulSelect,             //(in) output selection
                            ULONG ulWordSrc,            //(in) user or auto registered word?
                            UCHAR *pchBuffer,           //(in/out) buffer for storing array of IMEWRD
                            ULONG cbBuffer,             //(in) size of buffer in bytes
                            ULONG *pcWrd                //(out) count of IMEWRD's returned
                            ) PURE;
    STDMETHOD(NextWords)    (THIS_
                            UCHAR *pchBuffer,           //(in/out) buffer for storing array of IMEWRD
                            ULONG cbBuffer,             //(in) size of buffer in bytes
                            ULONG *pcWrd                //(out) count of IMEWRD's returned
                            ) PURE;
    STDMETHOD(Create)       (THIS_
                            CHAR *pchDictPath,          //(in) path for the new dictionary
                            IMESHF *pshf                //(in) dictionary header
                            ) PURE;
    STDMETHOD(SetHeader)    (THIS_
                            IMESHF *pshf                //(in) dictionary header
                            ) PURE;
    STDMETHOD(ExistWord)    (THIS_
                            IMEWRD *pwrd                //(in) word to check
                            ) PURE;
    STDMETHOD(ExistDependency) (THIS_
                            IMEDP *pdp                  //(in) see if dependency pair exist in dictionary
                            ) PURE;
    STDMETHOD(RegisterWord) (THIS_
                            IMEREG reg,                 //(in) type of operation to perform on IMEWRD
                            IMEWRD *pwrd                //(in) word to be registered or deleted
                            ) PURE;
    STDMETHOD(RegisterDependency) (THIS_
                            IMEREG reg,                 //(in) type of operation to perform on IMEDP
                            IMEDP *pdp                  //(in) dependency to be registered or deleted
                            ) PURE;
    STDMETHOD(GetDependencies) (THIS_
                            WCHAR *pwchKakariReading,   //(in) reading for kakarigo
                            WCHAR *pwchKakariDisplay,   //(in) display for kakarigo
                            ULONG ulKakariPos,          //(in) part of speech for kakarigo
                            WCHAR *pwchUkeReading,      //(in) reading for ukego
                            WCHAR *pwchUkeDisplay,      //(in) display for ukego
                            ULONG ulUkePos,             //(in) part of speech for ukego
                            IMEREL  jrel,               //(in) relation type
                            ULONG ulWordSrc,            //(in) registration type
                            UCHAR *pchBuffer,           //(in/out) buffer for storing array of IMEDP
                            ULONG cbBuffer,             //(in) size of buffer in bytes
                            ULONG *pcdp                 //(out) count of IMEDP's returned
                            ) PURE;
    STDMETHOD(NextDependencies) (THIS_
                            UCHAR *pchBuffer,           //(in/out) buffer for storing array of IMEDP
                            ULONG cbBuffer,             //(in) size of buffer in bytes
                            ULONG *pcDp                 //(out) count of IMEDP's retrieved
                            ) PURE;
};


//////////////////////////////////////////
// Definitions for IImeIPoint1
//////////////////////////////////////////

//----------------------------------------------------------------
// Fareast Language id
//----------------------------------------------------------------
#define IPFEID_MASK                 0x00F00000

#define IPFEID_NONE                 0x00000000
#define IPFEID_CHINESE_TRADITIONAL  0x00100000
#define IPFEID_CHINESE_SIMPLIFIED   0x00200000
#define IPFEID_CHINESE_HONGKONG     0x00300000
#define IPFEID_CHINESE_SINGAPORE    0x00400000
#define IPFEID_JAPANESE             0x00500000
#define IPFEID_KOREAN               0x00600000
#define IPFEID_KOREAN_JOHAB         0x00700000

//
// dwCharId
//
#define IPCHARID_CHARNO_MASK        0x0000FFFF

//----------------------------------------------------------------
//Control Id (dwIMEFuncID)
//----------------------------------------------------------------
                                            // lparam (N/A use IPCTRLPARAM_DEFAULT)
#define IPCTRL_CONVERTALL           1       // N/A
#define IPCTRL_DETERMINALL          2       // N/A
#define IPCTRL_DETERMINCHAR         3       // n:number of DETCHARS (IPCTRLPARAM_DEFAULT is same as 1)
#define IPCTRL_CLEARALL             4       // N/A
#define IPCTRL_CARETSET             5       // IPCTRLPARAM_MAKE_CARET_POS(uipos, chpos)
#define IPCTRL_CARETLEFT            6       // n:number of LEFT (IPCTRLPARAM_DEFAULT is same as 1)
#define IPCTRL_CARETRIGHT           7       // n:number of RIGHT(IPCTRLPARAM_DEFAULT is same as 1)
#define IPCTRL_CARETTOP             8       // N/A
#define IPCTRL_CARETBOTTOM          9       // N/A
#define IPCTRL_CARETBACKSPACE       10      // n:number of BS chars. (IPCTRLPARAM_DEFAULT is same as 1)
#define IPCTRL_CARETDELETE          11      // n:number of DEL chars. (IPCTRLPARAM_DEFAULT is same as 1)
#define IPCTRL_PHRASEDELETE         12      // N/A 
#define IPCTRL_INSERTSPACE          13      // N/A
#define IPCTRL_INSERTFULLSPACE      14      // N/A
#define IPCTRL_INSERTHALFSPACE      15      // N/A
#define IPCTRL_ONIME                16      // N/A
#define IPCTRL_OFFIME               17      // N/A
#define IPCTRL_PRECONVERSION        18      // IPCTRLPARAM_ON/IPCTRLPARAM_OFF
#define IPCTRL_PHONETICCANDIDATE    19      // N/A
#define IPCTRL_GETCONVERSIONMODE    20      // lparam should be LPARAM address.
#define IPCTRL_GETSENTENCENMODE     21      // lparam should be LPARAM address.
//----------------------------------------------------------------
//Control Id lparam
//----------------------------------------------------------------
#define IPCTRLPARAM_DEFAULT ((LPARAM)(0xfeeeeeee))
#define IPCTRLPARAM_ON      ((LPARAM)(0x00000001))
#define IPCTRLPARAM_OFF     ((LPARAM)(0x00000000))

// for IPCTRL_CARETSET
#define CARET_ICHPOS   (0x0000FFFF)     // IP Position on the composition string. 0-n
#define CARET_UIPOS    (0x0FFF0000)     // UIPOS (position on the character)
                                        // 0-3: X--XO--O ( 2chars )
                                        //      23012301
#define IPCTRLPARAM_MAKE_CARET_POS(uipos, chpos) ((LPARAM)(((uipos << 16) & CARET_UIPOS) | (chpos & CARET_ICHPOS)))

//
// for InsertImeItem (iPos)
//
#define IPINS_CURRENT         (0xfeeeeeee)

typedef struct tagIPCANDIDATE {
    DWORD   dwSize;                 // size of this structure
    DWORD   dwFlags;                // IPFLAG_XXXX
    INT     iSelIndex;              // select index.
    INT     nCandidate;             // number of candidate
    DWORD   dwPrivateDataOffset;    // Private data offset  
    DWORD   dwPrivateDataSize;      // Private data size
    DWORD   dwOffset[1];            //Offset of String from IPCANDIDATE struct's TOP address.
} IPCANDIDATE, * PIPCANDIDATE;


//////////////////////////////////////////
// The IImeIPoint1 interface
//////////////////////////////////////////

#undef  INTERFACE
#define INTERFACE   IImeIPoint1

DECLARE_INTERFACE_(IImeIPoint1, IUnknown)
{
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG, Release) (THIS) PURE;

    STDMETHOD(InsertImeItem)(THIS_
                             IPCANDIDATE* pImeItem,
                             INT          iPos,     // = IPINS_CURRENT:use current IP position and 
                                                    //                 set IP to the end of insert chars.
                                                    // = 0-n: The offset of all composition string to set 
                                                    //        IP position, before insert chars. 
                                                    //        and IP back to original position.
                             DWORD        *lpdwCharId) PURE;

    STDMETHOD(ReplaceImeItem)(THIS_
                             INT          iPos,     // = IPINS_CURRENT:use current IP position and 
                                                    //                 set IP to the end of insert chars.
                                                    // = 0-n: The offset of all composition string to set 
                                                    //        IP position, before insert chars. 
                                                    //        and IP back to original position.
                             INT          iTargetLen, 
                             IPCANDIDATE* pImeItem,
                             DWORD        *lpdwCharId) PURE;

    STDMETHOD(InsertStringEx)(THIS_ 
                             WCHAR* pwSzInsert,
                             INT    cchSzInsert,
                             DWORD  *lpdwCharId) PURE;

    STDMETHOD(DeleteCompString)(THIS_ 
                             INT          iPos,     // = IPINS_CURRENT:use current IP position .
                                                    // = 0-n: The offset of all composition string to set 
                                                    //        IP position, before deleting chars. 
                                                    //        and IP back to original position.
                             INT          cchSzDel) PURE;

    STDMETHOD(ReplaceCompString)(THIS_ 
                             INT          iPos,     // = IPINS_CURRENT:use current IP position and 
                                                    //                 set IP to the end of insert chars.
                                                    // = 0-n: The offset of all composition string to set 
                                                    //        IP position, before insert chars. 
                                                    //        and IP back to original position.
                             INT          iTargetLen, 
                             WCHAR       *pwSzInsert, 
                             INT          cchSzInsert,
                             DWORD       *lpdwCharId) PURE;

    STDMETHOD(ControlIME)(THIS_ DWORD dwIMEFuncID, LPARAM lpara) PURE;

    STDMETHOD(GetAllCompositionInfo)(THIS_      // You can set NULL to any parameter if you don't need.
        WCHAR   **ppwSzCompStr,         // All composition string with determined area. (
                                        //     This area allocated by IPOINT using CoTaskMemAlloc, so
                                        //     If function is succeed caller must free this area using CoTaskMemFree.
        DWORD   **ppdwCharID,           // Array of charID coresponding to composition string.
                                        //     This area allocated by IPOINT using CoTaskMemAlloc, so
                                        //     If function is succeed caller must free this area using CoTaskMemFree.
                                        //
                                        // if the character of compositionstring is changed from typingstring then
                                        // charID is null.
                                        //
        INT     *pcchCompStr,           // All composition string length.
        INT     *piIPPos,               // Current IP position.
        INT     *piStartUndetStrPos,    // Undetermined string start position.
        INT     *pcchUndetStr,          // Undetermined string length.
        INT     *piEditStart,           // Editable area start position.
        INT     *piEditLen              // Editable area length.
    ) PURE;


    STDMETHOD(GetIpCandidate)(THIS_ DWORD dwCharId, IPCANDIDATE **ppImeItem, INT *piColumn, INT *piCount) PURE;
                                        // ppImeItem is allocated by IPOINT using CoTaskMemAlloc
    STDMETHOD(SelectIpCandidate)(THIS_ DWORD dwCharId, INT iselno) PURE;

    STDMETHOD(UpdateContext)(THIS_ BOOL fGenerateMessage) PURE;

};


//////////////////////////////////////////
// Definitions for IImeIPoint2
//////////////////////////////////////////

//----------------------------------------------------------------
//Extra Control IDs (dwIMEFuncID)
//     1 - 30 defined in ipoint1.h
//----------------------------------------------------------------
                                            // lparam (N/A use IPCTRLPARAM_DEFAULT)
#define IPCTRL_SETCONVERSIONMODE    31      // CONVERSION MODE same as IMM
#define IPCTRL_SETSENTENCENMODE     32      // SENTENCE MODE
#define IPCTRL_MAKECANDIDATE        33      // N/A
#define IPCTRL_SELECTCANDIDATE      34      // n: 0 - n : selected cadidate number.

                                            // lparam (N/A use IPCTRLPARAM_DEFAULT)
#define IPCTRL_SETSHIFTSTATE        41      // Shift state
#define IPCTRL_GETERROR             42      // Error Status
#define IPCTRL_RELOADCFG            43      // Reload configration setting

#define CTRLID_RELOAD           (0x00000000)
#define CTRLID_INVOKE_PROP      (0x00000001)
#define CTRLID_RELOAD_FOR_PROP  (0x00000002)

//------------------------------------------------------------------
//
// japan specific
//
//
// Special conversion.
//
#define IPCTRL_JPN_CONVHIRA         100     // N/A       HIRAGANA 
#define IPCTRL_JPN_CONVKATA         101     // N/A       FULL-W KATAKANA
#define IPCTRL_JPN_CONVALNUM        102     // N/A       FULL-W ALPHA
#define IPCTRL_JPN_CONVALFW         103     // N/A       FULL-W to HALF-W
#define IPCTRL_JPN_CONVALNUMHLF     104     // N/A       HALF-W ALPHA
//
//
#define IPCTRL_JPN_BUNLENDEC        110     // N/A
#define IPCTRL_JPN_BUNLENINC        111     // N/A
#define IPCTRL_JPN_CRSRLEFT         112     // N/A
#define IPCTRL_JPN_CRSRRIGHT        113     // N/A
#define IPCTRL_JPN_UNDOBUNSETU      114     // N/A
#define IPCTRL_JPN_UNDOCLR          115     // N/A
#define IPCTRL_JPN_DETBUN           116     // N/A


//////////////////////////////////////////
// The IImeIPoint2 interface
//////////////////////////////////////////

#undef  INTERFACE
#define INTERFACE   IImeIPoint2

DECLARE_INTERFACE_(IImeIPoint2, IImeIPoint1)
{
    STDMETHOD(Initialize)(THIS_ VOID* pCallBack ) PURE;
};


//////////////////////////////////////////
// Definitions for IImeIPointNotifySink
//////////////////////////////////////////

/* Forward Declarations */ 

#ifndef __IImeIPointNotifySink_FWD_DEFINED__
#define __IImeIPointNotifySink_FWD_DEFINED__
typedef interface IImeIPointNotifySink IImeIPointNotifySink;
#endif  /* __IImeIPointNotifySink_FWD_DEFINED__ */


//////////////////////////////////////////
// The IImeIPointNotifySink interface
//////////////////////////////////////////

#ifndef __IImeIPointNotifySink_INTERFACE_DEFINED__
#define __IImeIPointNotifySink_INTERFACE_DEFINED__

EXTERN_C const IID IID_IImeIPointNotifySink;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BE2DF62A-8993-4204-BE53-138E3A8D0FE3")
    IImeIPointNotifySink : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetCursorPos( 
            /* [in] */ UINT uiCursorPos) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetResultStr( 
            /* [in] */ UINT cwch,
            /* [in] */ WCHAR __RPC_FAR *pwsz) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsReadingHalfKata( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetResultReadStr( 
            /* [in] */ UINT cwch,
            /* [in] */ WCHAR __RPC_FAR *pwsz) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetCompositionReadStr( 
            /* [in] */ UINT cwch,
            /* [in] */ WCHAR __RPC_FAR *pwsz) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetCompositionStr( 
            /* [in] */ UINT cwch,
            /* [in] */ WCHAR __RPC_FAR *pwsz) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetResultClause( 
            /* [in] */ UINT ciCls,
            /* [in] */ DWORD __RPC_FAR *pdw) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetResultReadClause( 
            /* [in] */ UINT ciCls,
            /* [in] */ DWORD __RPC_FAR *pdw) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetCompositionReadClause( 
            /* [in] */ UINT ciCls,
            /* [in] */ DWORD __RPC_FAR *pdw) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetCompositionClause( 
            /* [in] */ UINT ciCls,
            /* [in] */ DWORD __RPC_FAR *pdw) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetCompositionReadAttr( 
            /* [in] */ UINT ciAtr,
            /* [in] */ BYTE __RPC_FAR *pb) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetCompositionAttr( 
            /* [in] */ UINT ciAtr,
            /* [in] */ BYTE __RPC_FAR *pb) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetTargetClause( 
            /* [in] */ UINT uiTarget) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetCandidateStr( 
            /* [in] */ UINT ciCandidate,
            /* [in] */ WCHAR __RPC_FAR *pwsz,
            /* [in] */ UINT cbData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetCandidateSelection( 
            /* [in] */ UINT iciCandidate) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetDeltaPos( 
            /* [in] */ UINT iDelat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GenerateMessage( void) = 0;
        
    };
    
#else   /* C style interface */

    typedef struct IImeIPointNotifySinkVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IImeIPointNotifySink __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IImeIPointNotifySink __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IImeIPointNotifySink __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCursorPos )( 
            IImeIPointNotifySink __RPC_FAR * This,
            /* [in] */ UINT uiCursorPos);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetResultStr )( 
            IImeIPointNotifySink __RPC_FAR * This,
            /* [in] */ UINT cwch,
            /* [in] */ WCHAR __RPC_FAR *pwsz);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsReadingHalfKata )( 
            IImeIPointNotifySink __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetResultReadStr )( 
            IImeIPointNotifySink __RPC_FAR * This,
            /* [in] */ UINT cwch,
            /* [in] */ WCHAR __RPC_FAR *pwsz);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCompositionReadStr )( 
            IImeIPointNotifySink __RPC_FAR * This,
            /* [in] */ UINT cwch,
            /* [in] */ WCHAR __RPC_FAR *pwsz);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCompositionStr )( 
            IImeIPointNotifySink __RPC_FAR * This,
            /* [in] */ UINT cwch,
            /* [in] */ WCHAR __RPC_FAR *pwsz);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetResultClause )( 
            IImeIPointNotifySink __RPC_FAR * This,
            /* [in] */ UINT ciCls,
            /* [in] */ DWORD __RPC_FAR *pdw);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetResultReadClause )( 
            IImeIPointNotifySink __RPC_FAR * This,
            /* [in] */ UINT ciCls,
            /* [in] */ DWORD __RPC_FAR *pdw);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCompositionReadClause )( 
            IImeIPointNotifySink __RPC_FAR * This,
            /* [in] */ UINT ciCls,
            /* [in] */ DWORD __RPC_FAR *pdw);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCompositionClause )( 
            IImeIPointNotifySink __RPC_FAR * This,
            /* [in] */ UINT ciCls,
            /* [in] */ DWORD __RPC_FAR *pdw);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCompositionReadAttr )( 
            IImeIPointNotifySink __RPC_FAR * This,
            /* [in] */ UINT ciAtr,
            /* [in] */ BYTE __RPC_FAR *pb);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCompositionAttr )( 
            IImeIPointNotifySink __RPC_FAR * This,
            /* [in] */ UINT ciAtr,
            /* [in] */ BYTE __RPC_FAR *pb);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetTargetClause )( 
            IImeIPointNotifySink __RPC_FAR * This,
            /* [in] */ UINT uiTarget);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCandidateStr )( 
            IImeIPointNotifySink __RPC_FAR * This,
            /* [in] */ UINT ciCandidate,
            /* [in] */ WCHAR __RPC_FAR *pwsz,
            /* [in] */ UINT cbData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCandidateSelection )( 
            IImeIPointNotifySink __RPC_FAR * This,
            /* [in] */ UINT iciCandidate);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDeltaPos )( 
            IImeIPointNotifySink __RPC_FAR * This,
            /* [in] */ UINT iDelat);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GenerateMessage )( 
            IImeIPointNotifySink __RPC_FAR * This);
        
        END_INTERFACE
    } IImeIPointNotifySinkVtbl;

    interface IImeIPointNotifySink
    {
        CONST_VTBL struct IImeIPointNotifySinkVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IImeIPointNotifySink_QueryInterface(This,riid,ppvObject)    \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IImeIPointNotifySink_AddRef(This)   \
    (This)->lpVtbl -> AddRef(This)

#define IImeIPointNotifySink_Release(This)  \
    (This)->lpVtbl -> Release(This)


#define IImeIPointNotifySink_SetCursorPos(This,uiCursorPos) \
    (This)->lpVtbl -> SetCursorPos(This,uiCursorPos)

#define IImeIPointNotifySink_SetResultStr(This,cwch,pwsz)   \
    (This)->lpVtbl -> SetResultStr(This,cwch,pwsz)

#define IImeIPointNotifySink_IsReadingHalfKata(This)    \
    (This)->lpVtbl -> IsReadingHalfKata(This)

#define IImeIPointNotifySink_SetResultReadStr(This,cwch,pwsz)   \
    (This)->lpVtbl -> SetResultReadStr(This,cwch,pwsz)

#define IImeIPointNotifySink_SetCompositionReadStr(This,cwch,pwsz)  \
    (This)->lpVtbl -> SetCompositionReadStr(This,cwch,pwsz)

#define IImeIPointNotifySink_SetCompositionStr(This,cwch,pwsz)  \
    (This)->lpVtbl -> SetCompositionStr(This,cwch,pwsz)

#define IImeIPointNotifySink_SetResultClause(This,ciCls,pdw)    \
    (This)->lpVtbl -> SetResultClause(This,ciCls,pdw)

#define IImeIPointNotifySink_SetResultReadClause(This,ciCls,pdw)    \
    (This)->lpVtbl -> SetResultReadClause(This,ciCls,pdw)

#define IImeIPointNotifySink_SetCompositionReadClause(This,ciCls,pdw)   \
    (This)->lpVtbl -> SetCompositionReadClause(This,ciCls,pdw)

#define IImeIPointNotifySink_SetCompositionClause(This,ciCls,pdw)   \
    (This)->lpVtbl -> SetCompositionClause(This,ciCls,pdw)

#define IImeIPointNotifySink_SetCompositionReadAttr(This,ciAtr,pb)  \
    (This)->lpVtbl -> SetCompositionReadAttr(This,ciAtr,pb)

#define IImeIPointNotifySink_SetCompositionAttr(This,ciAtr,pb)  \
    (This)->lpVtbl -> SetCompositionAttr(This,ciAtr,pb)

#define IImeIPointNotifySink_SetTargetClause(This,uiTarget) \
    (This)->lpVtbl -> SetTargetClause(This,uiTarget)

#define IImeIPointNotifySink_SetCandidateStr(This,ciCandidate,pwsz,cbData)  \
    (This)->lpVtbl -> SetCandidateStr(This,ciCandidate,pwsz,cbData)

#define IImeIPointNotifySink_SetCandidateSelection(This,iciCandidate)   \
    (This)->lpVtbl -> SetCandidateSelection(This,iciCandidate)

#define IImeIPointNotifySink_SetDeltaPos(This,iDelat)   \
    (This)->lpVtbl -> SetDeltaPos(This,iDelat)

#define IImeIPointNotifySink_GenerateMessage(This)  \
    (This)->lpVtbl -> GenerateMessage(This)

#endif /* COBJMACROS */


#endif  /* C style interface */



HRESULT STDMETHODCALLTYPE IImeIPointNotifySink_SetCursorPos_Proxy( 
    IImeIPointNotifySink __RPC_FAR * This,
    /* [in] */ UINT uiCursorPos);


void __RPC_STUB IImeIPointNotifySink_SetCursorPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IImeIPointNotifySink_SetResultStr_Proxy( 
    IImeIPointNotifySink __RPC_FAR * This,
    /* [in] */ UINT cwch,
    /* [in] */ WCHAR __RPC_FAR *pwsz);


void __RPC_STUB IImeIPointNotifySink_SetResultStr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IImeIPointNotifySink_IsReadingHalfKata_Proxy( 
    IImeIPointNotifySink __RPC_FAR * This);


void __RPC_STUB IImeIPointNotifySink_IsReadingHalfKata_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IImeIPointNotifySink_SetResultReadStr_Proxy( 
    IImeIPointNotifySink __RPC_FAR * This,
    /* [in] */ UINT cwch,
    /* [in] */ WCHAR __RPC_FAR *pwsz);


void __RPC_STUB IImeIPointNotifySink_SetResultReadStr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IImeIPointNotifySink_SetCompositionReadStr_Proxy( 
    IImeIPointNotifySink __RPC_FAR * This,
    /* [in] */ UINT cwch,
    /* [in] */ WCHAR __RPC_FAR *pwsz);


void __RPC_STUB IImeIPointNotifySink_SetCompositionReadStr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IImeIPointNotifySink_SetCompositionStr_Proxy( 
    IImeIPointNotifySink __RPC_FAR * This,
    /* [in] */ UINT cwch,
    /* [in] */ WCHAR __RPC_FAR *pwsz);


void __RPC_STUB IImeIPointNotifySink_SetCompositionStr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IImeIPointNotifySink_SetResultClause_Proxy( 
    IImeIPointNotifySink __RPC_FAR * This,
    /* [in] */ UINT ciCls,
    /* [in] */ DWORD __RPC_FAR *pdw);


void __RPC_STUB IImeIPointNotifySink_SetResultClause_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IImeIPointNotifySink_SetResultReadClause_Proxy( 
    IImeIPointNotifySink __RPC_FAR * This,
    /* [in] */ UINT ciCls,
    /* [in] */ DWORD __RPC_FAR *pdw);


void __RPC_STUB IImeIPointNotifySink_SetResultReadClause_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IImeIPointNotifySink_SetCompositionReadClause_Proxy( 
    IImeIPointNotifySink __RPC_FAR * This,
    /* [in] */ UINT ciCls,
    /* [in] */ DWORD __RPC_FAR *pdw);


void __RPC_STUB IImeIPointNotifySink_SetCompositionReadClause_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IImeIPointNotifySink_SetCompositionClause_Proxy( 
    IImeIPointNotifySink __RPC_FAR * This,
    /* [in] */ UINT ciCls,
    /* [in] */ DWORD __RPC_FAR *pdw);


void __RPC_STUB IImeIPointNotifySink_SetCompositionClause_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IImeIPointNotifySink_SetCompositionReadAttr_Proxy( 
    IImeIPointNotifySink __RPC_FAR * This,
    /* [in] */ UINT ciAtr,
    /* [in] */ BYTE __RPC_FAR *pb);


void __RPC_STUB IImeIPointNotifySink_SetCompositionReadAttr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IImeIPointNotifySink_SetCompositionAttr_Proxy( 
    IImeIPointNotifySink __RPC_FAR * This,
    /* [in] */ UINT ciAtr,
    /* [in] */ BYTE __RPC_FAR *pb);


void __RPC_STUB IImeIPointNotifySink_SetCompositionAttr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IImeIPointNotifySink_SetTargetClause_Proxy( 
    IImeIPointNotifySink __RPC_FAR * This,
    /* [in] */ UINT uiTarget);


void __RPC_STUB IImeIPointNotifySink_SetTargetClause_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IImeIPointNotifySink_SetCandidateStr_Proxy( 
    IImeIPointNotifySink __RPC_FAR * This,
    /* [in] */ UINT ciCandidate,
    /* [in] */ WCHAR __RPC_FAR *pwsz,
    /* [in] */ UINT cbData);


void __RPC_STUB IImeIPointNotifySink_SetCandidateStr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IImeIPointNotifySink_SetCandidateSelection_Proxy( 
    IImeIPointNotifySink __RPC_FAR * This,
    /* [in] */ UINT iciCandidate);


void __RPC_STUB IImeIPointNotifySink_SetCandidateSelection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IImeIPointNotifySink_SetDeltaPos_Proxy( 
    IImeIPointNotifySink __RPC_FAR * This,
    /* [in] */ UINT iDelat);


void __RPC_STUB IImeIPointNotifySink_SetDeltaPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IImeIPointNotifySink_GenerateMessage_Proxy( 
    IImeIPointNotifySink __RPC_FAR * This);


void __RPC_STUB IImeIPointNotifySink_GenerateMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IImeIPointNotifySink_INTERFACE_DEFINED__ */


///////////////////////////////////
// Definitions for PerIME Interface
///////////////////////////////////

/***********************************************************************
    IME Version IDs
 ***********************************************************************/
#define VERSION_ID_JAPANESE                 0x01000000
#define VERSION_ID_KOREAN                   0x02000000
#define VERSION_ID_CHINESE_TRADITIONAL      0x04000000
#define VERSION_ID_CHINESE_SIMPLIFIED       0x08000000

#define VERSION_ID_IMEJP98  (VERSION_ID_JAPANESE | 0x980)
#define VERSION_ID_IMEJP2000 (VERSION_ID_JAPANESE | 0x98a)

/***********************************************************************
    Msg:    WM_MSIME_SERVICE
    Desc:   service functions
    Dir:    Apps to IME
    wParam: reserved
    lParam: reserved
 ***********************************************************************/

// Label for RegisterWindowMessage
#define RWM_SERVICE     TEXT("MSIMEService")

//getting version number (wParam)
#define FID_MSIME_VERSION       0

/***********************************************************************
    Msg:    WM_MSIME_UIREADY
    Desc:   service functions
    Dir:    IME to Apps
    wParam: Version ID
    lParam: reserved
 ***********************************************************************/

// Label for RegisterWindowMessage
#define RWM_UIREADY     TEXT("MSIMEUIReady")



/***********************************************************************
    Msg:    WM_MSIME_MOUSE
    Desc:   mouse operation definition
    Dir:    Apps to IME
 ***********************************************************************/

// Label for RegisterWindowMessage
#define RWM_MOUSE       TEXT("MSIMEMouseOperation")

// Mouse Operation Version (return value of IMEMOUSE_VERSION)
#define VERSION_MOUSE_OPERATION     1

// Mouse operation result
#define IMEMOUSERET_NOTHANDLED      (-1)

//WParam definition for WM_IME_MOUSE.
#define IMEMOUSE_VERSION    0xff    // mouse supported?

#define IMEMOUSE_NONE       0x00    // no mouse button was pushed
#define IMEMOUSE_LDOWN      0x01
#define IMEMOUSE_RDOWN      0x02
#define IMEMOUSE_MDOWN      0x04
#define IMEMOUSE_WUP        0x10    // wheel up
#define IMEMOUSE_WDOWN      0x20    // wheel down


/***********************************************************************
    Msg:    WM_MSIME_RECONVERT
    Desc:   reconversion
    Dir:    IME to Apps
 ***********************************************************************/

// Label for RegisterWindowMessage
#define RWM_RECONVERT           TEXT("MSIMEReconvert")


/***********************************************************************
    Msg:    WM_MSIME_RECONVERTREQUEST
    Desc:   reconversion
    Dir:    Apps to IME
 ***********************************************************************/

// wParam of WM_MSIME_RECONVERTREQUEST
#define FID_RECONVERT_VERSION   0x10000000

// Private reconversion Version
#define VERSION_RECONVERSION        1

// Label for RegisterWindowMessage
#define RWM_RECONVERTREQUEST    TEXT("MSIMEReconvertRequest")


/***********************************************************************
    Msg:    WM_MSIME_DOCUMENTFEED
    Desc:   Document feeding
    Dir:    IME to Apps
    Usage:  SendMessage( hwndApp, WM_MSIME_DOCUMENTFEED, VERSION_DOCUMENTFEED,
                (RECONVERTSTRING*)pReconv );
    wParam: VERSION_DOCUMENTFEED
    lParam: Pointer of RECONVERTSTRING structure
    return: size of RECONVERTSTRING structure
 ***********************************************************************/

// wParam of WM_MSIME_DOCUMENTFEED (set current docfeed version)
#define VERSION_DOCUMENTFEED        1

// lParam is pointer of RECONVERTSTRING structure

// Label for RegisterWindowMessage
#define RWM_DOCUMENTFEED    TEXT("MSIMEDocumentFeed")

/***********************************************************************
    Msg:    WM_MSIME_QUERYPOSITION
    Desc:   composition UI
    Dir:    IME to Apps
    Usage:  SendMessage( hwndApp, WM_MSIME_QUERYPOSITION, VERSION_QUERYPOSITION, (IMEPOSITION*)pPs );
    wParam: reserved. must be 0.
    lParam: pointer of IMEPOSITION structure
    return: Non-zero = success. Zero = error.
 ***********************************************************************/

// wParam of WM_MSIME_QUERYPOSITION
#define VERSION_QUERYPOSITION       1

// Label for RegisterWindowMessage
#define RWM_QUERYPOSITION   TEXT("MSIMEQueryPosition")


/***********************************************************************
    Msg:    WM_MSIME_MODEBIAS
    Desc:   input mode bias
    Dir:    Apps to IME
    Usage:  SendMessage( hwndDefUI, WM_MSIME_MODEBIAS, MODEBIAS_xxxx, MODEBIASMODE_xxxx );
    wParam: operation of bias
    lParam: bias mode
    return: If wParam is MODEBIAS_GETVERSION,returns version number of interface.
            If wParam is MODEBIAS_SETVALUE : return non-zero value if succeeded. Returns 0 if fail.
            If wParam is MODEBIAS_GETVALUE : returns current bias mode.
 ***********************************************************************/

// Label for RegisterWindowMessage
#define RWM_MODEBIAS            TEXT("MSIMEModeBias")

// Current version
#define VERSION_MODEBIAS        1

// Set or Get (wParam)
#define MODEBIAS_GETVERSION     0
#define MODEBIAS_SETVALUE       1
#define MODEBIAS_GETVALUE       2

// Bias (lParam)
#define MODEBIASMODE_DEFAULT                0x00000000  // reset all of bias setting
#define MODEBIASMODE_FILENAME               0x00000001  // filename


/***********************************************************************
    Msg:    WM_MSIME_SHOWIMEPAD
    Desc:   show ImePad
    Usage: SendMessage( hwndDefUI, WM_MSIME_SHOWIMEPAD, wParam, lParam );
    wParam: Applet selection option
    lParam: Applet selection parameter
            (Category defined in imepad.h or a pointer to GUID for Applet)
    return: Non-zero = accepted. Zero = not accepted.
 ***********************************************************************/

// Label for RegisterWindowMessage
#define RWM_SHOWIMEPAD          TEXT("MSIMEShowImePad")

// Applet selection option
#define SHOWIMEPAD_DEFAULT                  0x00000000  // default applet
#define SHOWIMEPAD_CATEGORY                 0x00000001  // selection by applet category
#define SHOWIMEPAD_GUID                     0x00000002  // selection by applet GUID


/***********************************************************************
    Msg:    WM_MSIME_KEYMAP
    Desc:   key map sharing with apps
 ***********************************************************************/

// Label for RegisterWindowMessage
#define RWM_KEYMAP              TEXT("MSIMEKeyMap")
#define RWM_CHGKEYMAP           TEXT("MSIMEChangeKeyMap")
#define RWM_NTFYKEYMAP          TEXT("MSIMENotifyKeyMap")

#define FID_MSIME_KMS_VERSION       1
#define FID_MSIME_KMS_INIT          2
#define FID_MSIME_KMS_TERM          3
#define FID_MSIME_KMS_DEL_KEYLIST   4
#define FID_MSIME_KMS_NOTIFY        5
#define FID_MSIME_KMS_GETMAP        6
#define FID_MSIME_KMS_INVOKE        7
#define FID_MSIME_KMS_SETMAP        8

#define IMEKMS_NOCOMPOSITION        0
#define IMEKMS_COMPOSITION          1
#define IMEKMS_SELECTION            2
#define IMEKMS_IMEOFF               3
#define IMEKMS_2NDLEVEL             4   // Reserved
#define IMEKMS_INPTGL               5   // Reserved
#define IMEKMS_CANDIDATE            6   // Reserved
#define IMEKMS_TYPECAND             7    //Reserved


typedef struct tagIMEKMSINIT {
    INT         cbSize;
    HWND        hWnd;   // Window which receives notification from IME.
                        // If hWnd is NULL, no notification is posted
                        // to Input context.
} IMEKMSINIT;

typedef struct tagIMEKMSKEY {
    DWORD dwStatus;     //Shift-Control combination status.
                        //Any combination of constants below
                        //(defined in IMM.H)
                        // 0x0000 (default)
                        // MOD_CONTROL     0x0002
                        // MOD_SHIFT       0x0004
                        // Alt key and Win key is not processed by IME.

    DWORD dwCompStatus; //Composition string status
                        //One of the constants below
                        // IMEKMS_NOCOMPOSITION  No composition string
                        // IMEKMS_COMPOSITION    Some composition string
                        // IMEKMS_SELECTION      Selection exists in apps
                        // IMEKMS_IMEOFF         IME Off state


    DWORD dwVKEY;       // VKEY code defined in IMM.H
    union {
        DWORD dwControl;// IME Functionality ID
        DWORD dwNotUsed;
    };
    union {
        WCHAR pwszDscr[31];// The pointer to string of description of this functionalify
        WCHAR pwszNoUse[31];
    };
} IMEKMSKEY;

typedef struct tagIMEKMS {
    INT         cbSize;
    HIMC        hIMC;
    DWORD       cKeyList;
    IMEKMSKEY   *pKeyList;
} IMEKMS;

typedef struct tagIMEKMSNTFY {
    INT         cbSize;
    HIMC        hIMC;
    BOOL        fSelect;
} IMEKMSNTFY;

typedef struct tagIMEKMSKMP {
    INT         cbSize;         //[in] size of this structure
    HIMC        hIMC;           //[in] Input context
    LANGID      idLang;         //[in] Language ID
    WORD        wVKStart;       //[in] VKEY start
    WORD        wVKEnd;         //[in] VKEY end
    INT         cKeyList;       //[out] number of IMEKMSKEY
    IMEKMSKEY   *pKeyList;      //[out] retrieve buffer of IMEKMSKEY
                                //      Must be GlobalMemFree by clients
} IMEKMSKMP;

typedef struct tagIMEKMSINVK {
    INT         cbSize;
    HIMC        hIMC;
    DWORD       dwControl;
} IMEKMSINVK;


/***********************************************************************
    Msg:    WM_MSIME_RECONVERTOPTIONS
    Desc:   Set reconversion options
    Usage: SendMessage( hwndDefUI, WM_MSIME_RECONVERTOPTIONS, dwOpt, (LPARAM)(HIMC)hIMC );
    wParam: options
    lParam: Input context handle
    return: Non-zero = accepted. Zero = not accepted.
 ***********************************************************************/

// Label for RegisterWindowMessage
#define RWM_RECONVERTOPTIONS          TEXT("MSIMEReconvertOptions")

//WParam definition for WM_IME_RECONVERTOPTIONS.
#define RECONVOPT_NONE              0x00000000  // default
#define RECONVOPT_USECANCELNOTIFY   0x00000001  // cancel notify

// parameter of ImmGetCompositionString
#define GCSEX_CANCELRECONVERT       0x10000000

#pragma pack()


#ifdef __cplusplus
extern "C" {
#endif

// The following API replaces CoCreateInstance(), when CLSID is not used.
HRESULT WINAPI CreateIFECommonInstance(VOID **ppvObj);
typedef HRESULT (WINAPI *fpCreateIFECommonInstanceType)(VOID **ppvObj);

// The IFELanguage global export function
HRESULT WINAPI CreateIFELanguageInstance(REFCLSID clsid, VOID **ppvObj);
typedef HRESULT (WINAPI *fpCreateIFELanguageInstanceType)(REFCLSID clsid, VOID **ppvObj);

// The following API replaces CoCreateInstance(), when CLSID is not used.
HRESULT WINAPI CreateIFEDictionaryInstance(VOID **ppvObj);
typedef HRESULT (WINAPI *fpCreateIFEDictionaryInstanceType)(VOID **ppvObj);

// The following API replaces CoCreateInstance(), when CLSID is not used.
HRESULT WINAPI CreateIImeIPointInstance(REFCLSID clsid, VOID **ppvObj);
typedef HRESULT (WINAPI *fpCreateIImeIPointInstance)(REFCLSID clsid, VOID **ppvObj);

#ifdef __cplusplus
} /* end of 'extern "C" {' */
#endif

#endif //__MSIME_H__
