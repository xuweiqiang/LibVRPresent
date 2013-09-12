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

Module Name:

    DwCeDump.h

Abstract:

    Public header file for Windows CE crash dumps.

--*/

#include <pshpack4.h>

#pragma warning(disable:4200) // Zero length array

#define CEDUMP_SIGNATURE_CONTEXT  ('XDEC')
#define CEDUMP_SIGNATURE_SYSTEM   ('SDEC')
#define CEDUMP_SIGNATURE_COMPLETE ('CDEC')
#define CEDUMP_VERSION   (2)
#define CEDUMP_ALIGNMENT (8)

#ifdef UNDER_CE 

typedef DWORD RVA;

C_ASSERT (sizeof (RVA) == 4);

typedef struct _MINIDUMP_LOCATION_DESCRIPTOR
{
    ULONG32 DataSize;
    RVA Rva;
} MINIDUMP_LOCATION_DESCRIPTOR;

typedef struct _MINIDUMP_MEMORY_DESCRIPTOR
{
    ULONG64 StartOfMemoryRange;
    MINIDUMP_LOCATION_DESCRIPTOR Memory;
} MINIDUMP_MEMORY_DESCRIPTOR, *PMINIDUMP_MEMORY_DESCRIPTOR;

typedef struct _MINIDUMP_HEADER
{
    ULONG32 Signature;
    ULONG32 Version;
    ULONG32 NumberOfStreams;
    RVA StreamDirectoryRva;
    ULONG32 CheckSum;
    union
    {
        ULONG32 Reserved;
        ULONG32 TimeDateStamp;
    };
    ULONG64 Flags;
} MINIDUMP_HEADER, *PMINIDUMP_HEADER;

//
// The MINIDUMP_HEADER field StreamDirectoryRva points to 
// an array of MINIDUMP_DIRECTORY structures.
//

typedef struct _MINIDUMP_DIRECTORY
{
    ULONG32 StreamType;
    MINIDUMP_LOCATION_DESCRIPTOR Location;
} MINIDUMP_DIRECTORY, *PMINIDUMP_DIRECTORY;

typedef struct _MINIDUMP_STRING
{
    ULONG32 Length;         // Length in bytes of the string, string may have embedded NULL characters
    WCHAR   Buffer [0];     // Variable size buffer
} MINIDUMP_STRING, *PMINIDUMP_STRING;

//
// The MINIDUMP_DIRECTORY field StreamType may be one of the following types.
// Types will be added in the future, so if a program reading the cedump
// header encounters a stream type it does not understand it should ignore
// the data altogether. Any tag above LastReservedStream will not be used by
// the system and is reserved for program-specific information.
//

typedef enum _MINIDUMP_STREAM_TYPE {

    UnusedStream                = 0,

// 1 to 0x7FFF - Desktop
    ReservedStream0             = 1,
    ReservedStream1             = 2,
    ThreadListStream            = 3,
    ModuleListStream            = 4,
    MemoryListStream            = 5,
    ExceptionStream             = 6,
    SystemInfoStream            = 7,
    ThreadExListStream          = 8,
    Memory64ListStream          = 9,
    CommentStreamA              = 10,
    CommentStreamW              = 11,
    HandleDataStream            = 12,
    FunctionTableStream         = 13,
    UnloadedModuleListStream    = 14,
    MiscInfoStream              = 15,
    MemoryInfoListStream        = 16,
    ThreadInfoListStream        = 17,

// 0x8000 to 0xFFFE - Windows CE stream types
    ceStreamNull                = 0x8000,
    ceStreamSystemInfo          = 0x8001,
    ceStreamException           = 0x8002,
    ceStreamModuleList          = 0x8003,
    ceStreamProcessList         = 0x8004,
    ceStreamThreadList          = 0x8005, 
    ceStreamThreadContextList   = 0x8006,
    ceStreamThreadCallStackList = 0x8007,
    ceStreamMemoryVirtualList   = 0x8008,
    ceStreamMemoryPhysicalList  = 0x8009,
    ceStreamBucketParameters    = 0x800A,     

    LastReservedStream          = 0xffff

} MINIDUMP_STREAM_TYPE;

#else // UNDER_CE

typedef struct tagPLATFORMVERSION 
{
    DWORD dwMajor;
    DWORD dwMinor;
} PLATFORMVERSION, *PPLATFORMVERSION;

#endif // UNDER_CE

//
// STREAM TYPE: ceStreamSystemInfo
//
// The cedump system information contains processor and
// Operating System specific information.
// 

typedef struct _CEDUMP_SYSTEM_INFO
{
    USHORT  SizeOfHeader;           // Size of _CEDUMP_SYSTEM_INFO
    USHORT  ProcessorArchitecture;  // wProcessorArchitecture member of SYSTEM_INFO
    ULONG32 NumberOfProcessors;     // dwNumberOfProcessors   member of SYSTEM_INFO
    ULONG32 ProcessorType;          // dwProcessorType        member of SYSTEM_INFO
    USHORT  ProcessorLevel;         // wProcessorLevel        member of SYSTEM_INFO
    USHORT  ProcessorRevision;      // wProcessorRevision     member of SYSTEM_INFO
    ULONG32 ProcessorFamily;        // Defined by PROCESSOR_FAMILY_XX in OsAxsFlexi.h 
    ULONG32 MajorVersion;           // dwMajorVersion member of OSVERSIONINFO
    ULONG32 MinorVersion;           // dwMinorVersion member of OSVERSIONINFO
    ULONG32 BuildNumber;            // dwBuildNumber  member of OSVERSIONINFO
    ULONG32 PlatformId;             // dwPlatformId   member of OSVERSIONINFO
    ULONG32 LCID;                   // Locale ID
    RVA     OEMStringRva;           // OEM string, may have embedded NULL characters
    ULONG32 SupportFlags;           // Reserved for future use
    USHORT  Machine;                // Machine member of IMAGE_FILE_HEADER
    USHORT  __unusedAlignment;
    RVA     PlatformTypeRva;        // Platform type string, may have embedded NULL characters
    DWORD   Platforms;              // Number of PLATFORMVERSION
    RVA     PlatformVersion;        // Pointer to array of PLATFORMVERSION * Platforms 
} CEDUMP_SYSTEM_INFO, *PCEDUMP_SYSTEM_INFO;

//
// STREAM TYPE: ceStreamModuleList & ceStreamProcessList & 
//              ceStreamThreadList & ceStreamThreadContextList.
//
// These streams are made up of _CEDUMP_ELEMENT_LIST followed 
// by (NumberOfFieldInfo * _CEDUMP_FIELD_INFO)
//

//
// _CEDUMP_FIELD_INFO is used to describe fields associated 
// with Modules, Processes, Threads, and Context records.
//
// The size of each data element (e.g. Module, Process, etc.) will be the sum of the 
// FieldSizes.
//
// FieldId is defined in OsAxsFlexi.h
// FieldSize is the size of the data for each field
// FieldLabel is the label for the field (Typically used for column headers)
// FieldFormat is a standard printf format specification, with the following additions:
//
//      -%T{N=BitFieldNameN, M=BitFieldNameM...} for bitfield description 
//          where bitnumbers (N and M) are in [0..63] and BitFieldNameN
//          and BitFieldNameM are strings of char with no ","
//          if bitnumber in [0..31], the BitfieldName will be display
//          for bitnumber == 1
//          if bitnumber in [32..63], the BitfieldName will be display
//          for bitnumber == 0 (Inverse logic)
//          Any non described bit will be ignored
//          Will display all set bitfield separated by a ,
//    
//      -%N{N=EnumNameN, M=EnumNameM...} for enumeration description
//          where N and M are decimal DWORD value and EnumNameN
//          and EnumNameM are strings of char with no ","
//          Any non described enum value should be ignored
//
//      -%U for a 128bit GUID/UUID/IID.
//
// The ceStreamThreadContextList uses some predefined Context FieldIDs that
// are defined in OsAxsFlexi.h, these do not have an associated FieldFormat (blank string).
// Additional registers can be defined in addition to the predefined ones, these will use
// the FieldFormat for display purposes.
//

typedef struct _CEDUMP_FIELD_INFO
{
    ULONG32 FieldId;
    ULONG32 FieldSize;
    RVA     FieldLabel;
    RVA     FieldFormat;
} CEDUMP_FIELD_INFO, *PCEDUMP_FIELD_INFO;   

typedef struct _CEDUMP_ELEMENT_LIST
{
    USHORT  SizeOfHeader;
    USHORT  SizeOfFieldInfo;     // Size of _CEDUMP_FIELD_INFO
    ULONG32 NumberOfFieldInfo;   // Number of _CEDUMP_FIELD_INFO
    ULONG32 NumberOfElements;    
    RVA     Elements;            // Pointer to array of data elements  
} CEDUMP_ELEMENT_LIST, *PCEDUMP_ELEMENT_LIST;

//
// STREAM TYPE: ceStreamThreadCallStackList.
//

//  Bit fields for Flags:
#define CEDUMP_CALL_STACK_ERROR_DETECTED  0x01  // Bit 0 = Error Detected during unwind

typedef struct _CEDUMP_THREAD_CALL_STACK_FRAME
{
    ULONG32 ReturnAddr;
    ULONG32 FramePtr;
    ULONG32 ProcessId;
    ULONG32 Flags;                  
    ULONG32 Params[4];
} CEDUMP_THREAD_CALL_STACK_FRAME, *PCEDUMP_THREAD_CALL_STACK_FRAME;

// 
// The thread call stack is a header for stack frames.
//

typedef struct _CEDUMP_THREAD_CALL_STACK
{
    ULONG32 ProcessId;
    ULONG32 ThreadId;
    USHORT  SizeOfFrame;        // Size of _CEDUMP_THREAD_CALL_STACK_FRAME
    USHORT  NumberOfFrames;     // Number of _CEDUMP_THREAD_CALL_STACK_FRAME
    RVA     StackFrames;        // Pointer to array of _CEDUMP_THREAD_CALL_STACK_FRAME * NumberOfCalls
} CEDUMP_THREAD_CALL_STACK, *PCEDUMP_THREAD_CALL_STACK;

//
// The thread call stack list is a header for thread call stacks.
// It must be followed by (NumberOfEntries * _CEDUMP_THREAD_CALL_STACK)
//

typedef struct _CEDUMP_THREAD_CALL_STACK_LIST
{
    USHORT  SizeOfHeader;
    USHORT  SizeOfEntry;        // Size of _CEDUMP_THREAD_CALL_STACK
    ULONG32 NumberOfEntries;    // Number of _CEDUMP_THREAD_CALL_STACK
} CEDUMP_THREAD_CALL_STACK_LIST, *PCEDUMP_THREAD_CALL_STACK_LIST;

//
// STREAM TYPE: ceStreamException.
//

#define EXCEPTION_MAXIMUM_PARAMETERS 15 // maximum number of exception parameters

typedef struct _CEDUMP_EXCEPTION
{
    ULONG32 ExceptionCode;
    ULONG32 ExceptionFlags;
    ULONG32 ExceptionRecord;
    ULONG32 ExceptionAddress;
    ULONG32 NumberParameters;
    ULONG32 ExceptionInformation [ EXCEPTION_MAXIMUM_PARAMETERS ];
} CEDUMP_EXCEPTION, *PCEDUMP_EXCEPTION;

//
// The exception information stream must be followed by the
// exception record (1 * _CEDUMP_EXCEPTION) and the context of the
// thread (1 * _CEDUMP_ELEMENT_LIST), there may be additional thread 
// info structs that follow that (NumberOfFieldInfo * _CEDUMP_FIELD_INFO).
//

//  Bit fields for Flags:
#define CEDUMP_EXCEPTION_FIRST_CHANCE          0x01  // Bit 0 = First Chance Exception
#define CEDUMP_EXCEPTION_CAPTURE_DUMP_FILE_API 0x02  // Bit 1 = CaptureDumpFileOnDevice API called
#define CEDUMP_EXCEPTION_REPORT_FAULT_API      0x04  // Bit 2 = ReportFault API called
#define CEDUMP_EXCEPTION_MATCHES_FOCUS_THREAD  0x08  // Bit 3 = Exception Context matches focus thread

typedef struct _CEDUMP_EXCEPTION_STREAM
{
    USHORT  SizeOfHeader;
    USHORT  SizeOfException;        // Size of _CEDUMP_EXCEPTION
    USHORT  SizeOfThreadContext;    // Size of _CEDUMP_ELEMENT_LIST
    USHORT  Flags;                  
    ULONG32 CurrentProcessId;       // Current process for ThreadId
    ULONG32 ThreadId;               // Thread ID where exception occurred, or focus of CaptureDumpFileOnDevice
    ULONG32 OwnerProcessId;         // Owner process for ThreadId
    ULONG32 CaptureAPICurrentProcessId; // Current process for CaptureAPIThreadId
    ULONG32 CaptureAPIThreadId;         // Thread ID which made the call to CaptureDumpFileOnDevice
    ULONG32 CaptureAPIOwnerProcessId;   // Owner process for CaptureAPIThreadId
} CEDUMP_EXCEPTION_STREAM, *PCEDUMP_EXCEPTION_STREAM;

//
// STREAM TYPE: ceStreamBucketParameters.
//

typedef struct _CEDUMP_BUCKET_PARAMETERS
{
    USHORT  SizeOfHeader;
    USHORT  __unusedAlignment;
    RVA     EventType;          
    ULONG32 fDebug;             // 1 if debug build of application
    RVA     AppName;            // Name of crashing application
    ULONG32 AppStamp;           // Time Stamp of crashing application
    ULONG32 AppVerMS;           // File version of crashing application
    ULONG32 AppVerLS;
    RVA     ModName;            // Name of crashing module
    ULONG32 ModStamp;           // Time Stamp of crashing module
    ULONG32 ModVerMS;           // File version of crashing module
    ULONG32 ModVerLS;
    ULONG32 Offset;             // Offset where crash occured
    RVA     OwnerName;          // Name of application that owns crashing thread
    ULONG32 OwnerStamp;         // Time Stamp of owner application
    ULONG32 OwnerVerMS;         // File version of owner application
    ULONG32 OwnerVerLS;
} CEDUMP_BUCKET_PARAMETERS, *PCEDUMP_BUCKET_PARAMETERS;

//
// STREAM TYPE: ceStreamMemoryVirtualList & ceStreamMemoryPhysicalList
//
// Memory Ranges
//
// The Memory List header must be followed 
// by (NumberOfEntries * _MINIDUMP_MEMORY_DESCRIPTOR)
//


typedef struct _CEDUMP_MEMORY_LIST
{
    USHORT  SizeOfHeader;
    USHORT  SizeOfEntry;
    ULONG32 NumberOfEntries;    // Number of _MINIDUMP_MEMORY_DESCRIPTOR
} CEDUMP_MEMORY_LIST, *PCEDUMP_MEMORY_LIST;

#include <poppack.h>

