// $Change: 395037 $
//--------------------------------------------------------------
// UserType.H
// 
// 记录用户自定义的消息和数据结构
//
// 备注：
//    出现该文件的原因：想把所有同类的东西放在一块，便于维护
//    最初想把不同应用程序的属性、语言放在一起，后来没有很好的方法解决查找，只有每一个应用程序一个对应文件，查找方式通过控件ID查找
//
// 历史记录：
//
//---------------------------------------------------------------

#ifndef _USERTYPES_H_
#define _USERTYPES_H_
//-----------------------------程序配置---开始-------------------------------------------
//这部分配置不同机型（不）需要的功能
//修改配置后REBUILD ALL
#define	EXIT2EXP_NO_CONDTN	1
#define FUNC_STYLE    1//是否有风格设置
#define FUNC_FM		  0//是否有FM发射
#define FUNC_BLUETOOTH  0//是否有蓝牙免提功能
	#define GPSNAME _T("G736A")
	#define GPSNAMELENGTH  5

//选择屏的分辨率
#define SCREEN_320_240   1
#define SCREEN_480_272   0

//音视频播放器编译时需修改CURAXPLAYER,并且修改编译链接名
//                =VIDEOPLAYER  同时需要修改LINK结果名为VPlayer.app
//                =AUDIOPLAYER  同时需要修改LINK结果名为APlayer.app
//AXPLAY播放器
#define AUDIOPLAYER 1
#define VIDEOPLAYER 2

#if 0//修改时别忘记修改目标文件名Project-setting-(Axplayer)link
#define CURAXPLAYER VIDEOPLAYER
#else
#define CURAXPLAYER AUDIOPLAYER //确定编译后是音频播放器还是视频播放器
#endif
//-----------------------------程序配置---结束-------------------------------------------

//-----------------------------程序中用到的常量---开始-----------------------------------
//这部分一般不用修改
//---------------------------------------------------------------------------------------
#ifdef _WIN32_WCE
#define SSCANF  swscanf
#else
#define SSCANF  sscanf
#endif

//--------------------------------------------------------------------------------------
//定义透明色
// 0 :表示没有透明色
//透明色必须是0颜色以外的颜色
#define TRANSCOLOR  0//整体截图没有透明色
#define TRANSCOLOR1 RGB(0xFF,0,0xFF) //某些控件必须透明色
#define INVALIDCOLOR  RGB(196,196,196)//控件无效状态时

//风格改变时，字体颜色可能需要改变
#define BLACKCOLOR RGB(0,0,0)
#define WHITECOLOR RGB(0xFF,0xFF,0xFF)
#define GREENCOLOR RGB(0,0xff,0)
//-----------------------------------------------------------------------------------
#define DEFAULT_GPSPATH "\\Storage Card\\Maps\\RTNavi\\RTNavi.exe"  //默认导航路径
#define DEFAULT_VOLUME 3 //默认音量
#define DEFAULT_BRIGHT 3 //默认亮度
#define DEFAULT_LANG ENG  //默认语言

#define BATTERY_STEPS                  5
#define BATTERY_ONESTEP				   22// = ((100-BATTERYLOWVALUE)/(BATTERY_STEPS-1))
#define BATTERYLOWVALUE				   15//判电池电量低的界限
#define BATTERYFULLVALUE			   100
#define BATTERYEMPTYVALUE			   0//电量空

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

//背光设备名
#define SPI_BRIGHT_DRIVER_NAME		_T("PWM1:")
#define I2C0_DRIVER_NAME		_T("I2C0:")

//-------------------------------------------------------------------------------------
#define FILELISTINDEXOFFSET    200  //防止不同应用程序用到该类时控件的序号重复
//--------------------------------------------------------------------------------------
//定义定时器时间
#ifdef _WIN32_WCE
#define CHECKDBLCLKTIME 200
#else
#define CHECKDBLCLKTIME 200
#endif

#define SLEEPRESETTIMERCOUNT    5000

#define PLAYTIMERID			1 //播放定时器ID
#define IDLERESETTIEMRID	2 //空闲时间复位定时器ID
#define DCLKCHECKID			3 //双击检测定时器ID
#define CHARGECHECKTIMERID	4 //电池电量定时器ID
#define CAPTURECHECKTIMERID 5 //光标获取检测定时器ID
#define APPCHECKTIMERID     6 //应用程序检测定时器ID
#define DATEREFRESHTIMERID  7 //日期刷新定时器ID
#define READYTIMERID        8 //操作前准备时间ID
#define REPEATTIMERID       9 //重复动作定时器ID

//---------------------------------------------------------------------------------------
//用户自定义消息
#define WM_USERBACKLIGHTONOFF	(WM_USER+0x200)
#define WM_USEREXITUSBMODE		(WM_USER+0x201)
#define WM_USERENTERUSBMODE     (WM_USER+0x202)
#define WM_GPSPLAYSOUND			(WM_USER+0x203)

//以下消息本来可以用一个（或较少几个）消息加不用的参数
//这样可以减少消息处理函数
//由于开始设计时分开了,就没有合成个一个消息，而是沿用了开始的设计思想
#define WM_USERCLK				(WM_USER+0x21F)
#define WM_USERDBLCLK			(WM_USER+0x220)
#define WM_USERPLAYCHANGE		(WM_USER+0x221)
#define WM_USERFOCUSITEMCHANGE	(WM_USER+0x222)
#define WM_USERDESTORYWINDOW	(WM_USER+0x223)
#define WM_USERPLAYERMSG		(WM_USER+0x224)//用于播放器动态连接链接库消息
#define WM_USERGPSRUN			(WM_USER+0x225)//GPS导航软件开始运行
#define WM_USERBTNDOWNREPEAT    (WM_USER+0x226)//按键按下重复消息
#define WM_USERLANGUAGECHANGE	(WM_USER+0x227)//语言改变
#define WM_USERMAINEXIT         (WM_USER+0x228)//主界面退出消息
#define WM_USERSTYLECHANGE		(WM_USER+0x229)//风格改变
#define WM_USERBTMSG_EVENT		(WM_USER+0x22A)//蓝牙模块事件消息
#define WM_USERBTWND_SHOW		(WM_USER+0x22B)
#define WM_USERMODULE			(WM_USER+0x22C)//模块开关等消息
	#define WPARA_FM			0x01
	#define WPARA_BT			0x02
//--------------------------------------------------------------------------------------
//定义注册表的键值
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
//定义应用程序HANDLE值和主窗口名
//各资源路径和资源过滤器
#ifndef FOURCC
#define FOURCC(a,b,c,d) \
	(((DWORD)(a) << 0) | ((DWORD)(b) << 8) | \
	((DWORD)(c) << 16) | ((DWORD)(d)<< 24))
#endif

//设置路径
#define STORRAGECARD "\\Storage Card" //存储卡路径
#define NANDFLASH     "\\ResidentFlash" 
#define GAPPOPTPATH   "\\ResidentFlash\\GOpt\\" 
#ifdef _WIN32_WCE
#define SEARCHPATH "\\Storage Card" //全搜索路径
#else
#define SEARCHPATH "D:\\WINCE_WORK\\Gps733\\Resource" //全搜索路径
#endif

#ifndef _WIN32_WCE  //PC上资源路径
#define SOURCEPATH  CString("D:\\WINCE_WORK\\Gps733\\Resource")
#endif

#ifdef _WIN32_WCE
#define OPTIONFILEPATH  (CString(GAPPOPTPATH)+CString("\\GpsUI.opt")) //配置文件路径
#else
#define OPTIONFILEPATH  "D:\\WINCE_WORK\\GApp2\\UI\\GpsUI.opt" //配置文件路径
#endif
#define COPYOPTIONFILEPATH "\\Storage Card\\GpsUI.opt" //拷贝配置文件路径（如果配置文件不存在，会查找该路径）


//电池电量低提示
#define SHOWHINT_PROGID FOURCC('H','I','N','T')
#define SHOWHINT_NAME  _T("Hint")
#define SHOWHINT_FILENAME "ShowHint.app"

//启动界面
#define APPMAIN_PROGID FOURCC('A','P','P','D')
#define APPMAIN_NAME   _T("GApp")

//如果用总的皮肤文件,定义宏SINGLESKINFILE
//#define SINGLESKINFILE   //用一张整图会花时间比较多
#define SKINFILENAME "D:\\WINCE_WORK\\GApp2\\UI\\Skin.jpg"
//地图
#define MAPUI_NAME _T("Map")
#define MAPUI_PROGID FOURCC('G','M','A','P')
//主界面
#define MAINUI_PROGID FOURCC('M','A','I','N')
#define MAINUI_NAME _T("GpsUI")
#ifdef SINGLESKINFILE
#define MAINUISKINFILE SKINFILENAME
#else
#define MAINUISKINFILE "D:\\WINCE_WORK\\GApp2\\UI\\Main.jpg"
#endif
#define MAINUI_FILENAME "GpsUI.app"

//设置界面
#define GSET_PROGID FOURCC('G','S','E','T')
#define GSET_NAME   _T("GSet")
#ifdef SINGLESKINFILE
#define GSETSKINFILE SKINFILENAME
#else
#define GSETSKINFILE "D:\\WINCE_WORK\\GApp2\\UI\\GSet.jpg"
#endif
#define GSET_FILENAME "GSet.app"

//图片浏览器
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

//TXT电子书
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

//通用对话框
#define COMDLGSKINFILE "D:\\WINCE_WORK\\GApp2\\UI\\PopUp.jpg" 

//蓝牙功能
#define BT_PROGID  FOURCC('B','T','_','_')
#define BT_NAME _T("TXTReader")
#define BLUETOOTHSKINFILE "D:\\WINCE_WORK\\GApp2\\UI\\BlueTooth.jpg" 
#define BLUETOOTH_NAME   _T("BlueTooth.app")

//-----------------------------------程序中用到的常量---结束------------------------------------------
//-----------------------------------------------------------------
//各控件属性数据结构
typedef enum _LanguageId
{
	ENG, //英语
	SHN, //简体中文
	CHN, //繁体中文

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
	RECT WndRect;//窗口位置、大小
	//如果需要叠加，则需要先获取该控件在整个背景图上的位置大小
	RECT WndGndRect;//窗口在整个背景图的位置、大小
	RECT NormalBmpRect;//窗口没有获得焦点时的位图所在位置、大小
	RECT FocusBmpRect;//窗口获得焦点时的位图所在位置、大小

	char LangText[LANG_NUM][200];//窗口包含文字的多国语言翻译
}WND_PROPERTY;

#endif
