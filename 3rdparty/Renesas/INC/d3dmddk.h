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
// This is a prerelease header file and subject to change.
//
// d3dmddk.h
// Direct3D Mobile Driver Interface Header File

#pragma once

#define D3DM_DRIVER_NOTHANDLED 0
#define D3DM_DRIVER_HANDLED    1

// We need to be able to refer to resource types in bit flag form (for the
// D3DMDEVICEFORMAT below).

#define D3DMRTYPEFLAG_SURFACE       (1 << D3DMRTYPE_SURFACE)
#define D3DMRTYPEFLAG_DEPTHSTENCIL  (1 << D3DMRTYPE_DEPTHSTENCIL)
#define D3DMRTYPEFLAG_TEXTURE       (1 << D3DMRTYPE_TEXTURE)     
#define D3DMRTYPEFLAG_FRONTBUFFER   (1 << D3DMRTYPE_FRONTBUFFER)
#define D3DMRTYPEFLAG_BACKBUFFER    (1 << D3DMRTYPE_BACKBUFFER)
#define D3DMRTYPEFLAG_VERTEXBUFFER  (1 << D3DMRTYPE_VERTEXBUFFER)
#define D3DMRTYPEFLAG_INDEXBUFFER   (1 << D3DMRTYPE_INDEXBUFFER)

// Direct3D Mobile must not only know what formats a driver supports, but what
// the driver can do with those formats. This structure describes both.

typedef struct _D3DMDEVICEFORMAT {

    D3DMFORMAT Format;
    ULONG      TypeFlags;
    ULONG      Usage;
} D3DMDEVICEFORMAT;

// These are description structures for the resource types that are not exposed
// through the API.

typedef struct _D3DMTEXTURE_DESC {

    // The first part of the texture description is intentionally identical to
    // the surface description. It will be kept so.

    D3DMFORMAT          Format;
    D3DMRESOURCETYPE    Type;
    ULONG               Usage;
    D3DMPOOL            Pool;
    UINT                Size;

    D3DMMULTISAMPLE_TYPE MultiSampleType;
    UINT                 Width;
    UINT                 Height;

    ULONG                Level;
    ULONG                TotalLevels;

} D3DMTEXTURE_DESC;

typedef struct _D3DMDEPTHSTENCIL_DESC {

    // The first part of the texture description is intentionally identical to
    // the surface description. It will be kept so.

    D3DMFORMAT          Format;
    D3DMRESOURCETYPE    Type;
    ULONG               Usage;
    D3DMPOOL            Pool;
    UINT                Size;

    D3DMMULTISAMPLE_TYPE MultiSampleType;
    UINT                 Width;
    UINT                 Height;

} D3DMDEPTHSTENCIL_DESC;

typedef struct _D3DMFRONTBUFFER_DESC {

    // The first part of the front buffer description is intentionally 
    // identical to the surface description. It will be kept so.

    D3DMFORMAT          Format;
    D3DMRESOURCETYPE    Type;
    ULONG               Usage;
    D3DMPOOL            Pool;
    UINT                Size;

    D3DMMULTISAMPLE_TYPE MultiSampleType;
    UINT                 Width;
    UINT                 Height;

    // The driver can expect this many create surface calls following the
    // initial front buffer call to create back buffers.

    ULONG               TotalBackBuffers;

} D3DMFRONTBUFFER_DESC;

typedef struct _D3DMBACKBUFFER_DESC {

    // The first part of the back buffer description is intentionally identical
    // to the surface description. It will be kept so.

    D3DMFORMAT          Format;
    D3DMRESOURCETYPE    Type;
    ULONG               Usage;
    D3DMPOOL            Pool;
    UINT                Size;

    D3DMMULTISAMPLE_TYPE MultiSampleType;
    UINT                 Width;
    UINT                 Height;

    // The back buffer number.

    ULONG               BackBuffer;

} D3DMBACKBUFFER_DESC;

// This structure is used to describe the possible formats a source format may be
// converted into.

typedef struct _D3DMFORMATCONVERSION {

    D3DMFORMAT SourceFormat;
    ULONG      DestCount;
    D3DMFORMAT DestFormats[1];

} D3DMFORMATCONVERSION;

// Parameter structures for each D3DM driver entry point.

typedef struct _D3DM_GETADAPTERINFO_DATA {

    D3DMADAPTER_IDENTIFIER* pIdentifier;
    D3DMCAPS              * pCaps;
    ULONG                   cDisplayMode;
    D3DMDISPLAYMODE       * pDisplayMode;
    ULONG                   nCurrentMode;
    ULONG                   nFormat;
    D3DMDEVICEFORMAT      * pFormat;
    ULONG                   nMultisample;
    D3DMMULTISAMPLE_TYPE  * pMultisample;
	ULONG                   nProfile;
	D3DMPROFILE           * pProfile;
    ULONG                   nConversionBytes;
    D3DMFORMATCONVERSION  * pConversion;
    ULONG                   cTotalVRam;
    ULONG                   cCurrentVRam;
    HRESULT                 rval;
} D3DM_GETADAPTERINFO_DATA;

typedef struct _D3DM_CREATECONTEXT_DATA {

    BOOL           bWindowed;
    D3DMSWAPEFFECT SwapEffect;
    DWORD          nVSyncInterval;
    ULONG          nContextId;
    HRESULT        rval;
} D3DM_CREATECONTEXT_DATA;

typedef struct _D3DM_DESTROYCONTEXT_DATA {

    ULONG   nContextId;
    HRESULT rval;
} D3DM_DESTROYCONTEXT_DATA;

typedef struct _D3DM_CREATESURFACE_DATA {

    D3DMRESOURCETYPE           SurfaceType;
    union {
        D3DMSURFACE_DESC       SurfaceDesc;
        D3DMVERTEXBUFFER_DESC  VertexDesc;
        D3DMINDEXBUFFER_DESC   IndexDesc;
        D3DMTEXTURE_DESC       TextureDesc;
        D3DMFRONTBUFFER_DESC   FrontDesc;
        D3DMBACKBUFFER_DESC    BackDesc;
        D3DMDEPTHSTENCIL_DESC  DepthDesc;
    }; 
    ULONG                      nContextId;
    ULONG                      nSurfaceId;
    HRESULT                    rval;
} D3DM_CREATESURFACE_DATA;

typedef struct _D3DM_DESTROYSURFACE_DATA {

    ULONG   nSurfaceId;
    HRESULT rval;
} D3DM_DESTROYSURFACE_DATA;

typedef struct _D3DM_LOCKSURFACE_DATA {

    RECT             Rect;
    D3DMLOCKED_RECT  LockedRect;
    ULONG            nSurfaceId;
    ULONG            Flags;
    HRESULT          rval;
} D3DM_LOCKSURFACE_DATA;

typedef struct _D3DM_UNLOCKSURFACE_DATA {

    ULONG            nSurfaceId;
    HRESULT          rval;
} D3DM_UNLOCKSURFACE_DATA;

typedef struct _D3DM_DRAWPRIMITIVE_DATA {

    ULONG   nContextId;
    LPVOID  pCommands;
    ULONG   nStartOffset;
    ULONG   nEndOffset;
    HRESULT rval;
} D3DM_DRAWPRIMITIVE_DATA;

typedef struct _D3DM_VALIDATEDEVICE_DATA {

    ULONG   nContextId;
    DWORD   NumPasses;
    HRESULT rval;
} D3DM_VALIDATEDEVICE_DATA;

typedef struct _D3DM_GETINFO_DATA {

    ULONG   nContextId;
    DWORD   DevInfoId;
    void*   pDevInfoStruct;
    DWORD   DevInfoStructSize;
    HRESULT rval;
} D3DM_GETINFO_DATA;

#define D3DM_GETINFO_GETCLIPSTATUS_DEVINFOID 0x00000001
#define D3DM_GETINFO_SETCLIPSTATUS_DEVINFOID 0x00000002

typedef enum _D3DM_RESOURCEMANAGER_ACTION {

    D3DM_RESACTION_DISCARD_BYTES    = 0,
    D3DM_RESACTION_SET_PRIORITY     = 1,
    D3DM_RESACTION_PRELOAD          = 2,

    D3DM_RESACTION_FORCE_ULONG      = 0xFFFFFFFF,   // force 32-bit size enum

} D3DM_RESOURCEMANAGER_ACTION;

typedef struct _D3DM_RESOURCEMANAGER_DATA {

    D3DM_RESOURCEMANAGER_ACTION Action;
    ULONG                       nContextId;
    ULONG                       nDiscardBytes;
    ULONG                       nSurfaceId;
    ULONG                       nNewPriority;
    HRESULT                     rval;

} D3DM_RESOURCEMANAGER_DATA;

typedef DWORD (*D3DM_GETADAPTERINFO_PTR)(D3DM_GETADAPTERINFO_DATA *);
typedef DWORD (*D3DM_CREATECONTEXT_PTR)(D3DM_CREATECONTEXT_DATA *);
typedef DWORD (*D3DM_DESTROYCONTEXT_PTR)(D3DM_DESTROYCONTEXT_DATA *);
typedef DWORD (*D3DM_CREATESURFACE_PTR)(D3DM_CREATESURFACE_DATA *);
typedef DWORD (*D3DM_DESTROYSURFACE_PTR)(D3DM_DESTROYSURFACE_DATA *);
typedef DWORD (*D3DM_LOCKSURFACE_PTR)(D3DM_LOCKSURFACE_DATA *);
typedef DWORD (*D3DM_UNLOCKSURFACE_PTR)(D3DM_UNLOCKSURFACE_DATA *);
typedef DWORD (*D3DM_DRAWPRIMITIVE_PTR)(D3DM_DRAWPRIMITIVE_DATA *);
typedef DWORD (*D3DM_VALIDATEDEVICE_PTR)(D3DM_VALIDATEDEVICE_DATA *);
typedef DWORD (*D3DM_GETINFO_PTR)(D3DM_GETINFO_DATA *);
typedef DWORD (*D3DM_RESOURCEMANAGER_PTR)(D3DM_RESOURCEMANAGER_DATA *);

typedef struct _D3DM_INITIALIZE_DATA {

    D3DM_GETADAPTERINFO_PTR  pGetAdapterInfo;
    D3DM_CREATECONTEXT_PTR   pCreateContext;
    D3DM_DESTROYCONTEXT_PTR  pDestroyContext;
    D3DM_CREATESURFACE_PTR   pCreateSurface;
    D3DM_DESTROYSURFACE_PTR  pDestroySurface;
    D3DM_LOCKSURFACE_PTR     pLockSurface;
    D3DM_UNLOCKSURFACE_PTR   pUnlockSurface;
    D3DM_DRAWPRIMITIVE_PTR   pDrawPrimitive;
    D3DM_VALIDATEDEVICE_PTR  pValidateDevice;
    D3DM_GETINFO_PTR         pGetInfo;
    D3DM_RESOURCEMANAGER_PTR pResourceManager;

} D3DM_INITIALIZE_DATA;

// This is the name of the entry point D3DM expect's it's driver dll to have.

#define D3DM_DLL_ENTRY TEXT("D3DM_Initialize")

// This function pointer type defines the expected prototype of the the
// D3DM_Initialize driver dll entry point.

typedef DWORD (*D3DM_INITIALIZE_PTR)(D3DM_INITIALIZE_DATA *);

// These are the command buffer token types.

typedef enum _D3DM_OPERATION {

    D3DMOP_BEGINSCENE,
    D3DMOP_ENDSCENE,
    D3DMOP_PRESENT,
    
    D3DMOP_COPYRECT,
    D3DMOP_CLEAR,

    D3DMOP_RENDERTARGET,
    D3DMOP_RENDERSTATE,

    D3DMOP_TRANSFORM,
    D3DMOP_VIEWPORT,
    D3DMOP_MATERIAL,
    D3DMOP_CREATELIGHT,
    D3DMOP_SETLIGHT,

    D3DMOP_TEXTURESTAGESTATE,

    D3DMOP_CREATEPALETTE,
    D3DMOP_SETPALETTE,
    
    D3DMOP_PROCESSVERTICES,
    D3DMOP_DRAWPRIMITIVE,
    D3DMOP_DRAWINDEXEDPRIMITIVE,

    D3DMOP_VERTEXBUFFER,
    D3DMOP_INDEXBUFFER,

    D3DMOP_COLORFILL,
    D3DMOP_STRETCHRECT

} D3DM_OPERATION;

// This header prefixes all command tokens. It identifies the type and count of the
// tokens to follow.

typedef struct _D3DM_COMMAND {

    BYTE nCommand;
    BYTE nReserved;
    WORD nCount;

} D3DM_COMMAND, *LPD3DM_COMMAND;

// These are the structures that match each individual operation code.

typedef struct _D3DM_RENDERSTATE
{
    D3DMRENDERSTATETYPE RenderState;
    D3DMVALUE           Value;
        
} D3DM_RENDERSTATE, * LPD3DM_RENDERSTATE;

typedef struct _D3DM_TRANSFORM
{
    D3DMTRANSFORMSTATETYPE TransformType;
    D3DMMATRIX             Matrix;
	D3DMFORMAT             Format;

} D3DM_TRANSFORM, * LPD3DM_TRANSFORM;

typedef struct _D3DM_VIEWPORT
{
    D3DMVIEWPORT Viewport;

} D3DM_VIEWPORT, * LPD3DM_VIEWPORT;

typedef struct _D3DM_MATERIAL
{
    D3DMMATERIAL Material;
	D3DMFORMAT   Format;

} D3DM_MATERIAL, * LPD3DM_MATERIAL;

typedef struct _D3DM_CREATELIGHT
{
    DWORD Index;

} D3DM_CREATELIGHT, * LPD3DM_CREATELIGHT;

typedef enum _D3DM_SETLIGHT_DATATYPE {

    D3DM_SETLIGHT_ENABLE = 0,
    D3DM_SETLIGHT_DISABLE = 1,
    D3DM_SETLIGHT_DATA_ENABLE = 2,
    D3DM_SETLIGHT_DATA_DISABLE = 3,

} D3DM_SETLIGHT_DATATYPE;

typedef struct _D3DM_SETLIGHT
{
    DWORD                  Index;
    D3DM_SETLIGHT_DATATYPE DataType;

} D3DM_SETLIGHT, * LPD3DM_SETLIGHT;

typedef struct _D3DM_SETLIGHT_WITHDATA
{
    DWORD                  Index;
    D3DM_SETLIGHT_DATATYPE DataType;
    D3DMLIGHT              Data;
	D3DMFORMAT             Format;

} D3DM_SETLIGHT_WITHDATA, * LPD3DM_SETLIGHT_WITHDATA;

// This is a valid value for state in addition to the actual state enumeration
// values. It is used to set the current texture handle for the stage.

#define D3DMTSS_TEXTURE 0

typedef struct _D3DM_TEXTURESTAGESTATE
{
    WORD  Stage;
    WORD  State;
    DWORD Value;

} D3DM_TEXTURESTAGESTATE, * LPD3DM_TEXTURESTAGESTATE;

typedef struct _D3DM_VERTEXBUFFER
{
    DWORD StreamIndex;
    ULONG BufferId;
    DWORD Stride;

} D3DM_VERTEXBUFFER, * LPD3DM_VERTEXBUFFER;

typedef struct _D3DM_INDEXBUFFER
{
    ULONG BufferId;
    DWORD Stride;

} D3DM_INDEXBUFFER, * LPD3DM_INDEXBUFFER;

// The clear token has a variable length array on the end. The Rects array
// is actually sized by the NumRects member.

typedef struct _D3DM_CLEAR
{
    DWORD Flags; 
    DWORD FillColor;
    float FillDepth;
    DWORD FillStencil;
    DWORD NumRects;
    RECT  Rects[1];

} D3DM_CLEAR, * LPD3DM_CLEAR;

typedef struct _D3DM_DRAWPRIMITIVE
{
    D3DMPRIMITIVETYPE PrimType;
    DWORD             StartVertex;
    DWORD             PrimitiveCount;

} D3DM_DRAWPRIMITIVE, * LPD3DM_DRAWPRIMITIVE;

typedef struct _D3DM_DRAWINDEXEDPRIMITIVE
{
    D3DMPRIMITIVETYPE PrimType;
    DWORD             MinIndex;
    DWORD             NumVertices;
    DWORD             StartIndexOffset;
    DWORD             PrimitiveCount;
    INT               BaseVertexIndex;

} D3DM_DRAWINDEXEDPRIMITIVE, * LPD3DM_DRAWINDEXEDPRIMITIVE;

typedef struct _D3DM_RENDERTARGET
{
    ULONG RenderTarget;
    ULONG ZBuffer;

} D3DM_RENDERTARGET, * LPD3DM_RENDERTARGET;

typedef struct _D3DM_PRESENT
{
    ULONG          PresentSurface;
    RECT           SourceRect;
    RECT           DestinationRect;
    DWORD          NumClippingRects;
    RECT           ClippingRects[1];

} D3DM_PRESENT, * LPD3DM_PRESENT;

typedef struct _D3DM_CREATEPALETTE
{
    DWORD Index;

} D3DM_CREATEPALETTE, * LPD3DM_CREATEPALETTE;

typedef enum _D3DM_SETPALETTE_DATATYPE {

    D3DM_SETPALETTE_SETACTIVE = 0,
    D3DM_SETPALETTE_SETENTRIES = 1,

} D3DM_SETPALETTE_DATATYPE;

typedef struct _D3DM_SETPALETTE
{
    DWORD                    Index;
    D3DM_SETPALETTE_DATATYPE DataType;

} D3DM_SETPALETTE, * LPD3DM_SETPALETTE;

typedef struct _D3DM_SETPALETTE_WITHENTRIES
{
    DWORD                    Index;
    D3DM_SETPALETTE_DATATYPE DataType;
    PALETTEENTRY             Entries[256];

} D3DM_SETPALETTE_WITHENTRIES, * LPD3DM_SETPALETTE_WITHENTRIES;

typedef struct _D3DM_COPYRECT
{
    ULONG SourceSurf;
    RECT  SourceRect;
    ULONG DestSurf;
    POINT DestPoint;

} D3DM_COPYRECT, * LPD3DM_COPYRECT;

typedef struct _D3DM_PROCESSVERTICES
{
    DWORD SrcStartIndex;
    DWORD DestIndex;
    DWORD VertexCount;
    ULONG DestBufferId;
    DWORD Flags;

} D3DM_PROCESSVERTICES, * LPD3DM_PROCESSVERTICES;

typedef struct _D3DM_COLORFILL
{
    D3DMCOLOR Color;
    ULONG     SurfaceId;
    RECT      Rect;

} D3DM_COLORFILL, * LPD3DM_COLORFILL;

typedef struct _D3DM_STRETCHRECT
{
    ULONG                 SourceId;
    RECT                  SourceRect;
    ULONG                 DestId;
    RECT                  DestRect;
    D3DMTEXTUREFILTERTYPE Filter;

} D3DM_STRETCHRECT, * LPD3DM_STRETCHRECT;
