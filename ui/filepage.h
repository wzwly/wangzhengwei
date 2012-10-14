#ifndef FILEPAGE_H
#define FILEPAGE_H

#include "basepage.h"
class QTipLabel;
class QPushBtn;

#define FINE_ITEM_NUM 9

class QFilePage : public QBasePage
{
    Q_OBJECT
    struct _FileItem
    {
            QTipLabel* pIndex; //序号
            QPushBtn* pFileName; //文件名
            QTipLabel* pFileSize; //
            QTipLabel* pFilePath; //

            void SetActive();
            void SetNormal();
    };
    struct _FileInfo
    {
            QString szName;
            int nPath; //0本地 、1U盘
            unsigned int nSize;
    };
public:
    QFilePage(QWidget* parent_);

public slots:
    void OnListClick(int nId_);
    signals:

public:
    virtual void OnSndBtnClick(int nIndex_);//响应mainframe 二级菜单点击
    virtual void OnUpDate(int nFlag_){};

protected:
        virtual void showEvent ( QShowEvent * event );
private:
        void CreatePageInfo();
        void ReadFile(const char* szDir_, int nPos_);
        void ShowFile(int nIndex_);
        void LoadFile();

private:
        _FileItem m_aFimeArray[FINE_ITEM_NUM];
        QList<_FileInfo> m_lFileList;
        int m_nSelFile;
        int m_nMaxShow;
        int m_nShowIndex;
};


#endif // FILEPAGE_H
