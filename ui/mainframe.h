#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <QMainWindow>
#include "./../ghead.h"

class  QPushBtn;
class  QBasePage;
class  QTipLabel;
class  QSysData;




class QMainFrame : public QMainWindow
{
    Q_OBJECT
public:
    QMainFrame(QWidget *parent = 0);
    ~QMainFrame();


public slots: 
    void OnMainMenuShow(int nId_);//一级菜单响应
    void OnSndMenuBtn(int nId_); //二级菜单响应
private:  

    QPushBtn*  m_pPageMenu[MAIN_MENU_COUNT];
    QPushBtn*  m_pScdMenuBtn[SECOND_MENU_COUNT];//二级菜单
    QTipLabel* m_pDateYMD;
    QTipLabel* m_pTitle;
    QTipLabel* m_pDateHMS;

    QBasePage* m_pMenuPage[MAIN_MENU_COUNT];
    QBasePage* m_pCurMenuPage; //当前选择的菜单界面


    LANG m_eLang;

    int m_nCurMainMenu;
    int m_nCurSndMenu;

    bool    m_bSndBtnActive;
    bool    m_bHasPressed;

    QSysData* m_pSysData;

protected:
    void CreateMainMenu();
    void CreatePage();
    void ChangeSndMenuText(int nIndex_);
    void timerEvent(QTimerEvent *event_); //定时器响应函数
    void OnTimerUpdate();				  //主界面定时更新函数
    virtual void keyPressEvent ( QKeyEvent* e_);
    virtual void keyReleaseEvent ( QKeyEvent* e_);


 public:
    //void OnChangeLang(LANG eLang_);
    void SetSndBtnUnActive(bool bActive_ = true){m_bSndBtnActive = bActive_;} //设置二级菜单，包括主菜单的激活状态
    void SetTiltleLabel(const QString& str_);
    LANG GetLangType() { return m_eLang;}
    void ReShowMenuBtn();
};


extern QMainFrame* GetMainFrame() ;
#endif // MAINFRAME_H