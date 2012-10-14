#ifndef BUTTON_H
#define BUTTON_H

#include <QPushButton>
#include  <QSlider>;
#include "item.h"

//可点击的Btn
class QPushBtn : public QAbstractButton, public QItem
{
    Q_OBJECT
public:
    QPushBtn(QWidget* parent_ = 0, COLOR_GROUP nColor_ =  INVALID_COLOR, int nId_ = 0,unsigned int f_ = Qt::AlignHCenter);
    ~QPushBtn(){};

protected:
    int   m_nId;
    bool m_bPushBtn;
public slots:
    signals:
    void ClickedEvent(int nId_);//用于发送消息
protected:
    virtual void mouseReleaseEvent(QMouseEvent* e_);
    virtual void	focusInEvent ( QFocusEvent * e );
    virtual void	focusOutEvent ( QFocusEvent * e );
    virtual void paintEvent(QPaintEvent *e_);
public:
    void SetAsPushBtn(bool bSet_ = true){m_bPushBtn = bSet_;}
    virtual void SetStatus(WIND_STATE State_){m_eSate = State_; update();}
    virtual void InitShow(int x_, int y_, int w_, int h_, int nFontsize_);
    virtual void InitShow(const char* Text_, int x_, int y_, int w_, int h_,  int nFontsize_);
};


class QLabelBtn: public QPushBtn
{
         Q_OBJECT
public:
        QLabelBtn(QWidget* parent_ = 0, COLOR_GROUP nColor_ =  INVALID_COLOR, int nId_ = 0,unsigned int f_ = Qt::AlignHCenter);
        ~QLabelBtn(){};
public:
        // virtual void InitShow(int x_, int y_, int w_, int h_, int nFontsize_);
         void InitShowEx(int x_, int y_, int w_, int h_, int nFontsize_, int nP_);
         void SetLabel(const char* Text_){m_pLabel->setText(Text_);}
         void SetValue(const char* Text_){m_pValue->setText(Text_);}
         void SetLabel(const QString& Text_){m_pLabel->setText(Text_);}
         void SetValue(const QString& Text_){m_pValue->setText(Text_);}
private:
         QTipLabel* m_pLabel;
         QTipLabel* m_pValue;
};



//可点击的Btn
///======================================
class QClickBtn : public QPushButton
{
    Q_OBJECT

public:
    QClickBtn(QWidget* parent_ = 0, int nId_ = 0);
    ~QClickBtn(){};

protected:
    int   m_nId;
public slots:
    signals:
    void ClickedEvent(int nId_);//用于发送消息
protected:
    virtual void mouseReleaseEvent(QMouseEvent* e_);
public:
    void SetStyleSheet(const char* pAtrib_);
    void InitShow(int x_, int y_, int w_, int h_);
    void InitShow(const char* Text_, int x_, int y_, int w_, int h_);
};
#endif // BUTTON_H
