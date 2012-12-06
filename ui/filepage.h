#ifndef FILEPAGE_H
#define FILEPAGE_H

#include "basepage.h"

class QListBox;
class QTipLabel;
class QPushBtn;
class QCodeEdit;

#define FINE_ITEM_NUM 9

class QFilePage : public QBasePage
{
    Q_OBJECT
    struct _FileInfo
    {
            QString szName;
            int nPath; //0本地 、1U盘
            unsigned int nSize;
    };
public:
    QFilePage(QWidget* parent_);
public:
    virtual void OnSndBtnClick(int nIndex_);//响应mainframe 二级菜单点击
    virtual void OnUpDate(int nFlag_){};
protected:
        virtual void showEvent ( QShowEvent * event );
private:
        void ReadFile(const char* szDir_, int nPos_);
        void ShowFile();
        void LoadFile();

private:
        QListBox* m_pFileList;
        QList<_FileInfo> m_lFileList;
        QCodeEdit* m_pCodeEdit;
        int m_nSelFile;
        int m_nMaxShow;
        int m_nShowIndex;
};


#endif // FILEPAGE_H
