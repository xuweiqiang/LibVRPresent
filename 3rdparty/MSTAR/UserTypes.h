// $Change: 395037 $
//--------------------------------------------------------------
// UserType.H
// 
// ��¼�û��Զ������Ϣ�����ݽṹ
//
// ��ע��
//    ���ָ��ļ���ԭ���������ͬ��Ķ�������һ�飬����ά��
//    �����Ѳ�ͬӦ�ó�������ԡ����Է���һ�𣬺���û�кܺõķ���������ң�ֻ��ÿһ��Ӧ�ó���һ����Ӧ�ļ������ҷ�ʽͨ���ؼ�ID����
//
// ��ʷ��¼��
//
//---------------------------------------------------------------

#ifndef _USERTYPES_H_
#define _USERTYPES_H_
//-----------------------------��������---��ʼ-------------------------------------------
//�ⲿ�����ò�ͬ���ͣ�������Ҫ�Ĺ���
//�޸����ú�REBUILD ALL
#define	EXIT2EXP_NO_CONDTN	1
#define FUNC_STYLE    1//�Ƿ��з������
#define FUNC_FM		  0//�Ƿ���FM����
#define FUNC_BLUETOOTH  0//�Ƿ����������Ṧ��
	#define GPSNAME _T("G736A")
	#define GPSNAMELENGTH  5

//ѡ�����ķֱ���
#define SCREEN_320_240   1
#define SCREEN_480_272   0

//����Ƶ����������ʱ���޸�CURAXPLAYER,�����޸ı���������
//                =VIDEOPLAYER  ͬʱ��Ҫ�޸�LINK�����ΪVPlayer.app
//                =AUDIOPLAYER  ͬʱ��Ҫ�޸�LINK�����ΪAPlayer.app
//AXPLAY������
#define AUDIOPLAYER 1
#define VIDEOPLAYER 2

#if 0//�޸�ʱ�������޸�Ŀ���ļ���Project-setting-(Axplayer)link
#define CURAXPLAYER VIDEOPLAYER
#else
#define CURAXPLAYER AUDIOPLAYER //ȷ�����������Ƶ������������Ƶ������
#endif
//-----------------------------��������---����-------------------------------------------

//-----------------------------�������õ��ĳ���---��ʼ-----------------------------------
//�ⲿ��һ�㲻���޸�
//---------------------------------------------------------------------------------------
#ifdef _WIN32_WCE
#define SSCANF  swscanf
#else
#define SSCANF  sscanf
#endif

//--------------------------------------------------------------------------------------
//����͸��ɫ
// 0 :��ʾû��͸��ɫ
//͸��ɫ������0��ɫ�������ɫ
#define TRANSCOLOR  0//�����ͼû��͸��ɫ
#define TRANSCOLOR1 RGB(0xFF,0,0xFF) //ĳЩ�ؼ�����͸��ɫ
#define INVALIDCOLOR  RGB(196,196,196)//�ؼ���Ч״̬ʱ

//���ı�ʱ��������ɫ������Ҫ�ı�
#define BLACKCOLOR RGB(0,0,0)
#define WHITECOLOR RGB(0xFF,0xFF,0xFF)
#define GREENCOLOR RGB(0,0xff,0)
//-----------------------------------------------------------------------------------
#define DEFAULT_GPSPATH "\\Storage Card\\Maps\\RTNavi\\RTNavi.exe"  //Ĭ�ϵ���·��
#define DEFAULT_VOLUME 3 //Ĭ������
#define DEFAULT_BRIGHT 3 //Ĭ������
#define DEFAULT_LANG ENG  //Ĭ������

#define BATTERY_STEPS                  5
#define BATTERY_ONESTEP				   22// = ((100-BATTERYLOWVALUE)/(BATTERY_STEPS-1))
#define BATTERYLOWVALUE				   15//�е�ص����͵Ľ���
#define BATTERYFULLVALUE			   100
#define BATTERYEMPTYVALUE			   0//������

#define BRIGHT_STEPS                   6
#define BRIGHTREG_STEPS  100
#define BRIGHTMIN_STEPS  5
#define BRIGHTMAX_STEPS   50
#define BRIGHT_STEP1   8
#define BRIGHT_STEP2   12
#define BRIGHT_STEP3   20
#define BRIGHT_STEP4   30

#define VOLUME_STEPS                   6
#define SYSVOL_MAX                     0xCCCC//0xFFFF

//�����豸��
#define SPI_BRIGHT_DRIVER_NAME		_T("PWM1:")
#define I2C0_DRIVER_NAME		_T("I2C0:")

//-------------------------------------------------------------------------------------
#define FILELISTINDEXOFFSET    200  //��ֹ��ͬӦ�ó����õ�����ʱ�ؼ�������ظ�
//--------------------------------------------------------------------------------------
//���嶨ʱ��ʱ��
#ifdef _WIN32_WCE
#define CHECKDBLCLKTIME 200
#else
#define CHECKDBLCLKTIME 200
#endif

#define SLEEPRESETTIMERCOUNT    5000

#define PLAYTIMERID			1 //���Ŷ�ʱ��ID
#define IDLERESETTIEMRID	2 //����ʱ�临λ��ʱ��ID
#define DCLKCHECKID			3 //˫����ⶨʱ��ID
#define CHARGECHECKTIMERID	4 //��ص�����ʱ��ID
#define CAPTURECHECKTIMERID 5 //����ȡ��ⶨʱ��ID
#define APPCHECKTIMERID     6 //Ӧ�ó����ⶨʱ��ID
#define DATEREFRESHTIMERID  7 //����ˢ�¶�ʱ��ID
#define READYTIMERID        8 //����ǰ׼��ʱ��ID
#define REPEATTIMERID       9 //�ظ�������ʱ��ID

//---------------------------------------------------------------------------------------
//�û��Զ�����Ϣ
#define WM_USERBACKLIGHTONOFF	(WM_USER+0x200)
#define WM_USEREXITUSBMODE		(WM_USER+0x201)
#define WM_USERENTERUSBMODE     (WM_USER+0x202)
#define WM_GPSPLAYSOUND			(WM_USER+0x203)

//������Ϣ����������һ��������ټ�������Ϣ�Ӳ��õĲ���
//�������Լ�����Ϣ������
//���ڿ�ʼ���ʱ�ֿ���,��û�кϳɸ�һ����Ϣ�����������˿�ʼ�����˼��
#define WM_USERCLK				(WM_USER+0x21F)
#define WM_USERDBLCLK			(WM_USER+0x220)
#define WM_USERPLAYCHANGE		(WM_USER+0x221)
#define WM_USERFOCUSITEMCHANGE	(WM_USER+0x222)
#define WM_USERDESTORYWINDOW	(WM_USER+0x223)
#define WM_USERPLAYERMSG		(WM_USER+0x224)//���ڲ�������̬�������ӿ���Ϣ
#define WM_USERGPSRUN			(WM_USER+0x225)//GPS���������ʼ����
#define WM_USERBTNDOWNREPEAT    (WM_USER+0x226)//���������ظ���Ϣ
#define WM_USERLANGUAGECHANGE	(WM_USER+0x227)//���Ըı�
#define WM_USERMAINEXIT         (WM_USER+0x228)//�������˳���Ϣ
#define WM_USERSTYLECHANGE		(WM_USER+0x229)//���ı�
#define WM_USERBTMSG_EVENT		(WM_USER+0x22A)//����ģ���¼���Ϣ
#define WM_USERBTWND_SHOW		(WM_USER+0x22B)
#define WM_USERMODULE			(WM_USER+0x22C)//ģ�鿪�ص���Ϣ
	#define WPARA_FM			0x01
	#define WPARA_BT			0x02
//--------------------------------------------------------------------------------------
//����ע���ļ�ֵ
#define MAINKEY_MACHINE      HKEY_LOCAL_MACHINE
	#define REGKEY_POWER		 _T("System\\CurrentControlSet\\Control\\Power")
		#define REGVAL_BATTPOWEROFF  _T("BattPowerOff")
		#define REGVAL_EXTPOWEROFF   _T("ExtPowerOff")
		#define REGVAL_SHUTDOWN      _T("ShutDown")

	#define REGKEY_POWERTIMEOUTS  _T("System\\CurrentControlSet\\Control\\Power\\Timeouts")
		#define REGVAL_ACUSERIDLE     _T("ACUserIdle")
		#define REGVAL_BATTUSERIDLE   _T("BattUserIdle")
		#define REGVAL_ACSYSTEMIDLE   _T("ACSystemIdle")
		#define REGVAL_BATTSYSTEMIDLE _T("BattSystemIdle")
		#define REGVAL_ACSUSPEND      _T("ACSuspend")
		#define REGVAL_BATTSUSPEND    _T("BattSuspend")
    
	#define REGKEY_PLAYER        _T("Software\\AxPlayer\\OPTION")
        //pictrue setting
		#define REGVAL_PICTRUEINDEX	_T("PictrueIndex")
		#define REGVAL_PICTRUESPEED _T("PictrueSpeed")
		#define REGVAL_PICTRUEAUTOPLAY _T("PictrueAutoplay")
		#define REGVAL_PICTRUERANDOM    _T("PictrueRandom")

        //movie setting
		#define REGVAL_MOVIEINDEX  _T("MovieIndex")
		#define REGVAL_MOVIEPLAYMODE  _T("MoviePlayMode")

		//music setting
		#define REGVAL_MUSICINDEX  _T("MusicIndex")
		#define REGVAL_MUSICPLAYMODE	_T("MusicPlayMode")
		#define REGVAL_FAVINDEX  _T("FavIndex")
		#define REGVAL_PLAYLIST  _T("PlayList")

		//reader setting
		#define REGVAL_TXTNAME   _T("TxtName")

        //setting
		#define REGVAL_MENULANG    _T("Language")
		#define REGVAL_BRIGHTSTEPS	_T("Bright")
		#define REGVAL_VOLSTEPS     _T("Volume")
		#define REGVAL_TIMEZONE     _T("Timezone")
		#define REGVAL_STYLE		_T("STyle")

		#define REGVAL_GPSPATH		_T("GpsPath")
	
		#define REGVAL_FMCTRL       _T("FMCtrl")
		#define REGVAL_FMFREQ       _T("FMFreq")

		#define REGVAL_BTCTRL       _T("BTCtrl")

	#define REGKEY_PWM1    _T("Drivers\\PWM\\1")
		#define REGVAL_TCNTB   _T("Tcntb")
		#define REGVAL_TCMPB   _T("Tcmpb")
		#define REGVAL_BRIGHTSTEP	   _T("Step")

#define REGKEY_AUDIODRIVER _T("\\Drivers\\BuiltIn\\Audio")
		#define REGVAL_PRIFIX _T("Prifix")
		#define REGVAL_AUDIODLL _T("DLL")
		#define REGVAL_INDEX _T("Index")
		#define REGVAL_ORDER _T("Order")

#define MAINKEY_CURUSER      HKEY_CURRENT_USER
	#define REGKEY_BACKLIGHT     _T("ControlPanel\\BackLight")
		#define REGVAL_ACTIMEOUT     _T("ACTimeout")
		#define REGVAL_BATTTIMEOUT   _T("BatteryTimeout")
		#define REGVAL_USEEXT        _T("UseExt")
		#define REGVAL_USEBATT       _T("UseBattery")
//-------------------------------------------------------------
//����Ӧ�ó���HANDLEֵ����������
//����Դ·������Դ������
#ifndef FOURCC
#define FOURCC(a,b,c,d) \
	(((DWORD)(a) << 0) | ((DWORD)(b) << 8) | \
	((DWORD)(c) << 16) | ((DWORD)(d)<< 24))
#endif

//����·��
#define STORRAGECARD "\\Storage Card" //�洢��·��
#define NANDFLASH     "\\ResidentFlash" 
#define GAPPOPTPATH   "\\ResidentFlash\\GOpt\\" 
#ifdef _WIN32_WCE
#define SEARCHPATH "\\Storage Card" //ȫ����·��
#else
#define SEARCHPATH "D:\\WINCE_WORK\\Gps733\\Resource" //ȫ����·��
#endif

#ifndef _WIN32_WCE  //PC����Դ·��
#define SOURCEPATH  CString("D:\\WINCE_WORK\\Gps733\\Resource")
#endif

#ifdef _WIN32_WCE
#define OPTIONFILEPATH  (CString(GAPPOPTPATH)+CString("\\GpsUI.opt")) //�����ļ�·��
#else
#define OPTIONFILEPATH  "D:\\WINCE_WORK\\GApp2\\UI\\GpsUI.opt" //�����ļ�·��
#endif
#define COPYOPTIONFILEPATH "\\Storage Card\\GpsUI.opt" //���������ļ�·������������ļ������ڣ�����Ҹ�·����


//��ص�������ʾ
#define SHOWHINT_PROGID FOURCC('H','I','N','T')
#define SHOWHINT_NAME  _T("Hint")
#define SHOWHINT_FILENAME "ShowHint.app"

//��������
#define APPMAIN_PROGID FOURCC('A','P','P','D')
#define APPMAIN_NAME   _T("GApp")

//������ܵ�Ƥ���ļ�,�����SINGLESKINFILE
//#define SINGLESKINFILE   //��һ����ͼ�Ứʱ��Ƚ϶�
#define SKINFILENAME "D:\\WINCE_WORK\\GApp2\\UI\\Skin.jpg"
//��ͼ
#define MAPUI_NAME _T("Map")
#define MAPUI_PROGID FOURCC('G','M','A','P')
//������
#define MAINUI_PROGID FOURCC('M','A','I','N')
#define MAINUI_NAME _T("GpsUI")
#ifdef SINGLESKINFILE
#define MAINUISKINFILE SKINFILENAME
#else
#define MAINUISKINFILE "D:\\WINCE_WORK\\GApp2\\UI\\Main.jpg"
#endif
#define MAINUI_FILENAME "GpsUI.app"

//���ý���
#define GSET_PROGID FOURCC('G','S','E','T')
#define GSET_NAME   _T("GSet")
#ifdef SINGLESKINFILE
#define GSETSKINFILE SKINFILENAME
#else
#define GSETSKINFILE "D:\\WINCE_WORK\\GApp2\\UI\\GSet.jpg"
#endif
#define GSET_FILENAME "GSet.app"

//ͼƬ�����
#define IMAGEVIEWER_PROGID FOURCC('P','I','C','D')
#define IMAGEVIEWER_NAME _T("ImageViewer")
#ifdef _WIN32_WCE
#define IMAGESOURCEPATH "\\Storage Card\\Photo"
#else
#define IMAGESOURCEPATH (SOURCEPATH+CString("\\Photo"))
#endif
#define IMAGEFILTER  "BMP;JPG;JPEG;"
#ifdef SINGLESKINFILE
#define IMAGESKINFILE SKINFILENAME
#else
#define IMAGESKINFILE "D:\\WINCE_WORK\\GApp2\\UI\\Picture.jpg"
#endif
#define IMAGEVIEWER_FILENAME "ImageViewer.app"
#define IMAGEFILELIST_FILENAME "ImageViewer.lst"

//TXT������
#define TXTREADER_PROGID  FOURCC('T','E','X','T')
#define TXTREADER_NAME _T("TXTReader")
#define TXTBOOKMARKFILE "bk.dat"
#ifdef _WIN32_WCE
#define TXTSOURCEPATH "\\Storage Card\\eBook"
#else
#define TXTSOURCEPATH (SOURCEPATH+CString("\\eBook"))
#endif
#define  TXTFILTER "TXT;"
#ifdef SINGLESKINFILE
#define TXTSKINFILE SKINFILENAME
#else
#define TXTSKINFILE "D:\\WINCE_WORK\\GApp2\\UI\\Reader.jpg"
#endif
#define TXTREADER_FILENAME "Reader.app"
#define TXTFILELIST_FILENAME "Reader.lst"

#define APLAYER_PROGRAMID  FOURCC('A','M','P','3')
#define APLAYER_NAME _T("APlayer")
#ifdef _WIN32_WCE
#define MUSICSOURCEPATH "\\Storage Card\\Music"
#else
#define MUSICSOURCEPATH (SOURCEPATH+CString("\\Music"))
#endif
#define  MUSICFILTER "MP3;WMA;WAV;"
#define AUDIOPLAYER_FILENAME "APlayer.app"

#define VPLAYER_PROGRAMID  FOURCC('V','M','P','4')
#define VPLAYER_NAME _T("VPlayer")
#ifdef _WIN32_WCE
#define MOVIESOURCEPATH "\\Storage Card\\Movie"
#else
#define MOVIESOURCEPATH (SOURCEPATH+CString("\\Movie"))
#endif
#define  MOVIEFILTER "MPEG;MPG;AVI;WMV;MP4;"
#define  VIDEOPLAYER_FILENAME "VPlayer.app"
#ifdef SINGLESKINFILE
#define AXPLAYSKINFILE SKINFILENAME
#else
#define AXPLAYSKINFILE "D:\\WINCE_WORK\\GApp2\\UI\\AxPlayer.jpg"
#endif
#if (CURAXPLAYER == VIDEOPLAYER)
#define AXPLAYERFILTER MOVIEFILTER
#define AXPLAYER_PROGID  VPLAYER_PROGRAMID
#define AXPLAYER_NAME VPLAYER_NAME
#define AXPLAYERSOURCEPATH MOVIESOURCEPATH
#define AXPLAYFILELIST_FILENAME   "VPlayer.lst"
#else
#define AXPLAYERFILTER MUSICFILTER
#define AXPLAYER_PROGID  APLAYER_PROGRAMID
#define AXPLAYER_NAME APLAYER_NAME
#define AXPLAYERSOURCEPATH MUSICSOURCEPATH
#define AXPLAYFILELIST_FILENAME   "APlayer.lst"
#define AXPLAYFAVLIST_FILENAME    "APlayerFav.lst"
#endif

//ͨ�öԻ���
#define COMDLGSKINFILE "D:\\WINCE_WORK\\GApp2\\UI\\PopUp.jpg" 

//��������
#define BT_PROGID  FOURCC('B','T','_','_')
#define BT_NAME _T("TXTReader")
#define BLUETOOTHSKINFILE "D:\\WINCE_WORK\\GApp2\\UI\\BlueTooth.jpg" 
#define BLUETOOTH_NAME   _T("BlueTooth.app")

//-----------------------------------�������õ��ĳ���---����------------------------------------------
//-----------------------------------------------------------------
//���ؼ��������ݽṹ
typedef enum _LanguageId
{
	ENG, //Ӣ��
	SHN, //��������
	CHN, //��������

	LANG_NUM
}LANGUAGEID;

typedef enum _STYLE
{
	STYLE_BLUE,
	STYLE_BLACK,
	STYLE_GREEN,

	STYLE_NUM
}TYPE_STYLE;

typedef struct _Wnd_property
{
	DWORD WndID;
	RECT WndRect;//����λ�á���С
	//�����Ҫ���ӣ�����Ҫ�Ȼ�ȡ�ÿؼ�����������ͼ�ϵ�λ�ô�С
	RECT WndGndRect;//��������������ͼ��λ�á���С
	RECT NormalBmpRect;//����û�л�ý���ʱ��λͼ����λ�á���С
	RECT FocusBmpRect;//���ڻ�ý���ʱ��λͼ����λ�á���С

	char LangText[LANG_NUM][200];//���ڰ������ֵĶ�����Է���
}WND_PROPERTY;

#endif
