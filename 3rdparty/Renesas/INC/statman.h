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
//
// Statman header file.
//    Contains  everything, probably will split this up in the future.
//    
//
#ifndef STATMAN_H
#define  STATMAN_H


#if 0 

// Currently Stats can be dumped using statmanctl.
// Look in oak/utils/statmanctl.



// Code Instrumentation examples:

// 1) Legacy Statistics.
// Description:
//  These are statistics that already exist in your code.  You want to export
//  this memory to statman so statmanctl(or any other stat pickup engine) can
//  dump then in a manner consistent with your new stats.  Remember you are
//  giving statman a memory location which it will query when displaying
//  statistics.



//  Usage:
//      /*Definition of old statistic*/
//     static int my_legacy_stat;
//    ....
//  Startup()
//  {
//      ...
//      /*Register statistic with statman*/
//      CTERegisterLocalStat(L"Legacy Stat #1",&my_legacy_stat)
//      ...
//  }
//  Shutdown()
//  {
//      ...
//      /*Data going away deregister statistic with statman*/
//      CTEDeregisterLocalStat(&my_legacy_stat)
//      ...
//  }



// 2)   Lazy Stats (** This is going to change.)
// Description: You want to pepper your code with statistics that don't take
//  away from code clarity. Have little perf impact and can tell you some
//  useful information, this is the mechanicism for you. You don't need to add
//  globals, 
//    


//Usage:
//  You need to  add your statistic  to the end of  STATMAN_SCALAR_TYPE  enum.
//  Then you add a string for your  statistic in the same slot to  STATMAN_SCALAR_NAME.
//  Assume You called your stat STATMAN_MY_MODULE_STAT4. You can use your stats by doing:  

// SomeCodeThatWantsAStat()
// {
//  ...
//   /*Inc my stat.*/
//   CTEStatInc(STATMAN_MY_MODULE_STAT4);
//  ...
//}

#endif 
#ifdef __cplusplus 
extern "C" { 
#endif 
#include <cxport.h>

// Add your constants here.
enum STATMAN_SCALAR_TYPE 
{

    STATMAN_NDIS_INDICATE_RX,
    STATMAN_NDIS_INDICATE_RX_PACKET,
    STATMAN_NDIS_BLOCK_LOCKED_LOOPED_EQ_0,
    STATMAN_NDIS_BLOCK_LOCKED_LOOPED_EQ_1,
    STATMAN_NDIS_BLOCK_LOCKED_LOOPED_GTE_2,
    STATMAN_NDIS_BLOCK_LOCKED_NO_CONTENTION,
    STATMAN_NDIS_LOCK_MINIPORT_OWN_CS_ALREADY,
    STATMAN_NDIS_MINIPORT_SEND_RETURNED_RESOURCES,
    STATMAN_NDIS_SERIALIZED_SEND_LOCK_MINIPORT_FAILED,
    STATMAN_A_DLL_STAT1,

    /*Always last entry*/
    STATMAN_SCALAR_TYPE_MAX // Must be the last item.
};

// In the same order put your name here.
// XXX: do some clever include it twice macro trick to read these values out.
const static WCHAR* STATMAN_SCALAR_NAME[] = 
{
    L"Indicate Receive",
    L"Indicate Receive Packet",
    L"Blocked Locked Spin == 0",
    L"Blocked Locked Spin == 1",
    L"Blocked Locked Spin >= 2",
    L"Blocked Locked No Contention",
    L"LOCK_MINIPORT Already owns CS",
    L"MiniportSend  Returned Status Resources",
    L"Ndis Send Failed To Lock Miniport For Serialized Access",
    L"DLL A",
    /*Always last entry*/
    L"MAX_STAT"
};
// XXX: Must  be  >= STATMAN_SCALAR_TYPE_MAX , but this lets us get away with not rebuilding statman each time we add a new scalar. 
#define MAX_STATS 1000 

// You should not access the following member directly.
#ifndef BUILD_STATLIB // should only be dllimport if building the netstat lib.
__declspec(dllimport)
#endif
    extern DWORD g_CTEStatTable[MAX_STATS];


////////////////////////////////////////////////////////////////////////////////
// The functions are used when you want to gather legacy/local stats.
// You pass in  a pointer to your data,  and you update the data as you choose.
////////////////////////////////////////////////////////////////////////////////
void CTERegisterLocalStat(WCHAR * statDesc, DWORD * pStat);


// When your stat is nolonger valid, you need to de-register.
// Only give me the same pointer.
void CTEDeregisterLocalStat(DWORD * pStat);

////////////////////////////////////////////////////////////////////////////////
//  The following 3 functions all for stat gathering with minimal code changes.
//  You add  a new value to the STATMAN_SCALAR_NAME enum, and use that to 
//  inc, stat and  clear.
////////////////////////////////////////////////////////////////////////////////
void inline CTEStatInc(const DWORD stat)
{
    g_CTEStatTable[stat]++;
    return;
}
void inline CTEStatSet(const DWORD stat, DWORD val)
{
    g_CTEStatTable[stat]=val;
    return;
}
void inline CTEStatClear(const DWORD stat)
{
    g_CTEStatTable[stat]=0;
    return;
}


//
//  Define our IOCTL Interface.
//

// statman gets IOCTLS from 0 - 1000

enum NESTAT_IOCTL 
{
    // Dump all stats to debug out.
    IOCTL_STATMAN_DUMP_STAT_DEBUG =  CXPORT_IOCTL_STATMAN_BASE ,
    IOCTL_STATMAN_GET_STAT_SIZE ,
    IOCTL_STATMAN_GET_STAT_NAME_SIZE ,
    IOCTL_STATMAN_GET_STAT_DATA,
    IOCTL_STATMAN_GET_STAT_NAME_DATA,

    IOCTL_STATMAN_LAST_STAT // Dummy IOCTL.
};


#ifdef __cplusplus 
}
#endif 

#endif // ifdef STATMAN_H
