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
    enum {
        SYS_PATH = 0,
        USB_PATH,

        NORMAL_FILE_STATUS,
        EDIT_FILE_STATE,
        NEW_FILE_STATE,
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
    void UnloadFile();

    void CpyFile(int nPath_);
    void DeleteFile();

    void ReListFile();
    bool GetCurSelFile(_FileInfo& info_);
    void EditFile();
    void NewFile();
    void SaveFile();
    void ShowEditView(bool bShow_);
private:
    QListBox* m_pFileList;
    QList<_FileInfo> m_lFileList;
    QCodeEdit* m_pCodeEdit;

    _FileInfo m_FileInEdit;
    int m_nFileStatus; 
};


#endif // FILEPAGE_H
