#ifndef LISTBOX_H
#define LISTBOX_H


#include <QStringList>
#include <QTreeWidget>

class QListBox : public QTreeWidget
{
    Q_OBJECT
public:
    QListBox( QWidget * parent_ = 0 );

    ~QListBox(){};

private:
    int m_nItemCount;
public:
    int GetListCount() {return m_nItemCount;}
    void SetWindPos(int x_, int y_, int w_, int h_) {move(x_, y_); setFixedSize(w_, h_);}
    void SetColumnCount(int n_) {setColumnCount(n_);}
    void SetColumnHeadName(const QStringList& strNameList_) {setHeaderLabels(strNameList_);}
    void SetColumnWidth(int nColumn_, int w_) {setColumnWidth(nColumn_,w_);}
    void SetColumnHeight(int w_)
    {
        QFont _font = this->font();
        _font.setPointSize( w_);
        this->setFont (_font);
    }

    void SetRowText(int nRow_, const QStringList& strNameList_)
    {
        if (nRow_ >= m_nItemCount)
            return;
        insertTopLevelItem(nRow_, new QTreeWidgetItem((QTreeWidget*)NULL,strNameList_));
    }

    void InsertRowText(const QStringList& strNameList_)
    {
        insertTopLevelItem(m_nItemCount++, new QTreeWidgetItem((QTreeWidget*)NULL,strNameList_));
    }

    void DeleteRow(int nRow_)
    {
        if (nRow_ >= m_nItemCount)
            return;
        QTreeWidgetItem* _p = takeTopLevelItem(nRow_);
        if (_p)
            delete _p;
        --m_nItemCount;
    }

    void ClearAllRow()
    {
        while(m_nItemCount > 0)
            DeleteRow(0);
    }
    int GetCurSel() ;
};




#endif // LISTBOX_H
