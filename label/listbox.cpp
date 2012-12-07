#include "listbox.h"
QListBox::QListBox(QWidget * parent_)
    :QTreeWidget(parent_)
{
    setUniformRowHeights(true);
    m_nItemCount = 0;

    QPalette _pal = palette();	//set back ground
    _pal.setColor(QPalette::Base, QColor(220,220, 220));
    _pal.setColor(QPalette::Window, QColor(220,220, 220));
    //_pal.setColor(QPalette::Text, QColor(33,123, 231));
    //_pal.setColor(QPalette::Highlight, QColor(233,123, 231));
    setPalette(_pal);

}

int QListBox::GetCurSel()
{
    QList<QTreeWidgetItem *> _Sel = selectedItems();

    if (_Sel.size() <= 0)
        return -1;

    QTreeWidgetItem* _SelItem =  _Sel[0];
    return indexOfTopLevelItem (_SelItem);
}
