#include "mainframe.h"
#include <QtGui>
#include "./../ghead.h"
#include "./../label/label.h"
#include "./../core/qsysdata.h"
#include "./../core/cmddef.h"
#include "./../core/serial.h"

#include "basepage.h"
#include "autopage.h"
#include "drillparam.h"
#include "filepage.h"
#include "sysparam.h"
#include "syspage.h"


static const char* g_szMainMenu[MAIN_MENU_COUNT] = {"自动页面", "程序文件", "系统参数", "系统监视"};
static const int g_nSndMenuQyt[MAIN_MENU_COUNT] = {3, 5, 4, 5};  //二级菜单显示个数

static const char* g_szPageBtn0[SECOND_MENU_COUNT] = {"启动","暂停", "停止", "回原点", "仿真", "手动"};
static const char* g_szPageBtn1[SECOND_MENU_COUNT] = {"加载", "卸载", "拷贝到U盘", "拷贝到系统", "删除","编辑"};
static const char* g_szPageBtn2[SECOND_MENU_COUNT] = {"系统参数", "速度参数", "限位参数", "视图参数","上页","下页"};
static const char* g_szPageBtn3[SECOND_MENU_COUNT] = {"","", "", "", "", "触屏校准"};

static const char* g_szPageBtn01[SECOND_MENU_COUNT] = {"步进","连续", "全部回原点", "单轴回原点", "设定原点", "自动"};
static const char* g_szPageBtn11[SECOND_MENU_COUNT] = {"编辑","新建","", "", "保存", "返回"};
static const char* g_szPageBtn21[SECOND_MENU_COUNT] = {"","","", "", "", ""};
static const char* g_szPageBtn31[SECOND_MENU_COUNT] = {"","", "", "", "", ""};

//typedef  const char* (*DEF_PARRAY_MENU_SND)[SECOND_MENU_COUNT];
static const char** g_zsSndMenu[MAIN_MENU_COUNT] = {g_szPageBtn0, g_szPageBtn1, g_szPageBtn2, g_szPageBtn3};
static const char** g_zsSndMenu1[MAIN_MENU_COUNT] = {g_szPageBtn01, g_szPageBtn11, g_szPageBtn21, g_szPageBtn31};

#define BTN_MENU_H  90
#define BTN_MENU_W  170
#define BTN_MENU_Y  768 - BTN_MENU_H
#define BTN_MENU_X  1024 - BTN_MENU_W

#define LABE_FONT_SIZE  18

QList<QObject*> g_pListObject;
static QMainFrame* g_pMainFrame = NULL;

QMainFrame* GetMainFrame() {return g_pMainFrame;}

QMainFrame::QMainFrame(QWidget *parent) :
    QMainWindow(parent)
{
    setFixedSize (1024, 768);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    move(0, 0);
    QPalette _pal = this->palette();
    _pal.setColor(QPalette::Background, QColor(150, 150, 150));
    this->setPalette(_pal);

    m_eLang = LANG_CHN;
    m_nCurMainMenu = 0;
    m_nCurSndMenu = 0;
    g_pMainFrame = this;
    m_bSndBtnActive = true;
    m_bHasPressed = false;

    m_pSysData = QSysData::Instance();
    CreateMainMenu();
    CreatePage();

    QSerial* _pCom = new QSerial(this);
    m_pModbus = new DevMaster(1, _pCom);
    _pCom->SetModbus(m_pModbus);
    m_pSysData->SetModbus(m_pModbus);
    startTimer(1000);  //开启定时器，1s一次
}

QMainFrame::~QMainFrame()
{
    delete m_pModbus;
    for ( QList<QObject*>::iterator _iter = g_pListObject.begin (); _iter != g_pListObject.end(); ++_iter)
        delete (*_iter);

    delete m_pSysData;

}


void QMainFrame::keyReleaseEvent (QKeyEvent * e_)
{
    if (e_->key() == Qt::Key_Escape)
        close();
    else if (m_bHasPressed ||    (e_->key() == Qt::Key_Up || e_->key() == Qt::Key_Down
                                  || e_->key() == Qt::Key_Left || e_->key() == Qt::Key_Right))

   m_bHasPressed = false;
}

void QMainFrame::keyPressEvent ( QKeyEvent * e_)
{
    m_bHasPressed = true;
}

void QMainFrame::CreateMainMenu()
{

    //左侧创建
    for (int _i = 0; _i < MAIN_MENU_COUNT; ++_i)
    {
        m_pPageMenu[_i] = new QPushBtn(this, QItem::MENU_BTN, _i);
        m_pPageMenu[_i]->InitShow(g_szMainMenu[_i],BTN_MENU_X, 60 + _i * BTN_MENU_H, BTN_MENU_W, BTN_MENU_H, MENU_FONT_SIZE);
        connect(m_pPageMenu[_i], SIGNAL(ClickedEvent(int)), this, SLOT(OnMainMenuShow(int)));
    }
    //创建底部
    for (int _i = 0; _i < SECOND_MENU_COUNT; ++_i)
    {
        m_pScdMenuBtn[_i] = new QPushBtn(this, QItem::MENU_BTN, _i);
        m_pScdMenuBtn[_i]->InitShow(_i * (BTN_MENU_W + 1),BTN_MENU_Y, BTN_MENU_W, BTN_MENU_H, MENU_FONT_SIZE);
        connect(m_pScdMenuBtn[_i], SIGNAL(ClickedEvent(int)), this, SLOT(OnSndMenuBtn(int)));
    }

    m_pPageInfo = new QTipLabel(this, QItem::TITLE_LABEL);
    m_pPageInfo->InitShow("自动加工", 0, 0, 250,60, LABE_FONT_SIZE );
    m_pWarning = new QTipLabel(this,QItem::TITLE_LABEL);
    m_pWarning->InitShow("", 250, 0, 549,60, LABE_FONT_SIZE );
    m_pDateHMS = new QTipLabel(this,QItem::TITLE_LABEL);
    m_pDateHMS->InitShow("12:18:28", 774, 0, 250,60, LABE_FONT_SIZE );
}


//创建二级菜单
void QMainFrame::CreatePage()
{
    m_pMenuPage[0] = new QAutoPage(this);
    m_pMenuPage[1] = new QFilePage(this);
   // m_pMenuPage[2] = new QDrillParamPage(this);
    m_pMenuPage[2] = new QSysParamPage(this);
    m_pMenuPage[3] = new QSysPage(this);
    for (int _i = 0; _i < MAIN_MENU_COUNT; ++_i)
    {
        m_pMenuPage[_i]->SetModbus(m_pModbus);
        m_pMenuPage[_i]->SetSysData(m_pSysData);
    }
    m_pCurMenuPage = m_pMenuPage[0];
    ChangeSndMenuText(0);
}

void QMainFrame::OnMainMenuShow(int nId_)//响应 m_pMainMenu
{
    if (m_nCurMainMenu == nId_ || !m_bSndBtnActive)
        return;
    m_pPageMenu[m_nCurMainMenu]->SetStatus(NORMAL_WIND);
    m_nCurMainMenu = nId_;
    m_pPageMenu[m_nCurMainMenu]->SetStatus(ACTIVE_WIND);
    ChangeSndMenuText(nId_);
}


void QMainFrame::OnSndMenuBtn(int nId_)
{

    if (m_nCurSndMenu == nId_ || !m_bSndBtnActive)
    {
        m_pCurMenuPage->OnSndBtnClick(nId_); //传给下三级菜单窗口处理
        return;
    }

    m_pScdMenuBtn[m_nCurSndMenu]->SetStatus(NORMAL_WIND);
    m_nCurSndMenu = nId_;
    m_pScdMenuBtn[m_nCurSndMenu]->SetStatus(ACTIVE_WIND);
    m_pCurMenuPage->OnSndBtnClick(nId_); //传给下三级菜单窗口处理
}


void QMainFrame::ChangeSndMenuText(int nIndex_)
{
    m_pScdMenuBtn[m_nCurSndMenu]->SetStatus(NORMAL_WIND);
    m_nCurSndMenu  = m_pCurMenuPage->GetSndMenuPos();
    m_pScdMenuBtn[m_nCurSndMenu]->SetStatus(ACTIVE_WIND);
    m_pCurMenuPage->hide();
    m_pCurMenuPage = m_pMenuPage[nIndex_];
    m_pCurMenuPage->show();

    ReShowMenuBtn();
}

void QMainFrame::ReShowMenuBtn()
{
    if(m_pCurMenuPage->GetSndBtnShow() == 0)
    {
        for (int _i = 0; _i < SECOND_MENU_COUNT; ++_i)
            m_pScdMenuBtn[_i]->setText(g_zsSndMenu[m_nCurMainMenu][_i]);
    }
    else
    {
        for (int _i = 0; _i < SECOND_MENU_COUNT; ++_i)
            m_pScdMenuBtn[_i]->setText(g_zsSndMenu1[m_nCurMainMenu][_i]);
    }
}

//定时器响应函数
void QMainFrame::timerEvent(QTimerEvent *event_)
{
    QDateTime _tTm = QDateTime::currentDateTime();
    QString _str = _tTm.toString("yyyy-MM-dd hh:mm:ss");
    m_pDateHMS->setText(_str);
    //Frame更新
    m_pCurMenuPage->OnUpDate(0);
    ShowErroInfo();

#if 0
    static int _s_n = 0;
    switch(_s_n)
    {
    case 0:
        m_pModbus->ReadCoil(0x10, 16);
        _s_n = 1;
        break;
    case 1:
        m_pModbus->ReadRegisters(0x10, 10);
        _s_n = 2;
        break;
    case 2:
        m_pModbus->ForceSingleCoil(0x10, 1);
        m_pModbus->ForceSingleCoil(0x11, 0);
        _s_n = 3;
        break;
    case 3:
        m_pModbus->PresetSingleRegister(0x10, 0x27ae);
        _s_n = 5;
        break;
    case 4:
    {
        unsigned char _out[5] = {0x33, 0xd7, 0xc8, 0xb9, 0xaa};
        m_pModbus->ForceMultipleCoils(0x10, 8, _out);
        _s_n = 5;
    }
        break;
    case 5:
    {
        unsigned short _outReg[5] = {0xee33, 0xccd7, 0xddc8, 0xffb9, 0x45aa};
        m_pModbus->PresetMultipleRegisters(0x10, 5, (unsigned char*)_outReg);
        _s_n = 0;
    }
        break;
    }
#endif
}



void QMainFrame::SetInfoItem(const QString& str_)
{
    m_pPageInfo->setText(str_);
}

void QMainFrame::SetWarnningItem(const QString& str_)
{
    m_pWarning->setText(str_);
}

void QMainFrame::LogCommucateErro(const ERRO_LOG& Erro_)
{
    m_pErroList.push_back(Erro_);
}

void QMainFrame::ShowErroInfo()
{
    static int _n_ShowIndex = 0;
    int _nSize = m_pErroList.size();
    if (_nSize <= 0)
    {   m_pWarning->setText(""); return;}

    if (_n_ShowIndex >= _nSize)
        _n_ShowIndex = 0;

    ERRO_LOG& _Erro = m_pErroList[_n_ShowIndex];
    QString _str;

    _Erro.iTimer--;
    switch(_Erro.cCmd)
    {
    case 1://读取线圈状态(读取点 16位以内)
    case 3://读取保持寄存器(一个或多个)
    case 5://强制单个线圈
    case 6://设置单个寄存器
    case 15://设置多个线圈
    case 16://设置多个寄存器
        _str = QString("通信出错：命令%1 地址%2").arg(int(_Erro.cCmd)).arg(int(_Erro.iAddr));
        break;
    default:
        break;
    }
    if (_Erro.iTimer <= 0)
        m_pErroList.remove(_n_ShowIndex);
    _n_ShowIndex++;
    m_pWarning->setText(_str);
}
