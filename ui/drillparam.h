#ifndef SCANPAGE_H
#define SCANPAGE_H

#include "basepage.h"
#define DRILL_PARAM_NUM  33

class QTipLabel;
class QPushBtn;

class QDrillParamPage : public QBasePage
{
    Q_OBJECT
    struct _ParamItem
    {
            QTipLabel* pName; //文件名
            QPushBtn* pData;  //
    };


public:
     struct _SaveData
     {
         int nSize;
         float m_fDataY[120];
         float m_fDataX[12];
         unsigned int iCrc;
         _SaveData(){nSize = sizeof(_SaveData); iCrc = 0x3535fefe;};
         bool IsValid() {return iCrc == 0x3535fefe;}
     };

    QDrillParamPage(QWidget* parent_);

public slots:
    void OnDataEdit(int nId_);//响应
    signals:

public:
    virtual void OnSndBtnClick(int nIndex_);//响应mainframe 二级菜单点击
    virtual void OnUpDate(int nFlag_){};

private:
    void CreatePageInfo();
    void OnShowPage(int nPos_);
protected:
    virtual void showEvent ( QShowEvent * event );

private:
    _ParamItem m_aParamArray[DRILL_PARAM_NUM];
     _SaveData m_cData;
    int m_nPagePos;
};








#endif // SCANPAGE_H
