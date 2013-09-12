//********************************************************************
//Message Log
//		by Benson Chuang
//			benson.chuang.tw@gmail.com
//********************************************************************

#ifndef _LIBMSG_H
#define _LIBMSG_H


//**************************************
// Definition of behavior
//**************************************
#define DEF_MSG_LOG_HIGH	0
#define DEF_MSG_LOG_MID		1
#define DEF_MSG_LOG_LOW		2

#define DEF_MSG_LOG_CURR	DEF_MSG_LOG_HIGH
//**************************************
// Definition of message log Function
//**************************************
void _msglog(unsigned long pri,const short* filename,int* linenum,wchar_t* txt,...);

//**************************************
// MACRO Definition
//**************************************
#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)

#define debug_msg(PRI,...) 	_msglog(PRI,__WFILE__,(int*)__LINE__,__VA_ARGS__)
#define mlog_assert(CONDITION, TXT)	CONDITION?((void)0):_msglog(DEF_MSG_LOG_HIGH,__FILE__,__LINE__,TXT)

#endif //LIBMSG_H