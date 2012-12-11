#ifndef __SETPAGE_H__
#define __SETPAGE_H__

#include "basepage.h"
#include "./../core/qsysdata.h"

class QListBox;

#define PARAM_COLOUM  9

class QSysParamPage : public QBasePage
{
    Q_OBJECT


public:
    QSysParamPage(QWidget* parent_);
    ~QSysParamPage(){};

private: 
    void InitParam();
    void UpdateView(int nIndex_);
    void ReShow();

public:
    virtual void OnSndBtnClick(int nIndex_);//响应mainframe 二级菜单点击
    virtual void OnUpDate(int nFlag_){};
 private:

   ConfigData* m_cConfigData;
   QSysData* m_pSysData;

   QListBox* m_pFileList;
   vector<DataMap*> m_vParamData0;
   vector<DataMap*> m_vParamData1;
   vector<DataMap*> m_vParamData2;
   vector<DataMap*> m_vParamData3;
   vector<DataMap*> m_vParamData4;
   vector<DataMap*>* m_pCurParamList;

   int m_nShowColumn;
};


#endif // SETPAGE_H
