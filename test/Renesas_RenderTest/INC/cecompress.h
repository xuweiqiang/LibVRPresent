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

#ifndef _CECOMPRESS_H_
#define _CECOMPRESS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ---------------------- Common declarations ------------------------- */
/*                        -------------------                           */

// max. size of input block
#define CECOMPRESS_MAX_BLOCK_LOG    16
#define CECOMPRESS_MAX_BLOCK        (1 << CECOMPRESS_MAX_BLOCK_LOG)


// preferred data alignment to avoid misaligned accesses
#define CECOMPRESS_ALIGNMENT        8

// user-supplied callback function that allocates memory
// if there is no memory available it shall return NULL
typedef
LPVOID
CeCompressAllocFn (
  LPVOID    Context,        // user-defined context (as passed to CeCompressEncodeCreate)
  DWORD     AllocSize       // size of memory block to allocate (bytes)
);

// user-supplied callback function that releases memory
typedef
VOID
CeCompressFreeFn (
  LPVOID Context,        // user-defined context (as passed to CeCompressEncodeClose)
  LPVOID Address         // pointer to the block to be freed
);


/* ----------------------------- Encoder ------------------------------ */
/*                               -------                                */

// declare unique anonymous types for type safety
typedef struct {DWORD CeCompressEncodeDummy;} *CeCompressEncodeStream;

// allocate and initialize encoder's data structures
// returns NULL if callback returned NULL (not enough memory)
CeCompressEncodeStream
CeCompressEncodeCreate (
  DWORD             MaxOrigSize,            // max size of original data block (bytes)
  LPVOID            Context,                // user-defined context info (will  be passed to AllocFn)
  CeCompressAllocFn *AllocFn                // memory allocation callback
);

// returns size of compressed data
// if compression failed then compressed buffer is left as is, and
// original data should be saved instead
DWORD
CeCompressEncode (
  CeCompressEncodeStream EncodeStream,      // encoder's workspace
  LPVOID                CompAdr,            // address of beggining of output memory region
  DWORD                 CompSize,           // size of output memory region (bytes)
  LPCVOID               OrigAdr,            // address of beggining of input data block
  DWORD                 OrigSize            // input data block size (bytes)
); 

// invalidate encoding stream and release workspace memory 
VOID 
CeCompressEncodeClose (
  CeCompressEncodeStream EncodeStream,      // encoder's workspace
  LPVOID                Context,            // user-defined context for FreeFn
  CeCompressFreeFn      *FreeFn             // memory release callback
);


/* ----------------------------- Decoder ------------------------------ */
/*                               -------                                */

// declare unique anonymous types for type safety
typedef struct {int CeCompressDecodeDummy;} *CeCompressDecodeStream;

// allocate memory for decoder. Returns NULL if not enough memory.
CeCompressDecodeStream
CeCompressDecodeCreate (
  LPVOID            Context,                // user-defined context info (will  be passed to AllocFn)
  CeCompressAllocFn *AllocFn                // memory allocation callback
);

// decode compressed block. Returns # of decoded bytes or -1 otherwise
DWORD
CeCompressDecode (
  CeCompressDecodeStream DecodeStream,      // decoder's workspace
  LPVOID                OrigAdr,            // address of beginning out output memory region
  DWORD                 OrigSize,           // size of output memory region (bytes)
  DWORD                 DecodeSize,         // # of bytes to decode ( <= OrigSize)
  LPCVOID               CompAdr,            // address of beginning of compressed data block
  DWORD                 CompSize            // size of compressed data block (bytes)
);

// invalidate decoding stream and release workspace memory
VOID
CeCompressDecodeClose (
  CeCompressDecodeStream DecodeStream,      // encoder's workspace
  LPVOID                Context,            // user-defined context info (will  be passed to FreeFn)
  CeCompressFreeFn      *FreeFn             // callback that releases the memory
);


#ifdef __cplusplus
};
#endif

#endif /* _CECOMPRESS_H_ */
