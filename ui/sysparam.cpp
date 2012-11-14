#include "sysparam.h"
#include "./../label/item.h"
#include "./../label/button.h"
#include "./../ghead.h"
#include "./../label/dlg.h"
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
    m_nShowIndex = 0;
    CreatePageInfo();
}

void QSysParamPage::CreatePageInfo()
{
    QTipLabel* _pTemp = new QTipLabel(this,QItem::LABEL_DLG);
    _pTemp->InitShow("编号", 0, 0, FILE_ITEM_W0, FILE_ITEM_H0, FILE_FONT_SIZE);
    _pTemp = new QTipLabel(this,QItem::LABEL_DLG);
    _pTemp->InitShow("参数名", 100, 0, FILE_ITEM_W1, FILE_ITEM_H0, FILE_FONT_SIZE);
    _pTemp = new QTipLabel(this,QItem::LABEL_DLG);
    _pTemp->InitShow("参数值", 554, 0, FILE_ITEM_W2, FILE_ITEM_H0, FILE_FONT_SIZE);
    _pTemp = new QTipLabel(this,QItem::LABEL_DLG);
    _pTemp->InitShow("单位", 704, 0, FILE_ITEM_W3, FILE_ITEM_H0, FILE_FONT_SIZE);

    for (int _i = 0; _i < PARAM_COLOUM; ++_i)
    {
        m_aParamArray[_i].pIndex = new QTipLabel(this, QItem::LABEL_LIST);
        m_aParamArray[_i].pIndex->InitShow(0, FILE_ITEM_H0 + FILE_ITEM_H * _i, FILE_ITEM_W0, FILE_ITEM_H, FILE_FONT_SIZE);
        m_aParamArray[_i].pName = new QTipLabel(this, QItem::LABEL_LIST, Qt::AlignLeft);
        m_aParamArray[_i].pName->InitShow(100, FILE_ITEM_H0 + FILE_ITEM_H * _i, FILE_ITEM_W1, FILE_ITEM_H, FILE_FONT_SIZE);
        m_aParamArray[_i].pData = new QPushBtn(this, QItem::LABEL_TIP,_i,Qt::AlignLeft);
        m_aParamArray[_i].pData->InitShow(554, FILE_ITEM_H0 + FILE_ITEM_H * _i, FILE_ITEM_W2, FILE_ITEM_H, FILE_FONT_SIZE);
        m_aParamArray[_i].pData->SetBdWide(5);
        m_aParamArray[_i].pUnit = new QTipLabel(this, QItem::LABEL_LIST);
        m_aParamArray[_i].pUnit->InitShow(704, FILE_ITEM_H0 + FILE_ITEM_H * _i, FILE_ITEM_W3, FILE_ITEM_H, FILE_FONT_SIZE);
        connect(m_aParamArray[_i].pData, SIGNAL(ClickedEvent(int)), this, SLOT(OnListClick(int)));
    }

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
            break;
        case 6:
            break;
        default:
            assert(false);
            break;
        }
    }

}

void QSysParamPage::Show()
{
    int _nLen = m_pSysParam->size() - m_nShowIndex;
    int _nMax = (_nLen > PARAM_COLOUM) ? PARAM_COLOUM :_nLen;


    int _j = 0;
    DataMap* _pMap = NULL;
    for (int _i = m_nShowIndex; _i < m_nShowIndex + _nMax; ++_i, ++_j)
    {
        _pMap = m_pSysParam->at(_i);
        m_aParamArray[_j].pIndex->setText(QString::number(_pMap->iNo));
        m_aParamArray[_j].pName->setText(_pMap->strName.data());
        m_aParamArray[_j].pData->setText(m_pSysData->GetValText(_pMap));
        m_aParamArray[_j].pUnit->setText(_pMap->strUnit.data());
        m_aParamArray[_j].pDataMap = _pMap;
    }

    for (; _j < PARAM_COLOUM; ++_j)
    {
        m_aParamArray[_j].pIndex->setText("");
        m_aParamArray[_j].pName->setText("");
        m_aParamArray[_j].pData->setText("");
        m_aParamArray[_j].pUnit->setText("");
         m_aParamArray[_j].pDataMap = NULL;
    }
}

void QSysParamPage::UpdateView(int nIndex_)
{
    if (0 == nIndex_)
        m_pSysParam = & m_vParamData0;
    else if (1 == nIndex_)
        m_pSysParam = & m_vParamData1;
    else if (2 == nIndex_)
        m_pSysParam = & m_vParamData2;
    else if (3 == nIndex_)
        m_pSysParam = & m_vParamData3;
    Show();
}

void QSysParamPage::OnListClick(int nId_)
{
    DataMap* _pMap = m_aParamArray[nId_].pDataMap;

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
    m_aParamArray[nId_].pData->setText(m_pSysData->GetValText(_pMap));

    m_pSysData->SendToModelBus(_pMap);
}

void QSysParamPage::OnSndBtnClick(int nIndex_)
{

    if (nIndex_ <= 3)
    {
        m_nShowIndex = 0;
        UpdateView(nIndex_);
    }
    else if (nIndex_ == 4)
    {
        if(m_nShowIndex > 0)
        {
            m_nShowIndex -= PARAM_COLOUM;
            Show();
        }
    }
    else if (nIndex_ == 5)
    {
         int _nLen = m_pSysParam->size() - m_nShowIndex;
         if (_nLen > PARAM_COLOUM)
         {
             m_nShowIndex += PARAM_COLOUM;
              Show();
         }
    }

}
