#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "windows.h"
#include "GfxPresent.h"
#include <iostream>
#include <QFile>
#include <qtextstream.h>

#ifndef iviDEFINE_GUID

#ifdef __GNUG__  /* gcc/g++ compiler (Linux, Symbian ARMI) does the equivalent of _declspec(selectany) */
#define ATTRIBUTE_WEAK     __attribute__ ((weak))
#define DECLSPEC_SELECTANY
#elif defined(__ADS__)
#define ATTRIBUTE_WEAK
#define DECLSPEC_SELECTANY
#else
#define ATTRIBUTE_WEAK
#if !defined(DECLSPEC_SELECTANY)
#define DECLSPEC_SELECTANY _declspec(selectany)
#endif
#endif

        #define iviDEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
                 EXTERN_C DECLSPEC_SELECTANY const GUID FAR name ATTRIBUTE_WEAK \
                = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

#endif

iviDEFINE_GUID(EMC_PROPSET_GFXCTXT,0x9eccdcca, 0xeb44, 0x11d4, 0xb3, 0x65, 0x00, 0x01, 0x02, 0x2a, 0x4a, 0x87);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QMainWindow* m_hwnd = new QMainWindow;

    IMcVideoPresenter* pPresneter = NULL;
    pPresneter = CreateVideoPresenter();

    VP_OpenOptions opt;
    memset(&opt, 0 ,sizeof(opt));
    opt.dwWidth = 352;
    opt.dwHeight= 288;
    opt.dwFourCC = MAKE_FOURCC('Y','V','1','2');
    opt.hWnd = this->winId();
    pPresneter->Open(&opt, sizeof(opt));

    RECT Dstrect;
    Dstrect.left = 0;
    Dstrect.right= 352;
    Dstrect.top = 0;
    Dstrect.bottom = 288;

    pPresneter->Set(EMC_PROPSET_GFXCTXT,MC_PROPID_VP_RECTDST,0,0,&Dstrect,0);
    char* pData = NULL;

    QFile* file = new QFile(".\test.cif");
    DWORD m_dwStrides[3];
    LPBYTE curr[3];
    m_dwStrides[0] = 352;
    m_dwStrides[1] = m_dwStrides[2] = 352>>1;

#if 1
    FILE m_file;
    file->open(&m_file, QIODevice::ReadOnly);
    fseek(&m_file,0,SEEK_END);
    DWORD m_FileLength = ftell(&m_file);
    fseek(&m_file, 0 , SEEK_SET);
#endif
    pData = new char[352*288*3/2];
    memset(pData, 0xff, sizeof(char)*352*288*3/2);
    while(1)
    {

      // fread(pData,sizeof(BYTE),352*288*3/2*sizeof(char),&m_file);

        curr[0] = &pData[0];
        curr[1] = &pData[352*288];
        curr[2] = &pData[352*288*5/4];

        pPresneter->Present(curr, m_dwStrides, 0);
    }

    pPresneter->Close();
    delete pPresneter;
}
