#pragma once


typedef struct sMod_LOOP_
{
	DEVMODE dispset;
	struct sMod_LOOP_* next_mode;
}sMod_LOOP;

typedef struct sRESOLUTION_LIST_
{
	DWORD devNum;
	DISPLAY_DEVICE sDev;
	sMod_LOOP* pDispSet_loophead; 
	struct sRESOLUTION_LIST_* next_dev;	
}sRESOLUTION_LIST;

typedef enum
{
 	RANDOM,
 	STANDARD,
}eRES_CHANGE_TYPE;

typedef struct
{
	DWORD devNum;
	DEVMODE dispset;
}sBACKUP_RES;
#include "resource.h"
