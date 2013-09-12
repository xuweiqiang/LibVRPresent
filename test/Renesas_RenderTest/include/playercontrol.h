//********************************************************************
//Renesas Player Controller
//		by Benson Chuang
//			benson.chuang.tw@gmail.com  benson.chuang@renesas.com
//********************************************************************
//Player : AAC , H264 , H264+AAC
//						Renesas Technology Taiwan/Engineerning Center
//********************************************************************
#ifndef _PLAYER_CONTROL_H
#define _PLAYER_CONTROL_H

//**************************************
//Public Function
//**************************************
void Player_Initial(PLAYER_INFO *player_info , PLAYER_NOTIFY_CB *cb_func);
unsigned long Player_Open(PLAYER_INFO *player_info);
unsigned long Player_Stop(PLAYER_INFO *player_info);
unsigned long Player_Play(PLAYER_INFO *player_info);
unsigned long Player_Pause(PLAYER_INFO *player_info);
unsigned long Player_Close(PLAYER_INFO *player_info);
unsigned long Player_SetViewWindows(PLAYER_INFO *player_info , PLAYER_SCREEN_TYPE type);

//**************************************
//Private Function
//**************************************
static PLAYER_DECODE_TYPE File_Check(WCHAR *filename);

#endif //_PLAYER_CONTROL_H