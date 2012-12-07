#ifndef __DLG_H__
#define __DLG_H__
 #include <QDialog>
#include <QLineEdit>
#include "./../ghead.h"
#include <QListWidget>

class QClickBtn;
class QTipLabel;
class  QAllKeyView;



//查看对话框的基类
class QBaseDlg : public QDialog
{
    Q_OBJECT
public:
    QBaseDlg(QWidget * parent_ = 0);
    ~QBaseDlg(){};

public slots:
    void OnBtnClicked(int nId_);//用于发送消息

 protected:
    QClickBtn* m_pOkBtn;
    QClickBtn* m_pCancelBtn;
     LANG m_eLang;
 protected:
     virtual bool OnOkCancel(int nKey_) { return true;}
};

class QOkDlg: public QBaseDlg
{
    Q_OBJECT
public:
    QOkDlg(const QString& str_,QWidget* parent_ = 0);
    QOkDlg(const char*str_,QWidget* parent_ = 0);
    ~QOkDlg();
private:
     QTipLabel* m_pInputTip;

};

//==================================
//==================================
class QStrInputDlg : public QBaseDlg
{
    Q_OBJECT
public:
    enum STR_DLG_TYPE
    {
            FILE_NAME_DLG = 0,
            STR_INPUT_DLG,
    };
    QStrInputDlg(QWidget * parent_ = 0, QStrInputDlg::STR_DLG_TYPE eDlg_ = STR_INPUT_DLG);
     ~QStrInputDlg();

 public slots:
     void OnKeyPushed(int nKey_);
     signals:
private:
     QTipLabel* m_pInputTip;
     QLineEdit* m_pInputText;
     QAllKeyView* m_pKeyBoard;
private:
     void InitialData(STR_DLG_TYPE eDlg_);
public:
     QString GetInputText(){return m_pInputText->text();}

//=======================================
//=======================================
};

//软键盘管理以及实现
class QNuberInput : public QDialog
{
        Q_OBJECT
protected:
        bool IsValidData(QString& strOld_);
public:
       ~QNuberInput();
        QNuberInput(QWidget* parent_ = NULL);

public slots:
    void OnBtnPushed(int nIndex_);

 private:
        QClickBtn* m_pBtnPush[16];

        QLineEdit * m_pEdit;
        QTipLabel* m_pRange;
        double m_dMin;
        double m_dMax;


public:
        QString GetText() {return m_pEdit->text();}
        double GetDbData();//{return m_pEdit->text().toDouble();}
        void SetRange(double dMin_, double dMax_, double dVal_);
        void SetRange(double dMin_, double dMax_, float fVal_);
        void SetRange(double dMin_, double dMax_, int nVal_);
        void SetRange(double dMin_, double dMax_, const QString& str_);
};



/*
class QStreeViewDlg : public QBaseDlg
{
    Q_OBJECT
public:
    enum TREE_DLG_TYPE
    {
            PROBE_SEL_DLG = 0,
            WELDGE_SEL_DLG,
    };
    QStreeViewDlg(QWidget * parent_, QStreeViewDlg::TREE_DLG_TYPE eDlg_);
    ~QStreeViewDlg();

public slots:
    void OnMainSelChange(int nRow_);
    void OnSubSelChange(int nRow_);
private:
    QListWidget* m_pMainList;
    QListWidget* m_pSubList;
    QString m_strBasePath;
    QString m_strFileName;
    QString m_strFullPath;
    QTipLabel* m_pInputTip;

    TREE_DLG_TYPE m_eDlgType;

    WEDGE* m_pWedge;
    PROBE*  m_pProbe;

public:
    void InitfileList(const QString& strPath_);
    const QString& GetFileName() {return m_strFileName;}
    const QString& GetFullPath(){return m_strFullPath;}
    bool GetWedgeParam(WEDGE* pWedge_);
    bool GetProbeParam(PROBE* pProbe_);
protected:
    virtual bool OnOkCancel(int nKey_) ;
    void ListSubViewFile(const QString& strPath_);
    void GetProbeWedgeInfo();

};*/
#endif // DLG_H





