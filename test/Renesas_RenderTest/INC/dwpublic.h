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
#pragma once

#if _WIN32_WCE < 500
#define WATSON_EXCEPTION_EVENT   L"WinCe420Exception"
#else
#ifdef WATSON_BINARY_DUMP
#define WATSON_EXCEPTION_EVENT   L"WinCE501Exception"
#define WATSON_SNAPSHOT_EVENT    L"WinCE501SnapShot"
#define WATSON_HOSTCAPTURE_EVENT L"WinCE501HostCapture"
#else
#define WATSON_EXCEPTION_EVENT   L"WinCe500Exception"
#endif // WATSON_BINARY_DUMP
#endif // _WIN32_WCE < 500

#define WATSON_CONTEXT_DUMPFILE_SIZE  (62 * 1024)
#define WATSON_REGISTRY_SETTINGS_SIZE (2 * 1024)
#define WATSON_MINIMUM_RESERVED_MEMORY (WATSON_CONTEXT_DUMPFILE_SIZE + WATSON_REGISTRY_SETTINGS_SIZE)
#define WATSON_MINIMUM_RESERVED_MEMORY_RECOMMENDED (128 * 1024)

// Folder attributes used by Watson folders 
#define WATSON_FOLDER_ATTRIBUTES    (FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_HIDDEN)

// Watson Registry settings for Report Upload
#define WATSON_REGKEY_UPLOAD_SETTINGS                       L"System\\ErrorReporting\\UploadSettings"
#define WATSON_REGVALUE_NO_CONSENT_REQUIRED                 L"NoConsentRequired"
#define WATSON_REGVALUE_NO_CONSENT_REQUIRED_DEFAULT_DATA    0
#define WATSON_REGVALUE_DONT_UPLOAD                         L"DontUpload"
#define WATSON_REGVALUE_DONT_UPLOAD_DEFAULT_DATA            0
#define WATSON_REGVALUE_MAX_DAILY_REPORTS                   L"MaxDailyReports"
#define WATSON_REGVALUE_MAX_DAILY_REPORTS_DEFAULT_DATA      5
#define WATSON_REGVALUE_MAX_WEEKLY_REPORTS                  L"MaxWeeklyReports"
#define WATSON_REGVALUE_MAX_WEEKLY_REPORTS_DEFAULT_DATA     5
#define WATSON_REGVALUE_UPLOAD_SUCCEEDED_DLG                L"UploadSucceededDlg"
#define WATSON_REGVALUE_UPLOAD_SUCCEEDED_DLG_DEFAULT_DATA   0
#define WATSON_REGVALUE_UPLOAD_FAILED_DLG                   L"UploadFailedDlg"
#define WATSON_REGVALUE_UPLOAD_FAILED_DLG_DEFAULT_DATA      0
#define WATSON_REGVALUE_UPLOAD_RETRY_INTERVAL               L"UploadRetryInterval"
#define WATSON_REGVALUE_UPLOAD_RETRY_INTERVAL_DEFAULT_DATA  (60 * 24)   // 24 hours(in minutes)

// Watson Registry settings for Dump Files attributes
#define WATSON_REGKEY_DUMP_FILES                            L"System\\ErrorReporting\\DumpFiles"

// Flags specific to each dump file
#define DUMP_FLAG_USERCONSENTED          0x00000001  // Indicates user consented to upload
#define DUMP_FLAG_UPLOADFAILED           0x00000002  // Last upload attempt failed
#define DUMP_FLAG_DONTDELETE             0x00000004  // Don't delete
#define DUMP_FLAG_CONSENTFAIL            0x00000008  // Negative consent (user doesn't want to send)
#define DUMP_FLAG_MANIFEST               0x00000010  // Manifest mode (no mini-dump file)
#define DUMP_FLAG_UPLOADED               0x00000020  // Indicates it has already been processed/uploaded
#define DUMP_FLAG_CACHEDLOG              0x00000040  // Indicates the log files have already been cached
#define DUMP_FLAG_FIRST_STAGE_SUCCEEDED  0x00000080  // Indicates first stage succeeded during last upload attempt

// Watson Registry settings for Creation of Dump Files
#define WATSON_REGKEY_DUMP_SETTINGS                         L"System\\ErrorReporting\\DumpSettings"
#define WATSON_REGVALUE_DUMP_DIRECTORY                      L"DumpDirectory"
#define WATSON_REGVALUE_DUMP_DIRECTORY_DEFAULT_DATA         L"\\Windows\\DumpFiles"
#define WATSON_REGVALUE_EXTRA_FILES_DIRECTORY               L"ExtraFilesDirectory"
#define WATSON_REGVALUE_EXTRA_FILES_DIRECTORY_DEFAULT_DATA  L"\\Windows\\ExtraDumpFiles"
#define WATSON_REGVALUE_CAB_DIRECTORY                       L"CabDirectory"
#define WATSON_REGVALUE_CAB_DIRECTORY_DEFAULT_DATA          L"\\Windows\\DumpFiles\\CabFiles"
#define WATSON_CAB_FILE_NAME                                L"DwCab.Cab"
#define WATSON_REGVALUE_UPLOAD_CLIENT                       L"UploadClient"
#define WATSON_REGVALUE_UPLOAD_CLIENT_DEFAULT_DATA          L"Dw.Exe"
#define WATSON_REGVALUE_MAX_DISK_USAGE                      L"MaxDiskUsage"
#ifdef WATSON_BINARY_DUMP
#define WATSON_REGVALUE_MAX_DISK_USAGE_DEFAULT_DATA         (4*DrWatsonGetSize())
#else
#define WATSON_REGVALUE_MAX_DISK_USAGE_DEFAULT_DATA         (128*1024*4)
#endif
#define WATSON_REGVALUE_MAX_LOG_FILES                       L"MaxLogFiles"
#define WATSON_REGVALUE_MAX_LOG_FILES_DEFAULT_DATA          3
#define WATSON_REGVALUE_MAX_LOG_FILE_SIZE                   L"MaxLogFileSize"
#define WATSON_REGVALUE_MAX_LOG_FILE_SIZE_DEFAULT_DATA      (100*1024)
#define WATSON_REGVALUE_DUMP_ENABLED                        L"DumpEnabled"
#define WATSON_REGVALUE_DUMP_ENABLED_DEFAULT_DATA           1
#ifdef WATSON_BINARY_DUMP
// Advanced binary dump settings, will not apear in registry by default
#define WATSON_REGVALUE_DUMP_TYPE                           L"DumpType"
#define WATSON_REGVALUE_DUMP_TYPE_DEFAULT_DATA              2   // 1 = Context, 2 = System, 3 = Complete
#define WATSON_REGVALUE_DUMP_FIRST_CHANCE                   L"DumpFirstChance"
#define WATSON_REGVALUE_DUMP_FIRST_CHANCE_DEFAULT_DATA      0   // 0 = Capture 2nd Chance, 1 = Capture 1st Chance
#define WATSON_REGVALUE_MAX_DUMP_SIZE                       L"MaxDumpSize"
#define WATSON_REGVALUE_MAX_DUMP_SIZE_DEFAULT_DATA          (DrWatsonGetSize() - WATSON_REGISTRY_SETTINGS_SIZE)
#endif

// Watson defines for dump file creation
#define WATSON_DUMP_FOLDER_NAME_FORMAT                      L"Ce%02u%02u%02u-%02u"
#ifdef WATSON_BINARY_DUMP
#define WATSON_DUMP_FILE_NAME_FORMAT                        L"Ce%02u%02u%02u-%02u.kdmp"
#else
#define WATSON_DUMP_FILE_NAME_FORMAT                        L"Ce%02u%02u%02u-%02u.udmp"
#endif
#define WATSON_MAX_DUMP_FILES_PER_DAY                       99

#define WATSON_MUTEX_DUMP_FILES                             L"WatsonMutexDumpFiles"
#define WATSON_TIMEOUT_MUTEX_DUMP_FILES                     (1000*60*5)

// Watson Registry settings for Upload Status
#define WATSON_REGKEY_UPLOAD_STATUS                         L"System\\ErrorReporting\\UploadStatus"
#define WATSON_REGVALUE_LAST_UPLOAD_SUCCEEDED               L"LastUploadSucceeded"
#define WATSON_REGVALUE_LAST_UPLOAD_STATUS                  L"LastUploadStatus"
#define WATSON_REGVALUE_LAST_UPLOAD_BUCKET_TABLE            L"LastUploadBucketTable"
#define WATSON_REGVALUE_LAST_UPLOAD_BUCKET_ID               L"LastUploadBucketID"
#define WATSON_REGVALUE_LAST_UPLOAD_FILE                    L"LastUploadFileName"

#define DEFAULT_UPLOAD_SERVER  "watson.microsoft.com"

// Accept response URL from <foo>.microsoft.com, <foo>.msn.com, or microsoft.com
#define VALID_RESPONSE_SERVER1 ".microsoft.com"
#define VALID_RESPONSE_SERVER2 ".msn.com"
#define VALID_RESPONSE_SERVER3 "microsoft.com"

#define DW_PRIVACY_POLICY_URL  "http://watson.microsoft.com/dw/dcp.asp"

#define WATSON_EVENT_DUMP_FILE_READY     L"WatsonEventDumpFileReady"
#define WATSON_EVENT_UPLOAD_CLIENT_READY L"WatsonUploadClientReady"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)   (sizeof(a)/sizeof(a[0]))
#endif

#define DwAlloc(x) malloc(x)
#define DwReAlloc(ptr, size) realloc(ptr, size)
#define DwFree(x) free(x)

#define DwSafeFree(x) if ((x) != NULL) DwFree(x);
#define DwSafeFreeAndNull(x) if ((x) != NULL) { DwFree(x); x = NULL; }

#define FStringSet(x) (x != NULL && x[0] != 0)

