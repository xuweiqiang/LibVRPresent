//********************************************************************
//Renesas audio output unit
//		by Benson Chuang
//			benson.chuang.tw@gmail.com  benson.chuang@renesas.com
//********************************************************************
//Component:	libvo
//						Renesas Technology Taiwan/Engineerning Center
//********************************************************************

#ifndef _VEU_SET_H
#define _VEU_SET_H

//**************************************
// Definition of VEU processing structure
//**************************************
typedef struct s_uge_draw_ex{
	int x;
	int y;
	int xoffset;
	int yoffset;
	int width_mem;
	int width;
	int height;
	int format;
	unsigned char *buff1;
	unsigned char *buff2;
} S_UGE_DRAW_EX;

#endif