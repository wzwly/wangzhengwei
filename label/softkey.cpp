#include "softkey.h"
#include <QtGui/QApplication>
#include <QPainter>
#include <QWSServer>
#include "button.h"
#include "./../ui/mainframe.h"

#define COLOR_BACK  QColor(82, 107, 107, COLOR_F)

static const char* g_szKeyVal[20] = {"7", "8", "9", "回格","4", "5", "6", "删除", "1", "2", "3",
                                 "+", "0", "↑", ".", "-", "←", "↓", "→", "确认"};
static const int g_cKeyVal[20] = {Qt::Key_7, Qt::Key_8, Qt::Key_9, Qt::Key_Backspace, Qt::Key_4, Qt::Key_5, Qt::Key_6,
                                    Qt::Key_Delete, Qt::Key_1, Qt::Key_2, Qt::Key_3, Qt::Key_Plus, Qt::Key_0, Qt::Key_Up, Qt::Key_Period,
                                    Qt::Key_Minus, Qt::Key_Left, Qt::Key_Down, Qt::Key_Right, Qt::Key_Return};


static QSoftKey*  g_pSoftKey = NULL;

QSoftKey* QSoftKey::Instance()
{

        if (g_pSoftKey == NULL)
            g_pSoftKey = new QSoftKey();
            // Note:这里不能添加到全局的指针对象队列，因为其加入到窗口内部，已经被App管理 wangzhengwei
         return g_pSoftKey;
}

QSoftKey::QSoftKey()
{
        m_pSoftKey = new QSoftKeyView();
        m_pAllKeyView = new QAllKeyView();
        connect(m_pSoftKey, SIGNAL(SendCharacter(int)), SLOT(SendKey(int)));
        connect(m_pAllKeyView, SIGNAL(SendCharacter(int)), SLOT(SendKey(int)));
}


QSoftKey::~QSoftKey()
{
       delete m_pSoftKey;
}


 void  QSoftKey::Show(bool bShow_, bool bAllKey_)
 {
     static bool _bShow = false;
       if (_bShow && bShow_)
           return;

       _bShow = bShow_;
        if (bAllKey_)
        {
            if  (bShow_)
            {
                QPoint _P(40, 335);
                _P = GetMainFrame()->mapToGlobal(_P);
               m_pAllKeyView->move(_P);
            }
             bShow_ ? m_pAllKeyView->show() :  m_pAllKeyView->hide();
        }
        else
        {
            if  (bShow_)
            {
                QPoint _P(450, 105);
                _P = GetMainFrame()->mapToGlobal(_P);
                m_pSoftKey->move(_P);
            }
              bShow_ ? m_pSoftKey->show() :  m_pSoftKey->hide();
        }
 }


void QSoftKey::SendKey(int nKey_)
{
    QWidget*  _pW =  focusWidget();


    if (nKey_ == Qt::Key_Down || nKey_ == Qt::Key_Up  || _pW == NULL)
    {
        QKeyEvent _keyPress(QEvent::KeyPress,  nKey_, Qt::NoModifier, QString(QChar(nKey_)));
        QCoreApplication::sendEvent( GetMainFrame(), &_keyPress);

        QKeyEvent _keyRelease(QEvent::KeyRelease,  nKey_, Qt::NoModifier, QString(QChar(nKey_)));
        QCoreApplication::sendEvent( GetMainFrame(), &_keyRelease);

    }
    else if (_pW)
    {
              QKeyEvent _keyPress(QEvent::KeyPress,  nKey_, Qt::NoModifier, QString(QChar(nKey_)));
              QCoreApplication::sendEvent(_pW, &_keyPress);

              QKeyEvent _keyRelease(QEvent::KeyRelease,  nKey_, Qt::NoModifier, QString(QChar(nKey_)));
              QCoreApplication::sendEvent(_pW, &_keyRelease);
      }
}


//==================
QSoftKeyView::QSoftKeyView(QWidget* parent_)
:QWidget(parent_, Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint)
{
    setFixedSize(200, 250);	//set size
    setAutoFillBackground(true);
    QPalette _pal = this->palette();	//set back ground
    _pal.setColor(QPalette::Background, COLOR_BACK);
    this->setPalette(_pal);

    //按钮
    for (int _i = 0; _i < 20 ; ++_i)
    {
            m_pBtnPush[_i] = new QClickBtn(this, _i);
            m_pBtnPush[_i]->setFixedSize(45, 45);
            m_pBtnPush[_i]->move(50 * (_i % 4) + 2, 50 * (_i / 4) + 2);
             m_pBtnPush[_i]->setStyleSheet("QPushButton {background-color:rgba(38, 54, 61); color:white; font-size:14px}\
                          QPushButton:pressed{background-color: rgba(115,95,65);}");
            m_pBtnPush[_i]->setText(g_szKeyVal[_i]);
            connect(m_pBtnPush[_i],SIGNAL(ClickedEvent(int)), this, SLOT(OnBtnPushed(int)));
    }

    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),
          this, SLOT(SaveFocusWidget(QWidget*,QWidget*)));

    m_pOldFocalWidget = NULL;

}

QSoftKeyView::~QSoftKeyView()
{

}


void QSoftKeyView::OnBtnPushed(int nIndex_)
{
     emit SendCharacter(g_cKeyVal[nIndex_]);
}


void QSoftKeyView::SaveFocusWidget(QWidget * pOldFocus_, QWidget* pNewFocus_)
{
    if (pNewFocus_ != NULL && !isAncestorOf(pNewFocus_))
    {
         m_pOldFocalWidget = pNewFocus_;
    }
}

//========================================================
static const char* g_szDownKeyVal[48] =
{"Esc", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "Back\nSpace",
  "q", "w", "e", "r", "t", "y", "u","i", "o", "p","-","+",
  "a","s","d","f","g","h","j","k","l","(",")",".",
   "Caps\nLock", "z", "x", "c", "v", "b","n","m","←", "→", "Space","Enter"};

static const char* g_szUpKeyVal[48] =
{"Esc", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "Back\nSpace",
  "Q", "W", "E", "R", "T", "Y", "U","I", "O", "P","-","+",
  "A","S","D","F","G","H","J","K","L","(",")",".",
   "Caps\nLock", "Z", "X", "C", "V", "B","N","M","←", "→", "Space","Enter"};

static const int g_cAllKeyVal[48] = {
Qt::Key_Escape, Qt::Key_1, Qt::Key_2, Qt::Key_3,Qt::Key_4,Qt::Key_5,Qt::Key_6,Qt::Key_7,Qt::Key_8,Qt::Key_9,Qt::Key_0,Qt::Key_Backspace,
Qt::Key_Q, Qt::Key_W,Qt::Key_E,Qt::Key_R,Qt::Key_T,Qt::Key_Y,Qt::Key_U,Qt::Key_I,Qt::Key_O,Qt::Key_P,Qt::Key_Minus, Qt::Key_Plus,
Qt::Key_A,Qt::Key_S,Qt::Key_D,Qt::Key_F,Qt::Key_G,Qt::Key_H,Qt::Key_J,Qt::Key_K,Qt::Key_L,Qt::Key_ParenLeft,Qt::Key_ParenRight,Qt::Key_Period,
Qt::Key_CapsLock, Qt::Key_Z, Qt::Key_X, Qt::Key_C, Qt::Key_V,  Qt::Key_B, Qt::Key_N,Qt::Key_M, Qt::Key_Left, Qt::Key_Right, Qt::Key_Space,Qt::Key_Return};



QAllKeyView::QAllKeyView(QWidget* parent_)
:QWidget(parent_, Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint)
{
    setFixedSize(720, 240);	//set size
    setAutoFillBackground(true);
    QPalette _pal = this->palette();	//set back ground
    _pal.setColor(QPalette::Background, COLOR_BACK);
    this->setPalette(_pal);
    m_bUpCaps = true;

    //按钮
    for (int _i = 0; _i < 48 ; ++_i)
    {
            m_pBtnPush[_i] = new QClickBtn(this, _i);
            m_pBtnPush[_i]->setFixedSize(58, 58);
            m_pBtnPush[_i]->move(60 * (_i % 12) + 2, 60 * (_i / 12) + 2);
             m_pBtnPush[_i]->setStyleSheet("QPushButton {background-color:rgba(38, 54, 61); color:white; font-size:18px; padding:0 pix; margin:0pix;}\
                          QPushButton:pressed{background-color: rgba(90,95,76);}");
            m_pBtnPush[_i]->setText(g_szUpKeyVal[_i]);
            connect(m_pBtnPush[_i],SIGNAL(ClickedEvent(int)), this, SLOT(OnBtnPushed(int)));
    }

    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),
          this, SLOT(SaveFocusWidget(QWidget*,QWidget*)));

    m_pOldFocalWidget = NULL;

}

QAllKeyView::~QAllKeyView()
{

}

void QAllKeyView::OnBtnPushed(int nIndex_)
{

    if (nIndex_ == 36)
    {
        m_bUpCaps = !m_bUpCaps;
        const char** _pText = m_bUpCaps ? g_szUpKeyVal : g_szDownKeyVal;
        for (int _i = 0; _i < 48 ; ++_i)
        {
                m_pBtnPush[_i]->setText(_pText[_i]);
        }
    }
    int _nKey = g_cAllKeyVal[nIndex_];

    if (!m_bUpCaps && (_nKey >= Qt::Key_A && _nKey <=Qt::Key_Z))
    {
            _nKey += 32;
    }

    emit SendCharacter(_nKey);
}


void QAllKeyView::SaveFocusWidget(QWidget * pOldFocus_, QWidget* pNewFocus_)
{
    if (pNewFocus_ != NULL && !isAncestorOf(pNewFocus_))
    {
         m_pOldFocalWidget = pNewFocus_;
    }
}

