
#include "dmgl_jis.h"
#include "cc_1seg.h"
#include "jis_convert.h"

/*
NEC Font index is the same as Kanji Set in ARIB-B24, and with symbols in Alpha-num(row 3), Hiragana(row 4), Katakana(row 5)
*/
static int GetKANJI_NEC(unsigned char *byte, unsigned short *index, int offset)
{
	int byte0, byte1;	

	byte0 = byte[0];
	byte1 = byte[1];

	//Kanji table starts with 0x21
	byte0 -= (offset+0x21);
	byte1 -= (offset+0x21);

	if(byte0>=94||byte1>=94)
		return 0;
	*index = (byte0*94)+byte1;
	return 1;
}


/*
NEC Font, Alpha-num(row 3)
*/
static int GetALPHANUMERIC_NEC(unsigned char *byte, unsigned short *index, int offset)
{
	int byte0;	

	byte0 = byte[0];	
	byte0 -= (offset+0x21);

	if(byte0>=94)
		return 0;
	*index = (2*94)+byte0;
	return 1;
}

/*
NEC Font, Hiragana(row 4)
*/
static int GetHIRAGANA_NEC(unsigned char *byte, unsigned short *index, int offset)
{
	int byte0;	

	byte0 = byte[0];	
	byte0 -= (offset+0x21);

	if(byte0>=94)
		return 0;
	*index = (3*94)+byte0;
	return 1;
}

/*
NEC Font, Katakana(row 5)
*/
static int GetKATAKANA_NEC(unsigned char *byte, unsigned short *index, int offset)
{
	int byte0;	

	byte0 = byte[0];	
	byte0 -= (offset+0x21);

	if(byte0>=94)
		return 0;
	*index = (4*94)+byte0;
	return 1;
}

/*
NEC Font, additional symbols startds from row 90
*/
static int GetADDSIGN_NEC(unsigned char *byte, unsigned short *index, int offset)
{
	int byte0, byte1;	

	byte0 = byte[0];
	byte1 = byte[1];

	//Kanji table starts with 0x21
	byte0 -= (offset+0x21);
	byte1 -= (offset+0x21);

	if(byte0>=94||byte1>=94)
		return 0;
	*index = (byte0*94)+byte1;
	return 1;
}
//End of NEC Font


/*
int Get_GL_GR_AlphaNum_Sjis(int data)
{
	data = (data&0x7F); 
	if((data<0x21)||(data>0x7E))
		return -1;
	//shit-jis start from 8140   jis from (pByte[0]-0xA0-1)
	if((data>=0x30) &&(data <= 0x39))		//0-9
		return data+0x821F;//-0x30+0x824F
	if((data>=0x41) &&(data <= 0x5A))		//A-Z
		return data+0x821F;//-0x41+0x8260
	if((data>=0x61) &&(data <= 0x7A))		//a-z
		return data+0x8220;//-0x61+0x8281
	switch (data){
	case 0x21:							//
		return 0x8149;
	case 0x22:							//〃
		return 0x818D;
	case 0x23:							//…
		return 0x8194;
	case 0x24:							//
		return 0x8190;
	case 0x25:							//
		return 0x8193;
	case 0x26:							//‘
		return 0x8195;
	case 0x27:							//‖
		return 0x818C;
	case 0x28:							//
		return 0x8169;
	case 0x29:							//
		return 0x816A;
	case 0x2A:							//’
		return 0x8196;
	case 0x2B:							//∠
		return 0x817B;
	case 0x2C:							//
		return 0x8143;
	case 0x2D:							//⌒
		return 0x817C;
	case 0x2E:							//
		return 0x8144;
	case 0x2F:							//〓
		return 0x815E;
	case 0x3A:							//
		return 0x8146;
	case 0x3B:							//
		return 0x8147;
	case 0x3C:							//≌
		return 0x8183;
	case 0x3D:							//∽
		return 0x8181;
	case 0x3E:							//≈
		return 0x8184;
	case 0x3F:							//
		return 0x8148;
	case 0x40:							//
		return 0x8197;
	case 0x5B:							//其
		return 0x816D;
	case 0x5C:							//
		return 0x818F;
	case 0x5D:							//棋
		return 0x816E;
	case 0x5E:							//?
		return 0x814F;
	case 0x5F:							//∧
		return 0x8151;
	case 0x60:							//`
		return 0x814D;
	case 0x7B:							//
		return 0x816F;
	case 0x7C:							//
		return 0x8162;
	case 0x7D:							//
		return 0x8170;
	case 0x7E:							//∶
		return 0x8150;
	default:
		return -1;
	}
}
*/
/*
return DRCS Set Number, from 0-15,
return -1 if error
*/
static int FinalByteSetting_DRCS(int final_byte)
{
	//DRCS-0 to DRCS-15 ==> 0x40 to 0x4F
	if( ((final_byte>>4)&0xF) != 4)
		return -1;

	return (final_byte&0xF);
}

static GSetName FinalByteSetting(int final_byte)
{
	switch(final_byte)
	{
		case 0x42:
		return KANJI;
		
		case 0x4A:
		return ALPHANUMERIC;
				
		case 0x30:
		return HIRAGANA;
		
		case 0x31:
		return KATAKANA;
		
		case 0x32:
		return MOSAICA;
		
		case 0x33:
		return MOSAICB;
		
		case 0x34:
		return MOSAICC;
		
		case 0x35:
		return MOSAICD;
		
		case 0x36:
		return P_ALPHANUMERIC;
		
		case 0x37:
		return P_HIRAGANA;
		
		case 0x38:
		return P_KATAGANA;
		
		case 0x49:
		return JISX0201KATAGANA;

		case 0x39: 
		return KANJI;//JISCHANGEKNAJI1;

		case 0x3A:
		return KANJI;//JISCHANGEKNAJI2;

		case 0x3B:
		return ADDSIGN;
	}
	return KANJI;
}

/*
for closed caption
*/

/*
The default setting
G0 Kanji system set
G1 Alphanumeric set
G2 Hiragana set
G3 Macro code set ?? or Katakana set

GL=>(LS0)  G0
GR=>(LS2R) G2 
*/
int CharacterDecode_JIS(unsigned int *pDst, int dst_length, unsigned char* pSrc, int src_length, GSetName gl, GSetName gr, void *param)
{	
	unsigned char *pByte = pSrc;
	unsigned char *pLastByte = pByte+src_length;
	int code;
	int out_len=0;
	
	int G_sets[4];
	int GL_area, GR_area;
	
	//use this flag to record the shift
	//default DM_FALSE 
	//if DM_TRUE after drawing the character we need to roll back the previous set
	int Before_Shift_set;
	int Single_Shift_Flag = 0;
	int MidSize = 0;

	//DRCS default: DRCS_1, 1 byte code
	int DRCS_Set = 0x41;
	int DRCS_bytes = 1;

	//default setting
	G_sets[0] = KANJI;
	G_sets[1] = ALPHANUMERIC;
	G_sets[2] = HIRAGANA;
	G_sets[3] = KATAKANA;	
	
	/*
#if 0
	//for EPG default setting 
	GL_area = G_sets[0];
	GR_area = G_sets[2];	
#else
	//for Closed Caption default setting
	GL_area = DRCS_FONT;
	GR_area = KANJI;
#endif
	*/
	GL_area = gl;
	GR_area = gr;


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
							G_sets[0] = FinalByteSetting(final_byte);
							pByte+=3;
						}
						else
						{
							//1-byte G set
							final_byte = pByte[1];
							G_sets[0] = FinalByteSetting(final_byte);
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
							G_sets[1] = FinalByteSetting(final_byte);
							pByte+=3;
						}
						else
						{
							//1-byte G set
							final_byte = pByte[1];
							G_sets[1] = FinalByteSetting(final_byte);
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
							G_sets[2] = FinalByteSetting(final_byte);
							pByte+=3;
						}
						else
						{
							//1-byte G set
							final_byte = pByte[1];
							G_sets[2] = FinalByteSetting(final_byte);
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
							G_sets[3] = FinalByteSetting(final_byte);
							pByte+=3;
						}
						else
						{
							//1-byte G set
							final_byte = pByte[1];
							G_sets[3] = FinalByteSetting(final_byte);
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
									G_sets[0] = FinalByteSetting(final_byte);
									pByte+=3;
									break;
								case 0x29:
									final_byte = pByte[2];							
									G_sets[1] = FinalByteSetting(final_byte);
									pByte+=3;
									break;
								case 0x2A:
									final_byte = pByte[2];							
									G_sets[2] = FinalByteSetting(final_byte);
									pByte+=3;
									break;
								case 0x2B:
									final_byte = pByte[2];							
									G_sets[3] = FinalByteSetting(final_byte);
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
									G_sets[1] = FinalByteSetting(final_byte);
									pByte+=2;
									break;
								case 0x2A:
									final_byte = pByte[1];									
									G_sets[2] = FinalByteSetting(final_byte);
									pByte+=2;
									break;
								case 0x2B:									
									final_byte = pByte[1];									
									G_sets[3] = FinalByteSetting(final_byte);
									pByte+=2;
									break;
								default:
									final_byte = code;
									G_sets[0] = FinalByteSetting(final_byte);
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
				if(GL_area==KANJI)
				{
					int len;
					unsigned short out_code;
					len = GetKANJI_NEC(pByte, &out_code, 0);
					if(len)
						pDst[out_len++] = out_code;					
					pByte+=2;
				}
				else if(GL_area==ALPHANUMERIC)
				{						
					int len;
					unsigned short out_code;
					len = GetALPHANUMERIC_NEC(pByte, &out_code, 0);
					if(len)
						pDst[out_len++] = out_code;					
					pByte++;
				}
				else if(GL_area==HIRAGANA)
				{
					int len;
					unsigned short out_code;
					len = GetHIRAGANA_NEC(pByte, &out_code, 0);
					if(len)
						pDst[out_len++] = out_code;					
					pByte++;				
									
				}
				else if(GL_area==KATAKANA)
				{
					int len;
					unsigned short out_code;
					len = GetKATAKANA_NEC(pByte, &out_code, 0);
					if(len)
						pDst[out_len++] = out_code;					
					pByte++;				
				}
				else if(GL_area==ADDSIGN)
				{
					int len;
					unsigned short out_code;
					len = GetADDSIGN_NEC(pByte, &out_code, 0);
					if(len)
						pDst[out_len++] = out_code;					
					pByte+=2;
				}
				else if(GL_area==DRCS_FONT)
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

				/*
					MOSAICA,
					MOSAICB,
					MOSAICC,
					MOSAICD,
					P_ALPHANUMERIC,
					P_HIRAGANA,
					P_KATAGANA,
					JISX0201KATAGANA,
					JISCHANGEKNAJI1,
					JISCHANGEKNAJI2,
					ADDSIGN
				*/
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
					pDst[out_len++] = I_FLC_ON;
				}
				else if(*(pByte+1) == 0x4F)
				{					
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
			if(GR_area==KANJI)
			{
				int len;
				unsigned short out_code;
				len = GetKANJI_NEC(pByte, &out_code, 0x80);
				if(len)
					pDst[out_len++] = out_code;					
				pByte+=2;
			}
			else if(GR_area==ALPHANUMERIC)
			{
				int len;
				unsigned short out_code;
				len = GetALPHANUMERIC_NEC(pByte, &out_code, 0x80);
				if(len)
					pDst[out_len++] = out_code;					
				pByte++;				
			}
			else if(GR_area==HIRAGANA)
			{
				//Hiragana	
				int len;
				unsigned short out_code;
				len = GetHIRAGANA_NEC(pByte, &out_code, 0x80);
				if(len)
					pDst[out_len++] = out_code;					
				pByte++;				
			}
			else if(GR_area==KATAKANA)//安
			{
				int len;
				unsigned short out_code;
				len = GetKATAKANA_NEC(pByte, &out_code, 0x80);
				if(len)
					pDst[out_len++] = out_code;					
				pByte++;				
			}
			else if(GR_area == ADDSIGN)
			{
				int len;
				unsigned short out_code;
				len = GetADDSIGN_NEC(pByte, &out_code, 0x80);
				if(len)
					pDst[out_len++] = out_code;					
				pByte+=2;
			}
			else
				pByte++;			
		}

		if(out_len>=dst_length)
			return out_len;
	}	
	return out_len;
}


int JIStoSJIS(unsigned char* pShiftJis, int iSjisLen, unsigned char* pJis, int iJisLen, GSetName gl, GSetName gr, void *param) 
{	
	unsigned char *pByte = (unsigned char*)pJis;
	unsigned char *pLastByte = pByte+iJisLen;
	unsigned char *pDst = (unsigned char*)pShiftJis;
	int code;
	int out_len = 0;

	int G_sets[4];
	int GL_area, GR_area;

	//use this flag to record the shift
	//default DM_FALSE 
	//if DM_TRUE after drawing the character we need to roll back the previous set
	int Before_Shift_set;
	int Single_Shift_Flag = 0;
//	int MidSize = 0;

	//default setting
	G_sets[0] = KANJI;
	G_sets[1] = ALPHANUMERIC;
	G_sets[2] = HIRAGANA;
	G_sets[3] = KATAKANA;	

	GL_area = gl;
	GR_area = gr;

	while (pByte < pLastByte)
	{	
		if (pByte[0] < 0x20) //C0
		{			
			if (pByte[0] == ESC)
			{
				int final_byte;
				pByte++;
				switch (pByte[0])
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
						G_sets[0] = FinalByteSetting(final_byte);
						pByte+=3;
					}
					else
					{
						//1-byte G set
						final_byte = pByte[1];
						G_sets[0] = FinalByteSetting(final_byte);
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
						G_sets[1] = FinalByteSetting(final_byte);
						pByte+=3;
					}
					else
					{
						//1-byte G set
						final_byte = pByte[1];
						G_sets[1] = FinalByteSetting(final_byte);
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
						G_sets[2] = FinalByteSetting(final_byte);
						pByte+=3;
					}
					else
					{
						//1-byte G set
						final_byte = pByte[1];
						G_sets[2] = FinalByteSetting(final_byte);
						pByte+=2;
					}
					break;
				case 0x2B:
					code = pByte[0];

					//get the final byte
					if (pByte[1] == 0x20)
					{
						//1-byte DRCS							
						final_byte = pByte[2];
						G_sets[3] = FinalByteSetting(final_byte);
						pByte+=3;
					}
					else
					{
						//1-byte G set
						final_byte = pByte[1];
						G_sets[3] = FinalByteSetting(final_byte);
						pByte+=2;
					}
					break;
				case 0x24:						
					pByte++;
					code = pByte[0];
					if (pByte[1] == 0x20)
					{
						//2-byte DRCS
						switch (code)
						{							
						case 0x28:
							final_byte = pByte[2];							
							G_sets[0] = FinalByteSetting(final_byte);
							pByte+=3;
							break;
						case 0x29:
							final_byte = pByte[2];							
							G_sets[1] = FinalByteSetting(final_byte);
							pByte+=3;
							break;
						case 0x2A:
							final_byte = pByte[2];							
							G_sets[2] = FinalByteSetting(final_byte);
							pByte+=3;
							break;
						case 0x2B:
							final_byte = pByte[2];							
							G_sets[3] = FinalByteSetting(final_byte);
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
							G_sets[1] = FinalByteSetting(final_byte);
							pByte+=2;
							break;
						case 0x2A:
							final_byte = pByte[1];							
							G_sets[2] = FinalByteSetting(final_byte);
							pByte+=2;
							break;
						case 0x2B:									
							final_byte = pByte[1];							
							G_sets[3] = FinalByteSetting(final_byte);
							pByte+=2;
							break;
						default:
							final_byte = code;
							G_sets[0] = FinalByteSetting(final_byte);
							pByte++;
							break;
						}							
					}						
					break;
				}//~switch(pByte[0])			

			} //~if (pByte[0] == ESC)
			////////////////////////////////////////////////////////////////
			//invokation of code element
			else if (pByte[0] == LS0) //LS0
			{
				GL_area = G_sets[0];
				pByte++;
			}
			else if (pByte[0] == LS1) //LS1
			{
				GL_area = G_sets[1];
				pByte++;
			}
			else if (pByte[0] == SS2) //SS2
			{
				Single_Shift_Flag = 1;
				Before_Shift_set = GL_area;
				GL_area = G_sets[2];
				pByte++;
			}
			else if (pByte[0] == SS3) //SS3
			{
				Single_Shift_Flag = 1;
				Before_Shift_set = GL_area;
				GL_area = G_sets[3];
				pByte++;
			}
			/*
			else if (pByte[0] == CS)
			{
				//clear
				pDst[out_len++] = I_CS;
				pByte++;
			}
			else if (pByte[0] == APR)
			{
				//change line			   	
				pDst[out_len++] = I_APR;
				pByte++;
			}
			*/
			//end invokation of code element
			////////////////////////////////////////////////////////////////
			else
				pByte++;
		}
		else if (pByte[0]>=0x20 && pByte[0]<0x80) //GL
		{
			if (pByte[0] == SP)
			{
				//space				
				//pDst[out_len++] = I_SP;
				pByte++;
			}
			else if (pByte[0] == DEL)
			{
				//pDst[out_len++] = I_DEL;
				pByte++;
			}
			else
			{
				if (GL_area == KANJI)
				{
					out_len += KANJItoSJIS(pByte, pDst+out_len, 0);
					pByte+=2;
				}
				else if (GL_area == ALPHANUMERIC)
				{		
					out_len += ALPHANUMERICtoSJIS(pByte, pDst+out_len, 0);
					pByte++;				
				}
				else if (GL_area == HIRAGANA)
				{
					out_len += HIRAGANAtoSJIS(pByte, pDst+out_len, 0);
					pByte++;
				}
				else if (GL_area == KATAKANA)
				{
					out_len += KATAKANAtoSJIS(pByte, pDst+out_len, 0);
					pByte++;				
				}
				else if (GL_area == ADDSIGN)
				{					
					out_len += KANJItoSJIS(pByte, pDst+out_len, 0);
					pByte+=2;
				}
				else if (GL_area == DRCS_FONT)
				{
					/*
					unsigned byte0, byte1;

					byte0 = pByte[0];
					byte1 = pByte[1];

					if (byte0 == 0x41) //1seg, DRCS1 only
					{
						//DRCS Font
						tDRCSdata *DSRC_1 = (tDRCSdata*)param;					 

						if (DSRC_1)
						{
							if (byte1 >= 0x21 && byte1 <= 0x7E)
							{
								byte1 -= 0x21;
								if(DSRC_1[byte1].m_used)
								{
									pDst[out_len++] = I_DRCS;
									pDst[out_len++] = DSRC_1[byte1].m_width; //width
									pDst[out_len++] = DSRC_1[byte1].m_height; //height
									pDst[out_len++] = (unsigned int)DSRC_1[byte1].m_data; //address
								}
							}
						}
					}
					*/
					pByte+=2;
				}

				/*
				MOSAICA,
				MOSAICB,
				MOSAICC,
				MOSAICD,
				P_ALPHANUMERIC,
				P_HIRAGANA,
				P_KATAGANA,
				JISX0201KATAGANA,
				JISCHANGEKNAJI1,
				JISCHANGEKNAJI2,
				ADDSIGN
				*/
				else
					pByte++;
			}
			if (Single_Shift_Flag)
			{
				GL_area = Before_Shift_set;
				Single_Shift_Flag = 0;
			}
		}
		else if (pByte[0] >= 0x80 && pByte[0] < 0xA0) //C1
		{
			if (pByte[0] >= BKF && pByte[0] <= WHF)
			{
				/*
				if (pByte[0] == BKF)
				{
					pDst[out_len++] = I_BKF;
				}
				else if (pByte[0] == RDF)
				{
					pDst[out_len++] = I_RDF;
				}
				else if (pByte[0] == GRF)
				{
					pDst[out_len++] = I_GRF;
				}
				else if (pByte[0] == YLF)
				{
					pDst[out_len++] = I_YLF;
				}
				else if (pByte[0] == BLF)
				{
					pDst[out_len++] = I_BLF;
				}
				else if (pByte[0] == MGF)
				{
					pDst[out_len++] = I_MGF;
				}
				else if (pByte[0] == CNF)
				{
					pDst[out_len++] = I_CNF;
				}
				else if (pByte[0] == WHF)
				{
					pDst[out_len++] = I_WHF;
				}
				*/
				pByte++;
			}
			else if (pByte[0] == MSZ)
			{
				/*
				MidSize = 1;
				pDst[out_len++] = I_MSZ;
				*/
				pByte++;
			}
			else if (pByte[0] == NSZ)
			{
				/*
				MidSize = 0;
				pDst[out_len++] = I_NSZ;
				*/
				pByte++;
			}
			else if (pByte[0] == FLC)
			{
				/*
				if (*(pByte+1) == 0x40)
				{					
					pDst[out_len++] = I_FLC_ON;
				}
				else if (*(pByte+1) == 0x4F)
				{					
					pDst[out_len++] = I_FLC_OFF;
				}
				*/
				pByte+=2;
			}
			else if (pByte[0] == TIM)
			{
				/*
				//Please refer to "ARIB STD-B24 Volume1 Part2 Table7.13 C1 control character set".
				unsigned char TimeControlP1 = pByte[1];
				unsigned char TimeControlP2  = pByte[2];

				if ((TimeControlP1 == 0x20) && ((TimeControlP2 >= 0x40) && (TimeControlP2 <= 0x7F)))
				{				
					pDst[out_len++] = I_TIM;
					pDst[out_len++] = TimeControlP2&0x3F;
				}	
				*/
				pByte+=3;
			}
			else
				pByte++;
		}
		else //GR
		{
			if (GR_area == KANJI)
			{
				out_len += KANJItoSJIS(pByte, pDst+out_len, 0x80);				
				pByte+=2;
			}
			else if (GR_area == ALPHANUMERIC)
			{
				out_len += ALPHANUMERICtoSJIS(pByte, pDst+out_len, 0x80);
				pByte++;				
			}
			else if (GR_area == HIRAGANA)
			{
				out_len += HIRAGANAtoSJIS(pByte, pDst+out_len, 0x80);				
				pByte++;				
			}
			else if (GR_area == KATAKANA)//安
			{
				out_len += KATAKANAtoSJIS(pByte, pDst+out_len, 0x80);				
				pByte++;				
			}
			else if (GR_area == ADDSIGN)
			{
				out_len += KANJItoSJIS(pByte, pDst+out_len, 0x80);
				pByte+=2;
			}
			else
				pByte++;			
		}
		if(out_len>=iSjisLen)
			return out_len;
	}	
	return out_len;
}


int JIStoUTF16(unsigned short* unicode, int outlen, unsigned char* pJis, int iJisLen, GSetName gl, GSetName gr, void *param)
{	
	unsigned char *pByte = (unsigned char*)pJis;
	unsigned char *pLastByte = pByte+iJisLen;
	unsigned short* pDst = (unsigned short*)unicode;
	int code;
	int out_len = 0;

	int G_sets[4];
	int GL_area, GR_area;

	//use this flag to record the shift	
	int Before_Shift_set;
	int Single_Shift_Flag = 0;
//	int MidSize = 0;

	//default setting
	G_sets[0] = KANJI;
	G_sets[1] = ALPHANUMERIC;
	G_sets[2] = HIRAGANA;
	G_sets[3] = KATAKANA;	

	GL_area = gl;
	GR_area = gr;

	while (pByte < pLastByte)
	{	
		if (pByte[0] < 0x20) //C0
		{			
			if (pByte[0] == ESC)
			{
				int final_byte;
				pByte++;
				switch (pByte[0])
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
						G_sets[0] = FinalByteSetting(final_byte);
						pByte+=3;
					}
					else
					{
						//1-byte G set
						final_byte = pByte[1];
						G_sets[0] = FinalByteSetting(final_byte);
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
						G_sets[1] = FinalByteSetting(final_byte);
						pByte+=3;
					}
					else
					{
						//1-byte G set
						final_byte = pByte[1];
						G_sets[1] = FinalByteSetting(final_byte);
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
						G_sets[2] = FinalByteSetting(final_byte);
						pByte+=3;
					}
					else
					{
						//1-byte G set
						final_byte = pByte[1];
						G_sets[2] = FinalByteSetting(final_byte);
						pByte+=2;
					}
					break;
				case 0x2B:
					code = pByte[0];

					//get the final byte
					if (pByte[1] == 0x20)
					{
						//1-byte DRCS							
						final_byte = pByte[2];
						G_sets[3] = FinalByteSetting(final_byte);
						pByte+=3;
					}
					else
					{
						//1-byte G set
						final_byte = pByte[1];
						G_sets[3] = FinalByteSetting(final_byte);
						pByte+=2;
					}
					break;
				case 0x24:						
					pByte++;
					code = pByte[0];
					if (pByte[1] == 0x20)
					{
						//2-byte DRCS
						switch (code)
						{							
						case 0x28:
							final_byte = pByte[2];							
							G_sets[0] = FinalByteSetting(final_byte);
							pByte+=3;
							break;
						case 0x29:
							final_byte = pByte[2];							
							G_sets[1] = FinalByteSetting(final_byte);
							pByte+=3;
							break;
						case 0x2A:
							final_byte = pByte[2];							
							G_sets[2] = FinalByteSetting(final_byte);
							pByte+=3;
							break;
						case 0x2B:
							final_byte = pByte[2];							
							G_sets[3] = FinalByteSetting(final_byte);
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
							G_sets[1] = FinalByteSetting(final_byte);
							pByte+=2;
							break;
						case 0x2A:
							final_byte = pByte[1];							
							G_sets[2] = FinalByteSetting(final_byte);
							pByte+=2;
							break;
						case 0x2B:									
							final_byte = pByte[1];							
							G_sets[3] = FinalByteSetting(final_byte);
							pByte+=2;
							break;
						default:
							final_byte = code;
							G_sets[0] = FinalByteSetting(final_byte);
							pByte++;
							break;
						}							
					}						
					break;
				}//~switch(pByte[0])			

			} //~if (pByte[0] == ESC)
			////////////////////////////////////////////////////////////////
			//invokation of code element
			else if (pByte[0] == LS0) //LS0
			{
				GL_area = G_sets[0];
				pByte++;
			}
			else if (pByte[0] == LS1) //LS1
			{
				GL_area = G_sets[1];
				pByte++;
			}
			else if (pByte[0] == SS2) //SS2
			{
				Single_Shift_Flag = 1;
				Before_Shift_set = GL_area;
				GL_area = G_sets[2];
				pByte++;
			}
			else if (pByte[0] == SS3) //SS3
			{
				Single_Shift_Flag = 1;
				Before_Shift_set = GL_area;
				GL_area = G_sets[3];
				pByte++;
			}				
			else if (pByte[0] == CS)
			{
				//clear
				//pDst[out_len++] = I_CS;
				pByte++;
			}
			else if (pByte[0] == APR)
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
		else if (pByte[0]>=0x20 && pByte[0]<0x80) //GL
		{
			if (pByte[0] == SP)
			{
				//space				
				pDst[out_len++] = 0x0020;
				pByte++;
			}
			else if (pByte[0] == DEL)
			{
				pDst[out_len++] = 0x007F;
				pByte++;
			}
			else
			{
				if (GL_area == KANJI)
				{					
					out_len += KANJItoUTF16(pByte, pDst+out_len, 0);										
					pByte+=2;
				}
				else if (GL_area == ALPHANUMERIC)
				{					
					out_len += ALPHANUMERICtoUTF16(pByte, pDst+out_len, 0);				
					pByte++;				
				}
				else if (GL_area == HIRAGANA)
				{
					out_len += HIRAGANAtoUTF16(pByte, pDst+out_len, 0);				
					pByte++;
				}
				else if (GL_area == KATAKANA)
				{
					out_len += KATAKANAtoUTF16(pByte, pDst+out_len, 0);									
					pByte++;				
				}
				else if (GL_area == ADDSIGN)
				{
					out_len += KANJItoUTF16(pByte, pDst+out_len, 0);										
					pByte+=2;
				}
				else if (GL_area == DRCS_FONT)
				{
					/*
					unsigned byte0, byte1;

					byte0 = pByte[0];
					byte1 = pByte[1];

					if (byte0 == 0x41) //1seg, DRCS1 only
					{
						//DRCS Font
						tDRCSdata *DSRC_1 = (tDRCSdata*)param;					 

						if (DSRC_1)
						{
							if (byte1 >= 0x21 && byte1 <= 0x7E)
							{
								byte1 -= 0x21;
								if(DSRC_1[byte1].m_used)
								{
									pDst[out_len++] = I_DRCS;
									pDst[out_len++] = DSRC_1[byte1].m_width; //width
									pDst[out_len++] = DSRC_1[byte1].m_height; //height
									pDst[out_len++] = (unsigned int)DSRC_1[byte1].m_data; //address
								}
							}
						}
					}
					*/
					pByte+=2;
				}

				/*
				MOSAICA,
				MOSAICB,
				MOSAICC,
				MOSAICD,
				P_ALPHANUMERIC,
				P_HIRAGANA,
				P_KATAGANA,
				JISX0201KATAGANA,
				JISCHANGEKNAJI1,
				JISCHANGEKNAJI2,
				ADDSIGN
				*/
				else
					pByte++;
			}
			if (Single_Shift_Flag)
			{
				GL_area = Before_Shift_set;
				Single_Shift_Flag = 0;
			}
		}
		else if (pByte[0] >= 0x80 && pByte[0] < 0xA0) //C1
		{
			if (pByte[0] >= BKF && pByte[0] <= WHF)
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
			else if (pByte[0] == FLC)
			{				
				pByte+=2;
			}
			else if (pByte[0] == TIM)
			{				
				pByte+=3;
			}
			else
				pByte++;
		}
		else //GR
		{
			if (GR_area == KANJI)
			{			
				out_len += KANJItoUTF16(pByte, pDst+out_len, 0x80);										
				pByte+=2;
			}
			else if (GR_area == ALPHANUMERIC)
			{				
				out_len += ALPHANUMERICtoUTF16(pByte, pDst+out_len, 0x80);
				pByte++;				
			}
			else if (GR_area == HIRAGANA)
			{				
				out_len += HIRAGANAtoUTF16(pByte, pDst+out_len, 0x80);			
				pByte++;				
			}
			else if (GR_area == KATAKANA)//安
			{				
				out_len += KATAKANAtoUTF16(pByte, pDst+out_len, 0x80);					
				pByte++;				
			}
			else if (GR_area == ADDSIGN)
			{				
				out_len += KANJItoUTF16(pByte, pDst+out_len, 0x80);
				pByte+=2;
			}

			else
				pByte++;			
		}

		if(out_len>=outlen)
			return out_len;
	}	
	return out_len;
}

