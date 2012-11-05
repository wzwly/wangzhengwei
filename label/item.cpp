#include "item.h"
#include <QPainter>
#include <QFocusEvent>

static QColor TITLE_LABEL_FGCOLOR[] = {QColor(50, 43, 87, COLOR_F), QColor(50, 43, 87, COLOR_F)};
static QColor TITLE_LABEL_BKCOLOR[] = {QColor(62, 187, 124, COLOR_F), QColor(177, 193, 155, COLOR_F)};
static QColor TITLE_LABEL_BDCOLOR[] = {QColor(177, 193, 155, COLOR_F),  QColor(177, 193, 155, COLOR_F)};


static QColor MENU_BTN_FGCOLOR[] = {QColor(250, 250, 250, COLOR_F), QColor(250, 250, 250, COLOR_F)};
static QColor MENU_BTN_BKCOLOR[] = {QColor(35, 38, 150, COLOR_F), QColor(0, 0, 255, COLOR_F)};
static QColor MENU_BTN_BDCOLOR[] = {QColor(100, 79, 90, COLOR_F),  QColor(100, 79, 90, COLOR_F)};

static QColor SND_BTN_FGCOLOR[] = {QColor(250, 250, 250, COLOR_F), QColor(250, 250, 0, COLOR_F)};
static QColor SND_BTN_BKCOLOR[] = {QColor(20, 105, 162, COLOR_F), QColor(0, 64, 94, COLOR_F)};
static QColor SND_BTN_BDCOLOR[] = {QColor(200, 200, 200, COLOR_F),  QColor(255, 206, 60, COLOR_F)};

static QColor LABEL_LED_FGCOLOR[] = {QColor(0, 0, 0, COLOR_F), QColor(0, 0, 0, COLOR_F)};
static QColor LABEL_LED_BKCOLOR[] = {QColor(0, 255, 0, COLOR_F), QColor(255, 0, 0, COLOR_F)};
static QColor LABEL_LED_BDCOLOR[] = {QColor(0, 0, 0, COLOR_F),  QColor(0, 0, 0, COLOR_F)};

static QColor LABEL_LIST_FGCOLOR[] = {QColor(0, 0, 0, COLOR_F), QColor(255, 255, 0, COLOR_F)};
static QColor LABEL_LIST_BKCOLOR[] = {QColor(0, 204, 255, COLOR_F), QColor(83, 143, 172, COLOR_F)};
static QColor LABEL_LIST_BDCOLOR[] = {QColor(20, 20, 20, COLOR_F),  QColor(20, 20, 20, COLOR_F)};



static QColor LABEL_TIP_FGCOLOR[] = {QColor(255, 0, 0, COLOR_F), QColor(255, 0, 0, COLOR_F)};
static QColor LABEL_TIP_BKCOLOR[] = {QColor(51, 204, 204, COLOR_F), QColor(30, 117, 117, COLOR_F)};
static QColor LABEL_TIP_BDCOLOR[] = {QColor(20, 20, 20, COLOR_F),  QColor(20, 20, 20, COLOR_F)};

static QColor LABLE_DLG_FGCOLOR[] = {QColor(50, 43, 87, COLOR_F), QColor(50, 43, 87, COLOR_F)};
static QColor LABLE_DLG_BKCOLOR[] = {QColor(177, 193, 155, COLOR_F), QColor(177, 193, 155, COLOR_F)};
static QColor LABLE_DLG_BDCOLOR[] = {QColor(177, 193, 155, COLOR_F),  QColor(177, 193, 155, COLOR_F)};




QItem::QItem(COLOR_GROUP nColor_)
{
        InitColor(nColor_);
        m_nBdWidth = 1;
        m_pDataMap = NULL;
}

QItem::~QItem()
{
}

void  QItem::InitColor(COLOR_GROUP nColor_)
{
       if (TITLE_LABEL == nColor_)
       {
            for (int _i = 0; _i < STATE_COUNT; ++_i)
            {
                m_cFgColor[_i] = TITLE_LABEL_FGCOLOR[_i];
                m_cBkColor[_i] = TITLE_LABEL_BKCOLOR[_i];
                m_cBdColor[_i] = TITLE_LABEL_BDCOLOR[_i];
            }
        }
       else if (MENU_BTN == nColor_)
        {
            for (int _i = 0; _i < STATE_COUNT; ++_i)
            {
                m_cFgColor[_i] = MENU_BTN_FGCOLOR[_i];
                m_cBkColor[_i] = MENU_BTN_BKCOLOR[_i];
                m_cBdColor[_i] = MENU_BTN_BDCOLOR[_i];
            }
        }
        else if (SND_BTN == nColor_)
        {
            for (int _i = 0; _i < STATE_COUNT; ++_i)
            {
                m_cFgColor[_i] = SND_BTN_FGCOLOR[_i];
                m_cBkColor[_i] = SND_BTN_BKCOLOR[_i];
                m_cBdColor[_i] = SND_BTN_BDCOLOR[_i];
            }
        }
       else if (LABEL_LED == nColor_)
       {
            for (int _i = 0; _i < STATE_COUNT; ++_i)
            {
                m_cFgColor[_i] = LABEL_LED_FGCOLOR[_i];
                m_cBkColor[_i] = LABEL_LED_BKCOLOR[_i];
                m_cBdColor[_i] = LABEL_LED_BDCOLOR[_i];
            }
        }
       else if (LABEL_LIST == nColor_)
       {
            for (int _i = 0; _i < STATE_COUNT; ++_i)
            {
                m_cFgColor[_i] = LABEL_LIST_FGCOLOR[_i];
                m_cBkColor[_i] = LABEL_LIST_BKCOLOR[_i];
                m_cBdColor[_i] = LABEL_LIST_BDCOLOR[_i];
            }
        }
        else if (LABEL_TIP == nColor_)
        {
            for (int _i = 0; _i < STATE_COUNT; ++_i)
            {
                m_cFgColor[_i] =  LABEL_TIP_FGCOLOR[_i];
                m_cBkColor[_i] =  LABEL_TIP_BKCOLOR[_i];
                m_cBdColor[_i] =  LABEL_TIP_BDCOLOR[_i];
            }
        }
        else if ( LABEL_DLG == nColor_)
        {
            for (int _i = 0; _i < STATE_COUNT; ++_i)
            {
                m_cFgColor[_i] =  LABLE_DLG_FGCOLOR[_i];
                m_cBkColor[_i] =  LABLE_DLG_BKCOLOR[_i];
                m_cBdColor[_i] =  LABLE_DLG_BDCOLOR[_i];
            }
        }
}


//=============================================
QTipLabel::QTipLabel(QWidget * parent_, COLOR_GROUP nColor_, unsigned int f_ )
    :QItem(nColor_), QLabel(parent_)
{
        m_nFlag =  f_ | Qt::AlignVCenter | Qt::AlignJustify ;
        m_eSate = NORMAL_WIND;
}


void QTipLabel::paintEvent (QPaintEvent* e_)
{
    QPainter _p(this);
    QFont _font = _p.font();
    _font.setPointSize(m_nFontSize);
    _p.setFont(_font);

    if (isEnabled())
     {
        _p.fillRect(0, 0, width(), height(), m_cBkColor[m_eSate]);
        _p.setPen(m_cFgColor[m_eSate]);
        QFont _font = _p.font();
        _font.setPointSize(m_nFontSize);
         _p.setFont (_font);
        _p.drawText(m_nBdWidth, 0,  width() - m_nBdWidth * 2, height() - m_nBdWidth * 2, m_nFlag,  text());

        if (m_nBdWidth > 0)
         {
               QPen _pen;
              _p.setPen(m_cBdColor[m_eSate]);
              _pen = _p.pen();
              _pen.setWidth(m_nBdWidth);
              _p.setPen( _pen);
             _p.drawRoundedRect(0, 0,  width() - m_nBdWidth , height() - m_nBdWidth,  RECT_RADIUS,  RECT_RADIUS);
         }
    }
    else
    {
            _p.fillRect(0, 0, width(), height(),  DISABLE_BK_COLOR);
            _p.setPen(DISABLE_FG_COLOR);
            QFont _font = _p.font();
            _font.setPointSize(m_nFontSize);
            _p.drawText(2, 0,  width(), height(), m_nFlag, text());
    }
}

void QTipLabel::InitShow(int x_, int y_, int w_, int h_, int nFontsize_)
{
    move(x_, y_);
    setFixedSize(w_, h_);
    m_nFontSize = nFontsize_;
}

void QTipLabel::InitShow(const char* Text_, int x_, int y_, int w_, int h_,  int nFontsize_)
{
    setText(Text_);
    InitShow(x_, y_, w_, h_, nFontsize_);
}


//====================================================


//================================================

QShape::QShape(QWidget* parent_, COLOR_GROUP nColor_,unsigned int f_)
:QLabel(parent_),QItem(nColor_)
{
        setAutoFillBackground(true);//自动画背景色
        m_nFlag =  f_ | Qt::AlignVCenter | Qt::AlignJustify ;
        m_eSate = NORMAL_WIND;
}


void QShape::InitShow(int x_, int y_, int w_ , int h_, int nFontsize_)
{
        m_nFontSize = nFontsize_;
        move(x_, y_);
        setFixedSize(w_, h_);
        m_drawPath.addEllipse(0,0,w_, h_);
}


void QShape::InitShow(const char* Text_, int x_, int y_, int w_ , int h_, int nFontsize_)
{
        setText(Text_);
        m_nFontSize = nFontsize_;
        move(x_, y_);
        setFixedSize(w_, h_);
        m_drawPath.addEllipse(1,1,w_ -2, h_ -2);
}

void QShape::paintEvent (QPaintEvent* e_)
{

        QPainter _p(this);

        QFont _font = _p.font();
        _font.setPointSize(m_nFontSize);
        _p.setFont(_font);

        _p.fillRect(1, 1, width() - 2, height() - 2, QColor(0, 0, 0, COLOR_F));
        _p.fillPath(m_drawPath, m_cBkColor[m_eSate]);
        _p.setPen(m_cFgColor[m_eSate]);
        _p.drawText(0, 0,  width(), height(), m_nFlag, text());
}
