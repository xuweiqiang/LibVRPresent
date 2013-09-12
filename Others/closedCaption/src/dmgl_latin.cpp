#include "dmgl_latin.h"
#include "dmgl_jis.h"
#include "cc_1seg.h"


static GSetName_Latin FinalByteSetting_Latin(int final_byte)
{
	switch(final_byte)
	{
		case 0x4A:
		return ALPHANUMERIC_LATIN;

		case 0x4B:
		return LATIN_EXTENSION;		

		case 0x4C:
		return SPECIAL_CHARACTERS_SET;
	}
	return LATIN_EXTENSION;
}

/*
unsigned char SpecialCodeMap[96] = {
	0x00, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
*/
static unsigned char SpecialCodeMap[96] = {
	0x00,    1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   2,    3,    4,    5,    6,    7,    8,    9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  10,   11,   12,   13,   14,   15,   16,   17,   18,   19,   20,   21, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//static unsigned short SpecialCodeMap_UTF16[96] = {
//	0x0000, 0x266a, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
//	0x00a4, 0x00a6, 0x00a8, 0x00b4, 0x00b8, 0x00bc, 0x00bd, 0x00be, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
//	0x2026, 0x2588, 0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2122, 0x215b, 0x215c, 0x215d, 0x215e, 0x0000, 0x0000, 0x0000, 0x0000,
//	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
//	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
//	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
//};

static int LatinSpecialCodeIndex(unsigned char code, unsigned char *index)
{
	if(code<0x20 || code >0x7F)
		return 0;
	
	if(SpecialCodeMap[code-0x20]==0)
		return 0;	

	*index = SpecialCodeMap[code-0x20]-1;

	return 1;
}


/*
The default setting
G0 ALPHANUMERIC_LATIN
G1 ALPHANUMERIC_LATIN
G2 LATIN_EXTENSION
G3 MSPECIAL_CHARACTERS_SET

GL=>(LS0)  G0
GR=>(LS2R) G2 
*/
int CharacterDecode_Latin(unsigned int *pDst, int dst_length, unsigned char* pSrc, int src_length, GSetName_Latin gl, GSetName_Latin gr, void *param) 
{	
	unsigned char *pByte = pSrc;
	unsigned char *pLastByte = pByte+src_length;
	int code;
	int out_len=0;	
	
	int G_sets[4];
	int GL_area, GR_area;
	
	//use this flag to record the shift	
	int Before_Shift_set;
	int Single_Shift_Flag = 0;
	int MidSize = 0;

	//DRCS default: DRCS_1, 1 byte code
	int DRCS_Set = 0x41;
	int DRCS_bytes = 1;
	
	GL_area = gl;
	GR_area = gr;

	//default setting
	G_sets[0] = ALPHANUMERIC_LATIN;
	G_sets[1] = ALPHANUMERIC_LATIN;
	G_sets[2] = LATIN_EXTENSION;
	G_sets[3] = SPECIAL_CHARACTERS_SET;	

	/*
	Invocation:
	GL LS0 (G0)
	GR LS2R (G2)
	*/


	while(pByte<pLastByte) {	
		if(pByte[0]<0x20) //C0
		{			
			if(pByte[0]==ESC)
			{
				int final_byte;
				pByte++;
				switch(pByte[0])
				{
					////////////////////////////////////////////////////////////////
					//invokation of code element
					//LS2
					case 0x6E:
						GL_area = G_sets[2];
						pByte++;						
					break;
					//LS3
					case 0x6F:
						GL_area = G_sets[3];
						pByte++;
						break;
					//LS1R
					case 0x7E:
						GR_area = G_sets[1];
						pByte++;
						break;
					//LS2R
					case 0x7D:
						GR_area = G_sets[2];
						pByte++;
						break;
					//LS3R
					case 0x7C:
						GR_area = G_sets[3];
						pByte++;
						break;
					//end invokation of code element
					////////////////////////////////////////////////////////////////
					case 0x28:
						code = pByte[0];
						//get the final byte
						if(pByte[1]==0x20)
						{
							//1-byte DRCS							
							final_byte = pByte[2];
							G_sets[0] = FinalByteSetting_Latin(final_byte);
							pByte+=3;
						}
						else
						{
							//1-byte G set
							final_byte = pByte[1];
							G_sets[0] = FinalByteSetting_Latin(final_byte);
							pByte+=2;
						}
						break;
					case 0x29:
						code = pByte[0];
						
						//get the final byte
						if(pByte[1]==0x20)
						{
							//1-byte DRCS							
							final_byte = pByte[2];
							G_sets[1] = FinalByteSetting_Latin(final_byte);
							pByte+=3;
						}
						else
						{
							//1-byte G set
							final_byte = pByte[1];
							G_sets[1] = FinalByteSetting_Latin(final_byte);
							pByte+=2;
						}
						break;
					case 0x2A:
						code = pByte[0];
						
						//get the final byte
						if(pByte[1]==0x20)
						{
							//1-byte DRCS							
							final_byte = pByte[2];
							G_sets[2] = FinalByteSetting_Latin(final_byte);
							pByte+=3;
						}
						else
						{
							//1-byte G set
							final_byte = pByte[1];
							G_sets[2] = FinalByteSetting_Latin(final_byte);
							pByte+=2;
						}
						break;
					case 0x2B:
						code = pByte[0];
						
						//get the final byte
						if(pByte[1]==0x20)
						{
							//1-byte DRCS							
							final_byte = pByte[2];
							G_sets[3] = FinalByteSetting_Latin(final_byte);
							pByte+=3;
						}
						else
						{
							//1-byte G set
							final_byte = pByte[1];
							G_sets[3] = FinalByteSetting_Latin(final_byte);
							pByte+=2;
						}
						break;
					case 0x24:						
						pByte++;
						code = pByte[0];
						if(pByte[1]==0x20)
						{
							//2-byte DRCS
							switch(code)
							{							
								case 0x28:
									final_byte = pByte[2];							
									G_sets[0] = FinalByteSetting_Latin(final_byte);
									pByte+=3;
									break;
								case 0x29:
									final_byte = pByte[2];							
									G_sets[1] = FinalByteSetting_Latin(final_byte);
									pByte+=3;
									break;
								case 0x2A:
									final_byte = pByte[2];							
									G_sets[2] = FinalByteSetting_Latin(final_byte);
									pByte+=3;
									break;
								case 0x2B:
									final_byte = pByte[2];							
									G_sets[3] = FinalByteSetting_Latin(final_byte);
									pByte+=3;
									break;
							}
						}
						else
						{
							//2-byte G set							
							switch(code)
							{							
								case 0x29:
									final_byte = pByte[1];									
									G_sets[1] = FinalByteSetting_Latin(final_byte);
									pByte+=2;
									break;
								case 0x2A:
									final_byte = pByte[1];									
									G_sets[2] = FinalByteSetting_Latin(final_byte);
									pByte+=2;
									break;
								case 0x2B:									
									final_byte = pByte[1];									
									G_sets[3] = FinalByteSetting_Latin(final_byte);
									pByte+=2;
									break;
								default:
									final_byte = code;
									G_sets[0] = FinalByteSetting_Latin(final_byte);
									pByte++;
									break;
							}							
						}						
						break;
				}//~switch(pByte[0])			
				
			} //~if(pByte[0]==ESC)			
			////////////////////////////////////////////////////////////////
			//invokation of code element
			else if(pByte[0]==LS0) //LS0
			{
				GL_area = G_sets[0];
				pByte++;
			}
			else if(pByte[0]==LS1) //LS1
			{
				GL_area = G_sets[1];
				pByte++;
			}
			else if(pByte[0]==SS2) //SS2
			{
				Single_Shift_Flag = 1;
				Before_Shift_set = GL_area;
				GL_area = G_sets[2];
				pByte++;
			}
			else if(pByte[0]==SS3) //SS3
			{
				Single_Shift_Flag = 1;
				Before_Shift_set = GL_area;
				GL_area = G_sets[3];
				pByte++;
			}			
			else if(pByte[0]==CS)
			{
				//clear
				pDst[out_len++] = I_CS;
				pByte++;
			}
			else if(pByte[0]==APR)
			{
				//change line			   	
				pDst[out_len++] = I_APR;
		   		pByte++;
			}
			//end invokation of code element
			////////////////////////////////////////////////////////////////
			else
				pByte++;
		}
		else if(pByte[0]>=0x20 && pByte[0]<0x80) //GL
		{
			if(pByte[0]==SP)
			{
				//space				
				pDst[out_len++] = I_SP;
				pByte++;
			}
			else if(pByte[0]==DEL)
			{
				pDst[out_len++] = I_DEL;
				pByte++;
			}
			else
			{
				if(GL_area==LATIN_EXTENSION)
				{
					code = pByte[0];
					pDst[out_len++] = code-0x20+96;					
					pByte++;
				}
				else if(GL_area==ALPHANUMERIC_LATIN)
				{		
					code = pByte[0];
					pDst[out_len++] = code-0x20;
					pByte++;				
				}
				else if(GL_area==SPECIAL_CHARACTERS_SET)
				{	
					unsigned char index;
					code = pByte[0];
					if(LatinSpecialCodeIndex(pByte[0], &index)>0)
					{
						pDst[out_len++] = index+96+96;
					}
					pByte++;
				}
				else if(GL_area == DRCS_FONT)
				{
					unsigned byte0;

					if(DRCS_bytes==1)
					{
						byte0 = pByte[0];						

						if(DRCS_Set==0x41) //1seg, DRCS1 only
						{
							//DRCS Font
							tDRCSdata *DSRC_1 = (tDRCSdata*)param;					 

							if(DSRC_1)
							{
								if(byte0>=0x21 && byte0<=0x7E)
								{
									byte0 -= 0x21;
									if(DSRC_1[byte0].m_used)
									{
										pDst[out_len++] = I_DRCS;
										pDst[out_len++] = DSRC_1[byte0].m_width; //width
										pDst[out_len++] = DSRC_1[byte0].m_height; //height
										pDst[out_len++] = (unsigned int)DSRC_1[byte0].m_data; //address
									}
								}
							}
						}
						pByte+=1;
					}
					else
					{
						//don't support 2-bytes DRCS now	
						pByte+=2;
					}
				}				
				else
					pByte++;
			}

			if(Single_Shift_Flag)
			{
				GL_area = Before_Shift_set;
				Single_Shift_Flag = 0;
			}
		}
		else if(pByte[0]>=0x80 && pByte[0]<0xA0) //C1
		{
			if(pByte[0]>=BKF && pByte[0]<=WHF)
			{
				if(pByte[0] == BKF)
		   		{
					pDst[out_len++] = I_BKF;
		   		}
		   		else if(pByte[0] == RDF)
		   		{
					pDst[out_len++] = I_RDF;
		   		}
		   		else if(pByte[0] == GRF)
		   		{
					pDst[out_len++] = I_GRF;
		   		}
		   		else if(pByte[0] == YLF)
		   		{
					pDst[out_len++] = I_YLF;
		   		}
		   		else if(pByte[0] == BLF)
		   		{
					pDst[out_len++] = I_BLF;
		   		}
		   		else if(pByte[0] == MGF)
		   		{
					pDst[out_len++] = I_MGF;
		   		}
		   		else if(pByte[0] == CNF)
		   		{
					pDst[out_len++] = I_CNF;
		   		}
		   		else if(pByte[0] == WHF)
		   		{
					pDst[out_len++] = I_WHF;
		   		}
				pByte++;
		   	}
			else if (pByte[0] == MSZ)
			{
				MidSize = 1;
				pDst[out_len++] = I_MSZ;
				pByte++;
			}
			else if (pByte[0] == NSZ)
			{
				MidSize = 0;
				pDst[out_len++] = I_NSZ;
				pByte++;
			}
			else if(pByte[0] == FLC)
		   	{
		   		if(*(pByte+1) == 0x40)
		   		{
					/*
					FlashArea = DM_TRUE;	
					GUI_Set_ClosedCaptionFlash();
					*/
					pDst[out_len++] = I_FLC_ON;
				}
				else if(*(pByte+1) == 0x4F)
				{
					/*
					FlashArea = DM_FALSE;
					*/
					pDst[out_len++] = I_FLC_OFF;
				}
		   		pByte+=2;
		   	}
	   		else if(pByte[0] == TIM)
	   		{
				//Please refer to "ARIB STD-B24 Volume1 Part2 Table7.13 C1 control character set".
				unsigned char TimeControlP1 = pByte[1];
				unsigned char TimeControlP2  = pByte[2];
					
				if((TimeControlP1 == 0x20) && ((TimeControlP2 >= 0x40)&&(TimeControlP2 <= 0x7F)))
				{
					//TimeControlPeriod  += ((*TimeControlP2)&0x3F);
					//GUI_Set_ClosedCaptionClearLater(TimeControlPeriod);
					pDst[out_len++] = I_TIM;
					pDst[out_len++] = TimeControlP2&0x3F;
			}	
	   			pByte+=3;
		   	}
			else
				pByte++;
		}
		else //GR
		{
			if(GR_area==LATIN_EXTENSION)
			{
				code = pByte[0];
				pDst[out_len++] = code-0xA0+96;					
				pByte++;
			}
			else if(GR_area==ALPHANUMERIC_LATIN)
			{
				code = pByte[0];
				pDst[out_len++] = code-0xA0;
				pByte++;				
			}
			else if(GR_area==SPECIAL_CHARACTERS_SET)
			{
				pByte++;
			}
			else
				pByte++;			
		}

		if(out_len >= dst_length)
			return out_len;
	}	
	return out_len;
}

const unsigned short LatinExt_utf16_tab[96] = {
0x0000, 0x00A1, 0x00A2, 0x00A3, 0x20AC, 0x00A5, 0x0160, 0x00A7, 0x0161, 0x00A9, 
0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF, 0x00B0, 0x00B1, 0x00B2, 0x00B3, 
0x017D, 0x00B5, 0x00B6, 0x00B7, 0x017E, 0x00B9, 0x00BA, 0x00BB, 0x0152, 0x0153, 
0x0178, 0x00BF, 0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7, 
0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF, 0x00D0, 0x00D1, 
0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7, 0x00D8, 0x00D9, 0x00DA, 0x00DB, 
0x00DC, 0x00DD, 0x00DE, 0x00DF, 0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 
0x00E6, 0x00E7, 0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF, 
0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00F7, 0x00F8, 0x00F9,
0x00FA, 0x00FB, 0x00FC, 0x00FD, 0x00FE, 0x0000
};

const unsigned short SpecialCharacter_utf16_tab[96] = {
//0x20
0x0000, 0x266A, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
//0x30
0x00A4, 0x00A6, 0x00A8, 0x00B4, 0x00B8, 0x00BC, 0x00BD, 0x00BE, 
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
//0x40
0x2026, 0x2588, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2122, 
0x215B, 0x215C, 0x215D, 0x215E, 0x0000, 0x0000, 0x0000, 0x0000,
//0x50
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
//0x60
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
//0x70
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};

int PORtoUTF16(unsigned short* unicode, int dst_length, unsigned char* pSrc, int src_length, GSetName_Latin gl, GSetName_Latin gr, void *param) 
{	
	unsigned char *pByte = pSrc;
	unsigned char *pLastByte = pByte+src_length;
	unsigned short* pDst = (unsigned short*)unicode;
	int code;
	int out_len=0;	
	
	int G_sets[4];
	int GL_area, GR_area;
	
	//use this flag to record the shift	
	int Before_Shift_set;
	int Single_Shift_Flag = 0;
//	int MidSize = 0;

	//DRCS default: DRCS_1, 1 byte code
//	int DRCS_Set = 0x41;
//	int DRCS_bytes = 1;
	
	GL_area = gl;
	GR_area = gr;

	//default setting
	G_sets[0] = ALPHANUMERIC_LATIN;
	G_sets[1] = ALPHANUMERIC_LATIN;
	G_sets[2] = LATIN_EXTENSION;
	G_sets[3] = SPECIAL_CHARACTERS_SET;	

	/*
	Invocation:
	GL LS0 (G0)
	GR LS2R (G2)
	*/


	while(pByte<pLastByte) {	
		if(pByte[0]<0x20) //C0
		{			
			if(pByte[0]==ESC)
			{
				int final_byte;
				pByte++;
				switch(pByte[0])
				{
					////////////////////////////////////////////////////////////////
					//invokation of code element
					//LS2
					case 0x6E:
						GL_area = G_sets[2];
						pByte++;						
					break;
					//LS3
					case 0x6F:
						GL_area = G_sets[3];
						pByte++;
						break;
					//LS1R
					case 0x7E:
						GR_area = G_sets[1];
						pByte++;
						break;
					//LS2R
					case 0x7D:
						GR_area = G_sets[2];
						pByte++;
						break;
					//LS3R
					case 0x7C:
						GR_area = G_sets[3];
						pByte++;
						break;
					//end invokation of code element
					////////////////////////////////////////////////////////////////
					case 0x28:
						code = pByte[0];
						//get the final byte
						if(pByte[1]==0x20)
						{
							//1-byte DRCS							
							final_byte = pByte[2];
							G_sets[0] = FinalByteSetting_Latin(final_byte);
							pByte+=3;
						}
						else
						{
							//1-byte G set
							final_byte = pByte[1];
							G_sets[0] = FinalByteSetting_Latin(final_byte);
							pByte+=2;
						}
						break;
					case 0x29:
						code = pByte[0];
						
						//get the final byte
						if(pByte[1]==0x20)
						{
							//1-byte DRCS							
							final_byte = pByte[2];
							G_sets[1] = FinalByteSetting_Latin(final_byte);
							pByte+=3;
						}
						else
						{
							//1-byte G set
							final_byte = pByte[1];
							G_sets[1] = FinalByteSetting_Latin(final_byte);
							pByte+=2;
						}
						break;
					case 0x2A:
						code = pByte[0];
						
						//get the final byte
						if(pByte[1]==0x20)
						{
							//1-byte DRCS							
							final_byte = pByte[2];
							G_sets[2] = FinalByteSetting_Latin(final_byte);
							pByte+=3;
						}
						else
						{
							//1-byte G set
							final_byte = pByte[1];
							G_sets[2] = FinalByteSetting_Latin(final_byte);
							pByte+=2;
						}
						break;
					case 0x2B:
						code = pByte[0];
						
						//get the final byte
						if(pByte[1]==0x20)
						{
							//1-byte DRCS							
							final_byte = pByte[2];
							G_sets[3] = FinalByteSetting_Latin(final_byte);
							pByte+=3;
						}
						else
						{
							//1-byte G set
							final_byte = pByte[1];
							G_sets[3] = FinalByteSetting_Latin(final_byte);
							pByte+=2;
						}
						break;
					case 0x24:						
						pByte++;
						code = pByte[0];
						if(pByte[1]==0x20)
						{
							//2-byte DRCS
							switch(code)
							{							
								case 0x28:
									final_byte = pByte[2];							
									G_sets[0] = FinalByteSetting_Latin(final_byte);
									pByte+=3;
									break;
								case 0x29:
									final_byte = pByte[2];							
									G_sets[1] = FinalByteSetting_Latin(final_byte);
									pByte+=3;
									break;
								case 0x2A:
									final_byte = pByte[2];							
									G_sets[2] = FinalByteSetting_Latin(final_byte);
									pByte+=3;
									break;
								case 0x2B:
									final_byte = pByte[2];							
									G_sets[3] = FinalByteSetting_Latin(final_byte);
									pByte+=3;
									break;
							}
						}
						else
						{
							//2-byte G set							
							switch(code)
							{							
								case 0x29:
									final_byte = pByte[1];									
									G_sets[1] = FinalByteSetting_Latin(final_byte);
									pByte+=2;
									break;
								case 0x2A:
									final_byte = pByte[1];									
									G_sets[2] = FinalByteSetting_Latin(final_byte);
									pByte+=2;
									break;
								case 0x2B:									
									final_byte = pByte[1];									
									G_sets[3] = FinalByteSetting_Latin(final_byte);
									pByte+=2;
									break;
								default:
									final_byte = code;
									G_sets[0] = FinalByteSetting_Latin(final_byte);
									pByte++;
									break;
							}							
						}						
						break;
				}//~switch(pByte[0])			
				
			} //~if(pByte[0]==ESC)			
			////////////////////////////////////////////////////////////////
			//invokation of code element
			else if(pByte[0]==LS0) //LS0
			{
				GL_area = G_sets[0];
				pByte++;
			}
			else if(pByte[0]==LS1) //LS1
			{
				GL_area = G_sets[1];
				pByte++;
			}
			else if(pByte[0]==SS2) //SS2
			{
				Single_Shift_Flag = 1;
				Before_Shift_set = GL_area;
				GL_area = G_sets[2];
				pByte++;
			}
			else if(pByte[0]==SS3) //SS3
			{
				Single_Shift_Flag = 1;
				Before_Shift_set = GL_area;
				GL_area = G_sets[3];
				pByte++;
			}			
			else if(pByte[0]==CS)
			{
				//clear
				//pDst[out_len++] = I_CS;
				pByte++;
			}
			else if(pByte[0]==APR)
			{
				//change line		
				//CR+LF
				pDst[out_len++] = 0x000D;
				pDst[out_len++] = 0x000A;
				pByte++;
			}
			//end invokation of code element
			////////////////////////////////////////////////////////////////
			else
				pByte++;
		}
		else if(pByte[0]>=0x20 && pByte[0]<0x80) //GL
		{
			if(pByte[0]==SP)
			{
				//space				
				pDst[out_len++] = 0x0020;
				pByte++;
			}
			else if(pByte[0]==DEL)
			{
				pDst[out_len++] = 0x007F;
				pByte++;
			}
			else
			{
				if(GL_area==LATIN_EXTENSION)
				{
					code = pByte[0]-0x20;
					if(LatinExt_utf16_tab[code]>0)
						pDst[out_len++] = LatinExt_utf16_tab[code];					
					pByte++;
				}
				else if(GL_area==ALPHANUMERIC_LATIN)
				{		
					code = pByte[0];
					pDst[out_len++] = code;
					pByte++;				
				}
				else if(GL_area==SPECIAL_CHARACTERS_SET)
				{					
					code = pByte[0]-0x20;					
					if(SpecialCharacter_utf16_tab[code]>0)
						pDst[out_len++] = SpecialCharacter_utf16_tab[code];
					pByte++;
				}
				else if(GL_area == DRCS_FONT)
				{
					pByte++;
				}
				else
					pByte++;
			}

			if(Single_Shift_Flag)
			{
				GL_area = Before_Shift_set;
				Single_Shift_Flag = 0;
			}
		}
		else if(pByte[0]>=0x80 && pByte[0]<0xA0) //C1
		{
			if(pByte[0]>=BKF && pByte[0]<=WHF)
			{				
				pByte++;
		   	}
			else if (pByte[0] == MSZ)
			{				
				pByte++;
			}
			else if (pByte[0] == NSZ)
			{				
				pByte++;
			}
			else if(pByte[0] == FLC)
		   	{
		   		pByte+=2;
		   	}
	   		else if(pByte[0] == TIM)
	   		{				
	   			pByte+=3;
		   	}
			else
				pByte++;
		}
		else //GR
		{
			if(GR_area==LATIN_EXTENSION)
			{
				code = pByte[0]-0xA0;
				if(LatinExt_utf16_tab[code]>0)
					pDst[out_len++] = LatinExt_utf16_tab[code];					
				pByte++;
			}
			else if(GR_area==ALPHANUMERIC_LATIN)
			{
				code = pByte[0];
				pDst[out_len++] = code-0x80;
				pByte++;				
			}
			else if(GR_area==SPECIAL_CHARACTERS_SET)
			{
				pByte++;
			}
			else
				pByte++;			
		}

		if(out_len >= dst_length)
			return out_len;
	}	
	return out_len;
}
