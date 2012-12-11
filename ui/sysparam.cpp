#include "sysparam.h"

#include "./../label/label.h"
#include "./../ghead.h"
#include "./../core/cmddef.h"
#include "addrdef.h"

#define FILE_ITEM_H0 60
#define FILE_ITEM_H  62
#define FILE_ITEM_W0 100
#define FILE_ITEM_W1 454
#define FILE_ITEM_W2 150
#define FILE_ITEM_W3 150

#define FILE_FONT_SIZE 18

#define SHOW_VIEW  5
#define SHOW_LIMIT 4
#define SHOW_SPEED 3
#define SHOW_SYS   2


QSysParamPage::QSysParamPage(QWidget* parent_)
    :QBasePage( parent_)
{
    m_nShowColumn = 0;
    m_pFileList = new QListBox(this);
    m_pFileList->SetWindPos(0, 0,853, 618);
    QStringList _listHead;
    _listHead << "编号"<<"参数名"<<"参数值"<<"单位";
    m_pFileList->SetColumnHeadName( _listHead);

    m_pFileList->SetColumnWidth(0, 135);
    m_pFileList->SetColumnWidth(1, 340);
    m_pFileList->SetColumnWidth(2, 180);
    m_pFileList->SetColumnWidth(3, 180);
    m_pFileList->SetColumnHeight(18);    
    InitParam();
    UpdateView(0);
}

void QSysParamPage::InitParam()
{    
    m_pSysData = QSysData::Instance();
    m_cConfigData = QSysData::Instance()->GetCfgData();

    DataMap* _pMap = NULL;
    for (int _i = 0; _i <  m_cConfigData->m_pArrayData.size(); ++_i)
    {
        _pMap = m_cConfigData->m_pArrayData[_i];
        switch(_pMap->iGroup)
        {
        case 1:
            m_vParamData0.push_back(_pMap);
            break;
        case 2:
            m_vParamData1.push_back(_pMap);
            break;
        case 3:
            m_vParamData2.push_back(_pMap);
            break;
        case 4:
            m_vParamData3.push_back(_pMap);
            break;
        case 5:
            m_vParamData4.push_back(_pMap);
            break;
        case 6:
            break;
        default:
            assert(false);
            break;
        }
    }

}

void QSysParamPage::ReShow()
{
    m_pFileList->ClearAllRow();
    DataMap* _pMap = NULL;
    for (int _i = 0; _i < m_pCurParamList->size(); ++_i)
    {
        _pMap = m_pCurParamList->at(_i);
        QStringList _listParam;
        _listParam << QString::number(_pMap->iNo) << _pMap->strName.data()
                << m_pSysData->GetValText(_pMap)<<_pMap->strUnit.data();
        m_pFileList->InsertRowText(_listParam);
    }
}

void QSysParamPage::UpdateView(int nIndex_)
{
    if (0 == nIndex_)
        m_pCurParamList = & m_vParamData0;
    else if (1 == nIndex_)
        m_pCurParamList = & m_vParamData1;
    else if (2 == nIndex_)
        m_pCurParamList = & m_vParamData2;
    else if (3 == nIndex_)
        m_pCurParamList = & m_vParamData3;
    else if (4 == nIndex_)
        m_pCurParamList = & m_vParamData4;
    ReShow();
}


void QSysParamPage::OnSndBtnClick(int nIndex_)
{
    if (nIndex_ <= 4)
    {
        if (m_nShowColumn == nIndex_)
            return;
        m_nShowColumn = nIndex_;
        UpdateView(nIndex_);
    }
    else if (nIndex_ == 5) //edit
    {
        int _nSel = m_pFileList->GetCurSel();
        if (_nSel < 0)
            return;

        DataMap* _pMap = m_pCurParamList->at(_nSel);
        if (_pMap == NULL)
            return;

        QNuberInput _SoftKey(this);
        double _dMin, _dMax;
        m_pSysData->GetMaxMinRange(_pMap, _dMin, _dMax);
        _SoftKey.SetRange(_dMin, _dMax, m_pSysData->GetValText(_pMap));
        if (!_SoftKey.exec())
            return;

        double _data = _SoftKey.GetDbData();
        if (!m_pSysData->CheckValid(_pMap,_data))
        {
            QPopTip::ShowTextInMainframe(" 参数范围设置错误！", 25);
            return;
        }

        m_pSysData->SetVal(_pMap, _data);        
        QStringList _listParam;
        _listParam << QString::number(_pMap->iNo) << _pMap->strName.data()
                << m_pSysData->GetValText(_pMap)<<_pMap->strUnit.data();

        m_pFileList->SetRowText( _nSel, _listParam);
        m_pSysData->SendToModelBus(_pMap);
    }

}
