#include "mainframe.h"
#include <QtGui>
#include "./../ghead.h"
#include "./../label/item.h"
#include "./../label/button.h"
#include "./../core/qsysdata.h"
#include "./../core/cmddef.h"
#include "./../core/serial.h"

#include "basepage.h"
#include "autopage.h"
#include "drillparam.h"
#include "filepage.h"
#include "sysparam.h"
#include "syspage.h"


static const char* g_szMainMenu[MAIN_MENU_COUNT] = {"自动页面", "程序文件","钻孔参数", "系统参数", "系统监视"};

static const int g_nSndMenuQyt[MAIN_MENU_COUNT] = {3, 5, 4, 5, 4};  //二级菜单显示个数

static const char* g_szPageBtn0[SECOND_MENU_COUNT] = {"启动","暂停", "停止", "回原点", "仿真", "手动"};
static const char* g_szPageBtn1[SECOND_MENU_COUNT] = {"加载", "拷贝", "删除", "", "↑","↓"};
static const char* g_szPageBtn2[SECOND_MENU_COUNT] = {"保存文件", "恢复默认", "读取文件", "", "↑","↓"};
static const char* g_szPageBtn3[SECOND_MENU_COUNT] = {"系统参数", "速度参数", "限位参数", "视图参数","↑","↓",};
static const char* g_szPageBtn4[SECOND_MENU_COUNT] = {"","", "", "", "", "触屏校准"};

static const char* g_szPageBtn01[SECOND_MENU_COUNT] = {"步进","连续", "全部回原点", "单轴回原点", "设定原点", "自动"};
static const char* g_szPageBtn11[SECOND_MENU_COUNT] = {"","", "", "", "", ""};
static const char* g_szPageBtn21[SECOND_MENU_COUNT] = {"","","", "", "", ""};
static const char* g_szPageBtn31[SECOND_MENU_COUNT] = {"","","", "", "", ""};
static const char* g_szPageBtn41[SECOND_MENU_COUNT] = {"","", "", "", "", ""};

//typedef  const char* (*DEF_PARRAY_MENU_SND)[SECOND_MENU_COUNT];
static const char** g_zsSndMenu[MAIN_MENU_COUNT] = {g_szPageBtn0, g_szPageBtn1, g_szPageBtn2, g_szPageBtn3, g_szPageBtn4};
static const char** g_zsSndMenu1[MAIN_MENU_COUNT] = {g_szPageBtn01, g_szPageBtn11, g_szPageBtn21, g_szPageBtn31, g_szPageBtn41};

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

    QSerial* _p = QSerial::Instance(0);
    OnTimerUpdate();  //初始化时更新一次
    startTimer(1000);  //开启定时器，1s一次
}

QMainFrame::~QMainFrame()
{
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

     //m_pCurMenuPage->keyRelease(e_);
     m_bHasPressed = false;
}

 void QMainFrame::keyPressEvent ( QKeyEvent * e_)
 {
        //m_pCurMenuPage->keyPress(e_);
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

    m_pDateYMD = new QTipLabel(this, QItem::TITLE_LABEL);
    m_pDateYMD->InitShow("2010-06-25", 0, 0, 225,60, LABE_FONT_SIZE );
    m_pTitle = new QTipLabel(this,QItem::TITLE_LABEL);
    m_pTitle->InitShow("自动加工", 225, 0, 574,60, LABE_FONT_SIZE );
    m_pDateHMS = new QTipLabel(this,QItem::TITLE_LABEL);
    m_pDateHMS->InitShow("12:18:28", 799, 0, 225,60, LABE_FONT_SIZE );
}


//创建二级菜单
void QMainFrame::CreatePage()
{

    m_pMenuPage[0] = new QAutoPage(this);
    m_pMenuPage[1] = new QFilePage(this);
    m_pMenuPage[2] = new QDrillParamPage(this);
    m_pMenuPage[3] = new QSysParamPage(this);
    m_pMenuPage[4] = new QSysPage(this);

    for (int _i = 0; _i < 5; ++_i)
        m_pMenuPage[_i]->SetSysData(m_pSysData);
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
       qDebug() << "sel" <<m_nCurSndMenu << nIndex_;
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
      QString _str = _tTm.toString("yyyy-MM-dd w hh:mm:ss");
      m_pDateYMD->setText(_str.section("w", 0, 0));
      m_pDateHMS->setText(_str.section("w", 1, 1));
      //Frame更新
      OnTimerUpdate();
  }

  unsigned short g_wSend[5] = {1,2,3,4,5};
  unsigned short g_wRecive[5] = {1,2,3,4,5};

  //主界面定时更新函数
  void QMainFrame::OnTimerUpdate()
  {
        m_pCurMenuPage->OnUpDate(0);

        //CmdWriteKeepReg(1, 5,(unsigned char*)g_wSend);
        //Cmd03ReadKeepReg(2, 5,(unsigned char*)g_wRecive);
        //for(int _i = 0;_i < 5; ++_i)
        //    qDebug()<<g_wRecive[_i];
  }


  void QMainFrame::SetTiltleLabel(const QString& str_)
  {
      m_pTitle->setText(str_);
  }
