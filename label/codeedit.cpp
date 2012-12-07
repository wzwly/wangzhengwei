#include "codeedit.h"
#include <QtGui/QApplication>
#include <QKeyEvent>
#include "./../ghead.h"
#include "softkey.h"

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


void QCodeEdit::showEvent ( QShowEvent * event )
{
    m_pKeyBd->show();
}

void QCodeEdit::hideEvent ( QHideEvent * event )
{
     m_pKeyBd->hide();
}


void QCodeEdit::InitWind()
{
    m_pTextEdit = new QTextEdit(this);
    m_pTextEdit->setFixedSize(854, 517);

    m_pKeyBd = new QEditKey(this);
    m_pKeyBd->move(0, 590);
    connect(m_pKeyBd,SIGNAL(SendCharacter(int)), this, SLOT(OnKeyPushed(int)));

}

 void QCodeEdit::OnKeyPushed(int nKey_)
 {
    QKeyEvent _keyPress(QEvent::KeyPress,  nKey_, Qt::NoModifier, QString(QChar(nKey_)));
    QCoreApplication::sendEvent(m_pTextEdit, &_keyPress);
 }


 void QCodeEdit::SetEdittext(const QString& str_)
 {
     m_pTextEdit->setPlainText(str_);
 }

 inline QString QCodeEdit::GetCodeText() const
 {
     m_pTextEdit->toPlainText ();
 }


