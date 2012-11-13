#ifndef __BASEPAGE_H__
#define __BASEPAGE_H__

#include <QWidget>
#include "./../core/qsysdata.h"
#include "./../core/cmddef.h"


#include <QKeyEvent>


class QMainFrame;


class QBasePage : public QWidget
{
    Q_OBJECT

public:
    QBasePage(QWidget* parent_);
    ~QBasePage();
protected:
   //由父窗口赋值
    QMainFrame* m_pMainFrame; //指向主窗口页
    QSysData* m_pSysData;
    //自己创建
    int m_nSndMenuPos; //指向当前二级三级菜单值
    int m_nSndBtnShow; //
    bool m_bIsEditValue;
    DevMaster* m_pModbus;

public:
    void SetModbus( DevMaster* p_){m_pModbus = p_;}
    int GetSndMenuPos(){return m_nSndMenuPos;} //返回当前选择的菜单
    int GetSndBtnShow(){return m_nSndBtnShow;}
    void SetSysData(QSysData* p_){m_pSysData = p_;}
    virtual void OnSndBtnClick(int nIndex_) = 0;//响应mainframe 二级菜单点击
    virtual void keyPress( QKeyEvent * e_);
    virtual void keyRelease( QKeyEvent * e_);
    virtual void OnUpDate(int nFlag_)= 0;
};

#endif // BASEPAGE_H
