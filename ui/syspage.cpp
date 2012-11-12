#include "syspage.h"
#include "./../label/item.h"
#include "./../label/button.h"
#include "./../ui/mainframe.h"
#include <QProcess>

#define LED_W0 50
#define LED_W1 125
#define LED_H  50

#define LED_LABEL_W 600
#define LED_LABEL_H 60

QSysPage::QSysPage(QWidget* parent_)
  :QBasePage( parent_)
{  
    CreatePageInfo();
}

static const char*  g_szLimit[] = {"X-正限位", "Y正限位", "Z-正限位", "X-负限位", "Y-负限位", "Z-负限位"};

void QSysPage::CreatePageInfo()
{
   int _j = 0;
   QString _str;
   for (int _i = 0; _i < INPUT_IO_NUM; ++_i)
    {
        if (_i == 8)
            _j = 0;
            m_pIOInput[_i] = new QShape(this);
            _str = QString("IN%1").arg(_i);
            m_pIOInput[_i]->InitShow(147 + 70 * _j, _i < 8 ? 68 : 130, 60,60, 14);
            m_pIOInput[_i]->SetStatus(_i % 2  == 0  ? NORMAL_WIND : ACTIVE_WIND);
            m_pIOInput[_i]->setText(_str);
            _str =  QString("OUT%1").arg(_i);
            m_pIOOutput[_i] = new QShape(this);
            m_pIOOutput[_i]->InitShow(147 + 70 * _j, _i < 8 ? 250 : 315, 60,60, 14);
            m_pIOOutput[_i]->SetStatus(_i % 2  == 0  ? NORMAL_WIND : ACTIVE_WIND);
            m_pIOOutput[_i]->setText(_str);
            _j++;
    }

    for (int _i = 0; _i < LIMIT_IO_NUM / 2; ++_i)
    {
        m_pLimitPort[_i] = new QShape(this);
        m_pLimitPort[_i]->InitShow(g_szLimit[_i], 165 + _i * 200, 440, 125, 60, 16);
        m_pLimitPort[_i + 3] = new QShape(this);
        m_pLimitPort[_i + 3]->InitShow(g_szLimit[_i + 3], 165 + _i * 200, 505, 125, 60, 16);
     }


  m_pInputTip = new QTipLabel(this,QItem::LABEL_LIST);
  m_pInputTip->InitShow("输入指示点",127, 5, LED_LABEL_W, LED_LABEL_H, 18);
  m_pOutPutTip = new QTipLabel(this,QItem::LABEL_LIST);
  m_pOutPutTip->InitShow("输出指示点",127, 190, LED_LABEL_W, LED_LABEL_H, 18);
  m_pLinitTip = new QTipLabel(this,QItem::LABEL_LIST);
  m_pLinitTip->InitShow("限位状态",127, 375, LED_LABEL_W, LED_LABEL_H, 18);
}

void QSysPage::OnSndBtnClick(int nId_)
{
    if (5 == nId_) //触摸屏校准
    {
#if  ARM
        //m_pMainFrame->hide();
        QProcess::execute("rm /etc/pointercal");
        QProcess::execute("sync");
        //m_pMainFrame->show();
#endif
    }
}

/*void QSysPage::showEvent ( QShowEvent * event )
{
    QBasePage::showEvent (event );
    qDebug() <<"system page show";
}*/
