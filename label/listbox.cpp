#include "listbox.h"
QListBox::QListBox(QWidget * parent_)
    :QTreeWidget(parent_)
{
    setUniformRowHeights(true);
    m_nItemCount = 0;
    /*
    QPalette _pal = _pListBox->palette();	//set back ground
    _pal.setColor(QPalette::Base, QColor(200,223, 231));
    _pal.setColor(QPalette::Window, QColor(200,223, 231));
    _pal.setColor(QPalette::Text, QColor(33,123, 231));
    _pal.setColor(QPalette::Highlight, QColor(233,123, 231));
    _pListBox->setPalette(_pal);
    */
}
