#include "basepage.h"


#include "mainframe.h"

#include <QPushButton>

#define COLOR_BACK  QColor(20, 35, 32, COLOR_F)


QBasePage::QBasePage(QWidget* parent_)
:QWidget(parent_)
{
    setFixedSize(854, 618);				//set size
    move(0, 60);
    setAutoFillBackground(true);
    this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    QPalette _pal = this->palette();	//set back ground
    _pal.setColor(QPalette::Background, COLOR_BACK);
    this->setPalette(_pal);
    m_pMainFrame = (QMainFrame*)parent_;//init the main frame pointer
    m_nSndMenuPos = 0;
    m_pSysData = QSysData::Instance();
    m_nSndBtnShow = 0;
    hide();
}


QBasePage::~QBasePage()
{


}


void QBasePage::keyPress( QKeyEvent * e_)
{
    //if (event_->key() == Qt::Key_Return)

}

void QBasePage::keyRelease( QKeyEvent * e_)
{

}
