#ifndef   __ITEM_H__
#define  __ITEM_H__

#include "./../ghead.h"
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPointer>

struct  DataMap;

class QItem
{
public:
    enum COLOR_GROUP
    {
        INVALID_COLOR = -1,
        TITLE_LABEL, //上部控件
        MENU_BTN,    //菜单按钮
        SND_BTN,     //内部按钮
        LABEL_LED,   //灯开关
        LABEL_LIST,  //列表
        LABEL_TIP,   //显示Label
        LABEL_DLG,

    };
    QItem(COLOR_GROUP nColor_);
    ~ QItem();
protected:
    WIND_STATE m_eSate;
    QColor m_cFgColor[STATE_COUNT];
    QColor m_cBkColor[STATE_COUNT];
    QColor m_cBdColor[STATE_COUNT];
    int        m_nFontSize;
    int        m_nBdWidth;
    unsigned int m_nFlag;
    DataMap* m_pDataMap;
public:
    inline void SetFgColor(QColor cColor_,  WIND_STATE State_ = INVALID_STATE){State_ == INVALID_STATE ? m_cFgColor[m_eSate] = cColor_ : m_cFgColor[State_] = cColor_;}
    inline void SetBkColor(QColor cColor_,  WIND_STATE State_ = INVALID_STATE){State_ == INVALID_STATE ? m_cBkColor[m_eSate] = cColor_ : m_cBkColor[State_] = cColor_;}
    inline void SetBdColor(QColor cColor_,  WIND_STATE State_ = INVALID_STATE){State_ == INVALID_STATE ? m_cBdColor[m_eSate] = cColor_ : m_cBdColor[State_] = cColor_;}
    inline void SetColor(QColor cFg_, QColor cBk_, QColor cBd_, WIND_STATE State_ = INVALID_STATE);
    inline void SetBdWide(int nW_) {m_nBdWidth = nW_;}
    void SetDataMap(DataMap* pMap_) {m_pDataMap = pMap_;}
    DataMap* GetDataMap() {return m_pDataMap;}
    virtual void SetFontSize(int nF_) {m_nFontSize = nF_;}

    virtual void SetStatus(WIND_STATE State_) = 0;
    virtual void InitShow(int x_, int y_, int w_, int h_, int nFontsize_) = 0;
    virtual void InitShow(const char* Text_, int x_, int y_, int w_, int h_,  int nFontsize_) = 0;

private:
    void  InitColor(COLOR_GROUP nColor_);
};



 void QItem::SetColor(QColor cFg_, QColor cBk_, QColor cBd_, WIND_STATE State_)
 {
          if (State_ == INVALID_STATE)
              State_  = m_eSate;
          m_cFgColor[State_]  = cFg_;
          m_cBkColor[State_]  = cBk_;
          m_cBdColor[State_]  = cBd_;
 }

//=====================================
 class QTipLabel : public QItem , public QLabel
 {

 public:
        QTipLabel(QWidget * parent_ = 0, COLOR_GROUP nColor_ =  INVALID_COLOR ,unsigned int f_ = Qt::AlignHCenter);
        ~QTipLabel() {};
protected:
        virtual void paintEvent (QPaintEvent* e_);
public:
        virtual void SetStatus(WIND_STATE State_){m_eSate = State_; update();}
        virtual void InitShow(int x_, int y_, int w_, int h_, int nFontsize_);
        virtual void InitShow(const char* Text_, int x_, int y_, int w_, int h_,  int nFontsize_);
 };
//======================================

 class QShape: public QLabel ,public QItem
 {

      Q_OBJECT
 public:
       QShape(QWidget * parent_ = 0, COLOR_GROUP nColor_ =  LABEL_LED ,unsigned int f_ = Qt::AlignHCenter);
       ~QShape(){};
 public:
   virtual void SetStatus(WIND_STATE State_){m_eSate = State_; update();}
   virtual void InitShow(int x_, int y_, int w_, int h_, int nFontsize_);
   virtual void InitShow(const char* Text_, int x_, int y_, int w_, int h_,  int nFontsize_);
 protected:
   virtual void paintEvent (QPaintEvent* e_);

 private:
   QPainterPath m_drawPath;
 };

//=========================================
 class QPopTip
 {
 public:
     //time 为Tip显示的时间（ms）,time小于0显示时间无穷大，
     //直到主动调用ShowTextInMainframe或HideTextInMainframe才消失。
     static void ShowTextInMainframe(QString text, int fontSize=20, int time = 2000);
     static void HideTextInMainframe();
 private:
     static QPointer<QLabel> m_label;
 };

#endif // ITEM_H
