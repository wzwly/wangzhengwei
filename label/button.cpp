#include "button.h"
#include <QPainter>
#include "./../ui/mainframe.h"

#define LABEL_HEIGHT 30

QPushBtn::QPushBtn(QWidget* parent_ , COLOR_GROUP nColor_, int nId_ ,unsigned int f_)
:QAbstractButton(parent_), QItem(nColor_)
{
        m_nId = nId_;
        m_bPushBtn = false;
        m_eSate = NORMAL_WIND;
         m_nFlag =  Qt::AlignVCenter |f_ | Qt::AlignJustify;
 }

void QPushBtn:: paintEvent(QPaintEvent *e_)
 {
    QPainter _p(this);
    _p.fillRect(0, 0, width(), height(), m_cBkColor[m_eSate]);
    _p.setPen(m_cFgColor[m_eSate]);
    QFont _font = _p.font();
    _font.setPointSize(m_nFontSize);
    _p.setFont (_font);
    _p.drawText(m_nBdWidth, m_nBdWidth,  width() - 2 * m_nBdWidth, height() - 2 * m_nBdWidth,  m_nFlag,  text());
   if (m_nBdWidth > 0)
    {
          QPen _pen;
         _p.setPen(m_cBdColor[m_eSate]);
         _pen = _p.pen();
         _pen.setWidth(m_nBdWidth);
         _p.setPen( _pen);
        _p.drawRoundedRect(0, 0,  width() - m_nBdWidth  , height() - m_nBdWidth,  RECT_RADIUS,  RECT_RADIUS);
    }
 }

//遇到按键或者鼠标点击。需要发送自己ID作为消息
void QPushBtn::mouseReleaseEvent(QMouseEvent* e_)
{
    emit ClickedEvent(m_nId);
}

void	QPushBtn::focusInEvent ( QFocusEvent * e )
{
    if ( m_bPushBtn)
        SetStatus(ACTIVE_WIND);
    else
        QAbstractButton::focusInEvent(e);
}

void	QPushBtn::focusOutEvent ( QFocusEvent * e )
{
    if ( m_bPushBtn)
        SetStatus( NORMAL_WIND);
    else
        QAbstractButton::focusOutEvent(e);
}


void QPushBtn::InitShow(int x_, int y_, int w_, int h_, int nFontsize_)
{
    move(x_, y_);
    setFixedSize(w_, h_);
    m_nFontSize = nFontsize_;
}

void QPushBtn::InitShow(const char* Text_, int x_, int y_, int w_, int h_,  int nFontsize_)
{
    setText(Text_);
    InitShow(x_, y_, w_, h_, nFontsize_);
}
//==========================================
//==========================================
QLabelBtn::QLabelBtn(QWidget* parent_ , COLOR_GROUP nColor_ , int nId_,unsigned int f_)
  : QPushBtn( parent_, nColor_, nId_, f_)
{
    m_pLabel  =new QTipLabel(this, nColor_);
    m_pValue  =new QTipLabel(this, nColor_);
}


void QLabelBtn::InitShowEx(int x_, int y_, int w_, int h_, int nFontsize_, int nP_)
{
        m_nFontSize = nFontsize_;
        m_pLabel->InitShow(1, 1, w_ * nP_ / 10,  h_ - 2, nFontsize_);
        m_pValue->InitShow(w_ * nP_ / 10, 1, w_ * (10 - nP_) / 10,  h_ - 2, nFontsize_);
        m_pLabel->SetBdWide(0);
        m_pValue->SetBdWide(0);
        move(x_, y_);
        setFixedSize(w_, h_);

}


//=========================================
 //===================自编的PushBtn=================
 QClickBtn::QClickBtn(QWidget* parent_ , int nId_ )
 :QPushButton(parent_)
 {
     m_nId = nId_;
     setStyleSheet("QPushButton {background-color:rgba(38,54,61); padding:0 pix; margin:0pix;border: 1px solid gray;\
                  font-size: 18px ; color:white;}QPushButton:pressed{background-color: rgba(85,95,85);}");
 }

 //遇到按键或者鼠标点击。需要发送自己ID作为消息
 void QClickBtn::mouseReleaseEvent(QMouseEvent* e_)
 {
     emit ClickedEvent(m_nId);
 }

 void QClickBtn::SetStyleSheet(const char* pAtrib_)
 {
     setStyleSheet(pAtrib_);
     update();
 }

 void QClickBtn::InitShow(int x_, int y_, int w_, int h_)
 {
     move(x_, y_);
     setFixedSize(w_, h_);
 }

 void QClickBtn::InitShow(const char* Text_, int x_, int y_, int w_, int h_)
 {
     move(x_, y_);
     setFixedSize(w_, h_);
     setText(Text_);
 }
