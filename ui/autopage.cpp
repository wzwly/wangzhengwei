#include "autopage.h"
#include "./../label/item.h"
#include "./../label/button.h"
#include "mainframe.h"
#include <QPainter>
#include "./../label/dlg.h"
#include "addrdef.h"
#include "./../core/cmddef.h"


#define XYZ_W0 50
#define XYZ_W1 125

#define LABEL_H 60
#define ROW_POS_W 175
#define OP_BTN_W  125
#define FONT_SIZE_AUTO 18

#define RULE_WIDTH  23

QAutoPage::QAutoPage(QWidget* parent_)
:QBasePage( parent_),m_nOrg(0,0)
{
    m_pDrawArea = new QDrawArea(this);
    m_pPaint = m_pDrawArea->GetPainter();
    m_pDrawArea->GetWidthHeight(m_nWidth,m_nHeight);
    CreatePageInfo();
    //m_pGlbCfg = m_pSysData->GetCfgData();
    m_fZoom = 1.0;
    m_fStep = 1.0;
    m_nAxisSel = 0;
}

void QAutoPage::DrawRuler()
{
    //m_pDrawArea->DrawRightRuler(m_pGlbCfg->fXoffsetView,m_pGlbCfg->fViewWidth + m_pGlbCfg->fXoffsetView,"mm");
    //m_pDrawArea->DrawBottomRuler(m_pGlbCfg->fYoffsetView,m_pGlbCfg->fViewHeight + m_pGlbCfg->fYoffsetView, "mm");
    m_pDrawArea->DrawRightRuler(0,1000,"mm");
    m_pDrawArea->DrawBottomRuler(0,1500, "mm");
}

static const char* g_szXYZ[] = {"X", "Y", "Z", "A", "B","U"};
static const char* g_szManuStep[2] = {"步长\nmm", "选择轴"};
static const char* g_szManuStepVal[2] = {"1.0", "X"};
static const char* g_szOpBtn[2] = {"←","→"};
void QAutoPage::CreatePageInfo()
{
    QTipLabel* _pTemp;
    for (int _i = 0; _i < AXIS_NUM ; ++_i)
    {
        _pTemp = new QTipLabel(this, QItem::LABEL_LIST);
        _pTemp->InitShow(g_szXYZ[_i], 0, 5  + (LABEL_H + 1) * _i, XYZ_W0, LABEL_H , FONT_SIZE_AUTO);

        m_pXYZCoor[_i] = new QTipLabel(this, QItem::LABEL_LIST);
        m_pXYZCoor[_i]->InitShow(XYZ_W0, 5  + (LABEL_H + 1) * _i, XYZ_W1, LABEL_H , FONT_SIZE_AUTO);
    }


    int _nID = 0;
    for (int _i = 0; _i < 2; ++_i)
    {
        m_pStepBtn[_i * 2] = new QPushBtn(this, QItem::LABEL_LIST, _nID++);
        m_pStepBtn[_i * 2]->InitShow(g_szManuStep[_i],0, 430  + (LABEL_H + 2) * _i, 90, LABEL_H, FONT_SIZE_AUTO);
        m_pStepBtn[_i * 2 + 1] = new QPushBtn(this, QItem::LABEL_TIP, _nID++);
        m_pStepBtn[_i * 2 + 1]->InitShow(g_szManuStepVal[_i],90, 430  + (LABEL_H + 2) * _i, 83, LABEL_H, FONT_SIZE_AUTO);
        m_pStepBtn[_i * 2 + 1]->SetBdWide(2);
        m_pStepOp[_i] = new QPushBtn(this, QItem::SND_BTN, _nID++);
        m_pStepOp[_i]->InitShow(g_szOpBtn[_i], _i * 88, 555 , 88, LABEL_H, FONT_SIZE_AUTO);
        connect(m_pStepBtn[_i * 2],SIGNAL(ClickedEvent(int)), this, SLOT(OnStepBtnClick(int)));
        connect(m_pStepBtn[_i * 2 + 1],SIGNAL(ClickedEvent(int)), this, SLOT(OnStepBtnClick(int)));
    }
}


void QAutoPage::showEvent ( QShowEvent * event )
{
    //if (m_pGlbCfg->bReDrawRule)
   // {
   //     DrawRuler();
    //    m_pGlbCfg->bReDrawRule = false;
    //    m_dDeltH = double(m_nHeight) / m_pGlbCfg->fViewHeight;
    //    m_dDeltW = double(m_nWidth) / m_pGlbCfg->fViewWidth;
   // }
    if (m_pSysData->IsLoadFile())
    {
        m_pMainFrame->SetTiltleLabel(QString("文件:%1").arg(m_pSysData->GetFileName()));
    }
    else
        m_pMainFrame->SetTiltleLabel(QString("自动加工"));

    QBasePage::showEvent (event );
}

// 0  1 [2]
// 3  4 [5]
const static float g_fStep[5] = {0.1, 1.0, 5.0, 10.0, 50.0};
const static char* g_szAxisSel[6] = {"X", "Y", "Z", "A", "B", "U"};
void QAutoPage::OnStepBtnClick(int nId_)
{
    if (0 == nId_)
    {
        static int _s_nSel = 1;
        _s_nSel = (++_s_nSel) % 5;
        m_fStep = g_fStep[_s_nSel];
        m_pStepBtn[1]->setText(QString::number(m_fStep, 'f', 2));
    }
    else if (1 == nId_)
    {
        QNuberInput _SoftKey(this);
        _SoftKey.SetRange(0.01, 500.0, m_fStep);

        if (!_SoftKey.exec())
                return;
        m_fStep = _SoftKey.GetDbData();
        m_pStepBtn[1]->setText(QString::number(m_fStep, 'f', 2));
    }
    else if(3 == nId_)
    {
        m_nAxisSel += 1;
        m_nAxisSel = m_nAxisSel % 6;
        m_pStepBtn[3]->setText(g_szAxisSel[m_nAxisSel]);
    }
    else if (4 == nId_)
    {
        m_nAxisSel -= 1;
        if(m_nAxisSel < 0)
            m_nAxisSel  = 5;
        m_pStepBtn[3]->setText(g_szAxisSel[m_nAxisSel]);
    }
    else if (2 == nId_) //负方向
    {

    }
    else if (5 == nId_)//正方向
    {

    }
}

void QAutoPage::OnSndBtnClick(int nId_)
{
    if (0 == nId_)
    {

    }
    else if (1 ==nId_)
    {

    }
    else if (2 == nId_)
    {

    }
    else if (3 == nId_)
    {
        DrawSimulate();
    }
    else if (5 == nId_)//自动
    {
        m_nSndBtnShow =  m_nSndBtnShow ? 0 : 1;
        m_pMainFrame->ReShowMenuBtn();
    }
}


void QAutoPage::OnViewBtnClick(int nId_)
{
    if (0 == nId_)//放大
    {
        m_fZoom += 1.0;
    }
    else if (1 ==nId_)
    {
        m_nOrg.y--;
    }
    else if (2 == nId_)//缩小
    {
        if (m_fZoom > 1.0)
                m_fZoom -= 1.0;
    }
    else if (3 == nId_)
    {
         m_nOrg.x--;
    }
    else if (4 == nId_)
    {
         m_nOrg.y++;
    }
    else
    {
         m_nOrg.x++;
    }
    DrawSimulate();
}

void QAutoPage::ClearArea()
{
    m_pDrawArea->ClearShape();
    DrawRuler();
}

void QAutoPage::DrawSimulate()
{
    ClearArea();

    QPainter* _p = m_pPaint;
    QPen _pen = _p->pen();
    _pen.setColor(QColor(200, 189, 98, COLOR_F));
    _pen.setWidth(1);
    _p->setPen(_pen);
     //_p->translate(m_nOrg.x, m_nOrg.y);
     //_p->scale( m_fZoom,  m_fZoom);

  /* int _x, _y;
   int _nRadius =  m_pSysData->m_cGlbData.fHoleRadius;
   QSysData::FLOAT_POINT _fP;
   for (int _i = 0; _i < m_pSysData->m_vDrillData.size(); ++_i)
   {
       _fP = m_pSysData->m_vDrillData[_i];
        _x = _fP.x * m_dDeltW;
        _y = _fP.y * m_dDeltH;
        _p->drawArc(_x, _y, _nRadius, _nRadius, 0.0, 360 * 16);
   }*/
   m_pDrawArea->update();
   //_p->scale( 1.0/m_fZoom,  1.0/m_fZoom);
   //_p->translate(-m_nOrg.x, -m_nOrg.y);
}


void QAutoPage::OnUpDate(int nFlag_)
{

    for(int _i = 0; _i < AXIS_NUM; ++_i)
    {
        //Cmd03ReadKeepReg(CTL_PARAM_ADDR::AXIS_X_POS,6,(unsigned char*)m_pGlbCfg->fAxisPos);
        //m_pXYZCoor[_i]->setText(QString::number(m_pGlbCfg->fAxisPos[_i], 'f', 2));
        //qDebug() <<"update!";

        //Cmd06WriteKeepReg(1, 0x55);
        const  char * _p = "1234567890\n00";
        //CmdWriteKeepReg(1, 11, (const unsigned char*)_p);
        //for(int _i = 0; _i < 100)
        static int _iCont = 0;
        _iCont++;
        if (_iCont > 100)
            _iCont = 0;
        CmdWriteKeepReg(_iCont, 4, (const unsigned char*)&_iCont);
    }
}







//===============================================
//===============================================

#define COLOR_BACK QColor(34, 68, 38, COLOR_F)

QDrawArea::QDrawArea(QWidget* p_)
:QWidget( p_)
{
    setFixedSize(680, 450);				//set size
    move(175, 0);
    setAutoFillBackground(true);
    this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    QPalette _pal = this->palette();	//set back ground
    _pal.setColor(QPalette::Background, COLOR_BACK);
    this->setPalette(_pal);

    m_pPixMap = new QPixmap(width(), height());
    m_pPixMap->fill(COLOR_BACK);
    m_pPaint = new QPainter(m_pPixMap);
}

void QDrawArea::paintEvent(QPaintEvent * e_)
{
    QPainter _p(this);
    _p.drawPixmap(0,0,*m_pPixMap);
}


void QDrawArea::ClearShape()
{
    //m_pPixMap->fill(COLOR_BACK);
    m_pPaint->fillRect(0,0,width() - RULE_WIDTH,height() - RULE_WIDTH,COLOR_BACK);
}

void QDrawArea::GetWidthHeight(int&w_,int&h_)
{
    w_ = width(); h_ = height();
}

//每个刻度实际显示的单位值
double QDrawArea::GetRuleInterval(const double& fMin_, const double& fMax_, const int& nLen_)
  {
         int _nMarkQty;
         double _fInterval;

         if(nLen_ >= 400)
             _nMarkQty = 100;
         else if (nLen_ >= 300)
             _nMarkQty = 80;
         else if (nLen_ >= 200)
             _nMarkQty = 50;
         else if (nLen_ >= 100)
             _nMarkQty = 30;
        else
            _nMarkQty = 10;

          _fInterval =  10.0 * (fMax_ - fMin_) / double(_nMarkQty);void SetWindTitle(QString& strTitle_);

          if (DB_L(_fInterval, 0.1)) //fInterval_ < 0.1
              _fInterval = 0.1;
          else if (DB_L(0.1, _fInterval) &&DB_L(_fInterval, 1.0))
              _fInterval =  (int(_fInterval * 10.0)) / 10.0;
          else if (DB_GE(_fInterval, 1.0) && DB_LE(_fInterval, 5.0))
              _fInterval = int(_fInterval);
          else if (DB_G(_fInterval, 5.0) && DB_LE(_fInterval, 10.0))
              _fInterval = 10.0;
          else if (DB_G(_fInterval, 10.0) && DB_L(_fInterval, 50.0))
               _fInterval = 10.0 * (1 + int(_fInterval / 10.0));
          else if (DB_GE(_fInterval, 50.0) && DB_LE(_fInterval, 100.0))
              _fInterval = 100.0;
         else if (DB_G(_fInterval, 100.0) && DB_LE(_fInterval, 200.0))
              _fInterval = 200.0;
         else if (DB_G(_fInterval, 200.0))
             _fInterval = 100.0 * (1 + int(_fInterval / 100.0));

         _fInterval /= 10.0;

         return _fInterval;
  }


//绘制右侧标尺
void QDrawArea::DrawRightRuler(const double fMin_, const double fMax_, QString strUnit_)
{
      QColor color_(200, 200, 0, COLOR_F);
      double  _dbInterval;
      int _nLen = height() - RULE_WIDTH;
      int _nX = 0, _nY = 0;
      bool bUp_ = true;

      _nY = bUp_ ? _nLen : 0;
      _nX = width() - RULE_WIDTH;
      _dbInterval = GetRuleInterval(fMin_, fMax_, _nLen);//每个刻度实际显示的单位值

      double _dbPixmm = double(_nLen) / (fMax_ - fMin_);
      int _nPos = int (fMin_ /  _dbInterval);

      if (bUp_)
          _dbPixmm *= -1.0;

      if(fMin_ > 0)
          ++_nPos;

      double _dbCurPos = _nPos * _dbInterval;
      int _k;
      bool _bFirst = bUp_;

      QPen _pen = m_pPaint->pen();
      _pen.setColor(QColor(0, 1, 1, COLOR_F));
      _pen.setWidth(1);
      m_pPaint->setPen(_pen);

      m_pPaint->fillRect(_nX, 0, RULE_WIDTH , _nLen, color_);


      while(_dbCurPos < fMax_)
      {
               _k = _nY + (_dbCurPos - fMin_) * _dbPixmm;
               if (_nPos % 10 == 0)
               {
                      m_pPaint->drawLine(_nX, _k, _nX + 12, _k);
                      m_pPaint->translate(_nX, _k);
                      m_pPaint->rotate(270);
                      if (_bFirst)
                      {
                              if (DB_EZ(_dbCurPos) )
                                      m_pPaint->drawText(7, 20, QString::number(_dbCurPos));

                             m_pPaint->drawText(14, 20, strUnit_);
                           _bFirst = false;
                      }
                      else
                            m_pPaint->drawText(7, 20, QString::number(_dbCurPos));

                      m_pPaint->resetTransform();
               }
               else if (_nPos % 5 == 0)
               {
                      m_pPaint->drawLine(_nX, _k, _nX + 8, _k);
                      if (_bFirst)
                      {
                            m_pPaint->drawLine(_nX, _k, _nX + 12, _k);
                            m_pPaint->translate(_nX, _k);
                            m_pPaint->rotate(270);
                           if (_bFirst)
                                 m_pPaint->drawText(7, 14, strUnit_);
                            _bFirst = false;
                             m_pPaint->resetTransform();
                      }
               }
               else
               {
                      m_pPaint->drawLine(_nX, _k, _nX + 4, _k);
               }
              ++_nPos;
               _dbCurPos = _nPos * _dbInterval;
      }

      if (!bUp_)
      {
              m_pPaint->translate(_nX, _k);
              m_pPaint->rotate(270);
              m_pPaint->drawText(14, 20, strUnit_);
              m_pPaint->resetTransform();
      }
}


void QDrawArea::DrawBottomRuler(const double fMin_, const double fMax_, QString strUnit_)
{
    double  _dbInterval;
    int _nLen = width()- RULE_WIDTH;
    int _nX = 0, _nY = 0;
    bool bUp_ = true;
    QColor color_(200, 200, 0, COLOR_F);

    _nX =  0;//
    _nY = height() - RULE_WIDTH;

    _dbInterval = GetRuleInterval(fMin_, fMax_, _nLen);//每个刻度实际显示的单位值

    double _dbPixmm = double(_nLen) / (fMax_ - fMin_);
    int _nPos = int (fMin_ /  _dbInterval);

    if (!bUp_)
        _dbPixmm *= -1.0;

    if(fMin_ > 0)
        ++_nPos;

    double _dbCurPos = _nPos * _dbInterval;
    int _k;
    bool _bFirst = true;

    m_pPaint->fillRect(0,  _nY, _nLen, RULE_WIDTH, color_);
    QPen _pen = m_pPaint->pen();
    _pen.setColor(QColor(0, 1, 1, COLOR_F));
    _pen.setWidth(1);
    m_pPaint->setPen(_pen);

    while(_dbCurPos < fMax_)
    {
             _k =  _nX + (_dbCurPos - fMin_) * _dbPixmm;
             if (_nPos % 10 == 0)
             {
                    m_pPaint->drawLine(_k, _nY, _k, _nY + 12);
                    if (_bFirst)
                    {
                            if (DB_EZ(_dbCurPos) )
                                    m_pPaint->drawText(_k + 1, _nY + 20, QString::number(_dbCurPos));

                           m_pPaint->drawText(_k + 10, _nY + 20, strUnit_);
                         _bFirst = false;
                    }
                    else
                            m_pPaint->drawText(_k + 1, _nY + 20, QString::number(_dbCurPos));
             }
             else if (_nPos % 5 == 0)
             {
                    m_pPaint->drawLine(_k, _nY, _k, _nY + 8);
                    if (_bFirst)
                    {
                             m_pPaint->drawText(_k - 2, _nY + 20, strUnit_);
                             _bFirst = false;
                    }
             }
             else
             {
                    m_pPaint->drawLine(_k, _nY, _k, _nY + 4);
             }
            ++_nPos;
             _dbCurPos = _nPos * _dbInterval;
    }
}
