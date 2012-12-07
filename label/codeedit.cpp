#include "codeedit.h"
#include "./../ghead.h"

#define COLOR_BACK  QColor(120, 135, 132, COLOR_F)

QCodeEdit::QCodeEdit(QWidget* parent)
:QWidget(parent)
{
    setFixedSize(854, 618); //set size
    setAutoFillBackground(true);
    this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    QPalette _pal = this->palette();	//set back ground
    _pal.setColor(QPalette::Background, COLOR_BACK);
    this->setPalette(_pal);

    InitWind();
}


void QCodeEdit::InitWind()
{

    for (int _i = 0; _i < EDIT_KEY_NUM; ++_i)
    {
        m_pKeyBtn[_i] = new QClickBtn(this, _i);
        m_pKeyBtn[_i]->setFixedSize(80, 65);
        m_pKeyBtn[_i]->move(90 * (_i % 7) + 2, 70 * (_i / 7) + 200);
    }
}
