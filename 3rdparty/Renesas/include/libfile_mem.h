//********************************************************************
//Renesas File mapping memory I/O
//		by Benson Chuang
//			benson.chuang.tw@gmail.com  benson.chuang@renesas.com
//********************************************************************
//Component:	File_MEM
//						Renesas Technology Taiwan/Engineerning Center
//Warning: Ths component support re-entry, global variables are prohibited.
//********************************************************************
#ifndef _FILEMEM_H
#define _FILEMEM_H

//**************************************
// Include file dependencies
//**************************************
#include <winbase.h>

//**************************************
// Operation type
//**************************************
enum FILEMEM_TYPE{
	FILEMEM_TYPE_FILEIO = 0,
	FILEMEM_TYPE_MEMIO,
};
//**************************************
// Structure Definition
//**************************************
typedef struct filemem_info
{	
	HANDLE	hHandle;
	HANDLE	hMappingHandle;
	HANDLE	hIoHandle;
	LPCTSTR lpFileName;
	DWORD	dwDesiredAccess;
	DWORD	dwWindowsSize;
	FILEMEM_TYPE	operation_type;
	LPVOID	pMem;
	unsigned long	offset;
	//File information
	unsigned long	file_length;
	bool	eof;
}FILEMEM_INFO;

//**************************************
//Public Function
//**************************************
void FILEMEM_init(FILEMEM_INFO *info);
void FILEMEM_deinit(FILEMEM_INFO *info);
unsigned long FILEMEM_read(FILEMEM_INFO *info, char* des_addr , unsigned long num);
void FILEMEM_write(FILEMEM_INFO *info, char* des_addr , unsigned long num);
void FILEMEM_seek(FILEMEM_INFO *info , unsigned long offset);

#endif //_FILEMEM_H