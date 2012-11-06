#ifndef __SETPAGE_H__
#define __SETPAGE_H__

#include "basepage.h"
#include "./../core/qsysdata.h"


class QTipLabel;
class QPushBtn;
#define PARAM_COLOUM  9

class QSysParamPage : public QBasePage
{
    Q_OBJECT
    struct _ParamItem
    {
        QTipLabel* pIndex; //
        QTipLabel* pName; //
        QPushBtn* pData;  //
        QTipLabel* pUnit; //
    };

public:
    QSysParamPage(QWidget* parent_);
    ~QSysParamPage(){};

public slots:
    void OnListClick(int);
    signals:

private:
    void CreatePageInfo();
    void InitParam();
    void UpdateView(int nIndex_);
    void Show();

    void SendSystemParam(int nPos_ = -1);
    void SendSpeedParam(int nPos_ = -1);
    void SendLimitParam(int nPos_ = -1);
public:
    virtual void OnSndBtnClick(int nIndex_);//响应mainframe 二级菜单点击
    virtual void OnUpDate(int nFlag_){};
 private:
    _ParamItem m_aParamArray[PARAM_COLOUM];

   ConfigData* m_cConfigData;
   QSysData* m_pSysData;
   vector<DataMap*> m_vParamData0;
   vector<DataMap*> m_vParamData1;
   vector<DataMap*> m_vParamData2;
   vector<DataMap*> m_vParamData3;
   vector<DataMap*>* m_pSysParam;

   int m_nShowIndex;
};


#endif // SETPAGE_H
