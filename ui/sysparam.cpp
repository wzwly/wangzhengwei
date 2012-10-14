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

const static char* g_szUnit[PARAM_DEF::UNIT_QTY] =
{"mm", "m", "s", "min", "ms", "mm/min", "m/s","m/min", ""};

QSysParamPage::QSysParamPage(QWidget* parent_)
 :QBasePage( parent_)
{
    CreatePageInfo();
    m_nShow = SHOW_VIEW;
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

const static char* g_szAxixLimitP[AXIS_NUM] =
                    {"X+软限位", "Y+软限位", "Z+软限位", "A+软限位", "B+软限位", "U+软限位"};
const static char* g_szAxixLimitN[AXIS_NUM] =
                    {"X-软限位", "Y-软限位", "Z-软限位", "A-软限位", "B-软限位", "U-软限位"};

const static char* g_szAxixSpeed[AXIS_NUM] =
                    {"X轴速度", "Y轴速度", "Z轴速度", "A轴速度", "B轴速度", "U轴速度"};

void QSysParamPage::InitParam()
{
    m_cGlbData = QSysData::Instance()->GetCfgData();

    //视图参数
    m_vViewParam.append(new PARAM_DEF(&m_cGlbData->fXoffsetView,
                    PARAM_DEF::VIEW_PARM,PARAM_DEF::UNIT_MM,"视图X起点"));
    m_vViewParam.append(new PARAM_DEF(&m_cGlbData->fYoffsetView,
                    PARAM_DEF::VIEW_PARM,PARAM_DEF::UNIT_MM,"视图Y起点"));
    m_vViewParam.append(new PARAM_DEF(&m_cGlbData->fViewWidth,
                    PARAM_DEF::VIEW_PARM,PARAM_DEF::UNIT_MM,"视图宽度"));
    m_vViewParam.append(new PARAM_DEF(&m_cGlbData->fViewHeight,
                    PARAM_DEF::VIEW_PARM,PARAM_DEF::UNIT_MM,"视图高度"));
    m_vViewParam.append(new PARAM_DEF(&m_cGlbData->fRowGap,
                    PARAM_DEF::VIEW_PARM,PARAM_DEF::UNIT_MM,"排间距"));
    //限位参数
    for (int _i = 0; _i < AXIS_NUM; ++_i)
    {
        m_vLimitParam.append(new PARAM_DEF(&m_cGlbData->fLimitP[_i],
                        PARAM_DEF::LIMIT_PARAM,PARAM_DEF::UNIT_MM,g_szAxixLimitP[_i]));
        m_vLimitParam.append(new PARAM_DEF(&m_cGlbData->fLimitN[_i],
                        PARAM_DEF::LIMIT_PARAM,PARAM_DEF::UNIT_MM,g_szAxixLimitN[_i]));
    }
    //速度参参数
    for (int _i = 0; _i < AXIS_NUM; ++_i)
    {
        m_vSpeedParam.append(new PARAM_DEF(&m_cGlbData->fSpeed[_i],
                        PARAM_DEF::SPEED_PARAM,PARAM_DEF::UNIT_MMPS,g_szAxixSpeed[_i]));
    }
    m_vSpeedParam.append(new PARAM_DEF(&m_cGlbData->fManuSpeed,
                    PARAM_DEF::SPEED_PARAM,PARAM_DEF::UNIT_MMPS,"手动速度"));
    m_vSpeedParam.append(new PARAM_DEF(&m_cGlbData->fManuStep,
                    PARAM_DEF::SPEED_PARAM,PARAM_DEF::UNIT_MM,"手动步长"));

    //系统参数

    m_vSysParam.append(new PARAM_DEF(&m_cGlbData->iDrillAxis,
                        PARAM_DEF::SYSTEM_PARAM,PARAM_DEF::UINT_NONE,"钻孔轴个数"));
    m_vSysParam.append(new PARAM_DEF(&m_cGlbData->iAskTime,
                        PARAM_DEF::SYSTEM_PARAM,PARAM_DEF::UNIT_MS,"系统查询时间"));
}

void QSysParamPage::Show()
{
    int _nLen = m_vParamData.size() - m_nShowIndex;
    int _nMax = (_nLen > PARAM_COLOUM) ? PARAM_COLOUM :_nLen;

    if (_nMax == 0)
        return;

    int _j = 0;
    for (int _i = m_nShowIndex; _i < m_nShowIndex + _nMax; ++_i, ++_j)
    {
        m_aParamArray[_j].pIndex->setText(QString::number(m_vParamData[_i]->iIndex));
        m_aParamArray[_j].pName->setText(m_vParamData[_i]->strName);
        m_aParamArray[_j].pData->setText(m_vParamData[_i]->Value());
        m_aParamArray[_j].pUnit->setText(g_szUnit[m_vParamData[_i]->cUnit]);
    }

    for (; _j < PARAM_COLOUM; ++_j)
    {
        m_aParamArray[_j].pIndex->setText("");
        m_aParamArray[_j].pName->setText("");
        m_aParamArray[_j].pData->setText("");
        m_aParamArray[_j].pUnit->setText("");
    }
}

void QSysParamPage::UpdateView(int nIndex_)
{
   m_nShow = SHOW_VIEW;
    m_vParamData.clear();
    for (int _i = 0; _i < m_vViewParam.size(); ++_i)
    {
        m_vParamData.append(m_vViewParam[_i]);
    }
    Show();
}

void QSysParamPage::UpdateSystem(int nIndex_)
{
    m_nShow = SHOW_SYS;
    m_vParamData.clear();
    int _nSize = m_vSysParam.size();
    for (int _i = 0; _i < _nSize ; ++_i)
    {
        m_vParamData.append(m_vSysParam[_i]);
    }
    Show();
}

void QSysParamPage::UpdateSpeed(int nIndex_)
{
    m_nShow = SHOW_SPEED;
    m_vParamData.clear();
    int _nSize = m_vSpeedParam.size();
    for (int _i = 0; _i < _nSize ; ++_i)
    {
        m_vParamData.append(m_vSpeedParam[_i]);
    }
    Show();
}

void QSysParamPage::UpdateLimit(int nIndex_)
{
    m_nShow = SHOW_LIMIT;
    m_vParamData.clear();
    int _nSize = m_vLimitParam.size();
    for (int _i = 0; _i < _nSize ; ++_i)
    {
        m_vParamData.append(m_vLimitParam[_i]);
    }
    Show();
}

void QSysParamPage::OnListClick(int nId_)
{
    if (nId_ >= m_vParamData.size())
        return;
    PARAM_DEF* _p = m_vParamData[nId_];
    QNuberInput _SoftKey(this);
    _SoftKey.SetRange(-1000, 2000, _p->Data());
    if (!_SoftKey.exec())
        return;
    double _data = _SoftKey.GetDbData();
    _p->SetData(_data);
    m_aParamArray[nId_].pData->setText(m_vParamData[nId_]->Value());

     short _wAddr;
    if (m_nShow == SHOW_VIEW)
    {
        m_cGlbData->bReDrawRule = true;
        return;
    }
    else if (m_nShow ==  SHOW_SYS)
        _wAddr = GEN_PARAM::DRILL_AXIS_QTY + _p->iIndex;
    else if (m_nShow == SHOW_SPEED)
         _wAddr = GEN_PARAM::OFFSET_SPEED + _p->iIndex;
    else if (m_nShow == SHOW_LIMIT)
        _wAddr = GEN_PARAM::OFFSET_LIMIT + _p->iIndex;
    else
        return;

    if ( _p->cDataP == PARAM_DEF::DATA_INT)
    {
           int _nData =  int (_data);
           Cmd06WriteKeepReg( _wAddr,  _nData);
    }
    else
    {
        float _fData =  float (_data);
        Cmd06WriteKeepReg( _wAddr,  _fData);
    }
}

void QSysParamPage::OnSndBtnClick(int nIndex_)
{
    if (nIndex_ == 0)
    {
        if(m_nShowIndex > 0)
        {
            m_nShowIndex -= PARAM_COLOUM;
            Show();
        }
    }
    else if (nIndex_ == 1)
    {
       int _nLen = m_vParamData.size() - m_nShowIndex;
       if (_nLen > PARAM_COLOUM)
       {
           m_nShowIndex += PARAM_COLOUM;
            Show();
       }
    }
    else if (nIndex_ == 2)
    {
        m_nShowIndex = 0;
        UpdateSystem(0);
    }
    else if (nIndex_ == 3)
    {
        m_nShowIndex = 0;
        UpdateSpeed(0);
    }
    else if (nIndex_ == 4)
    {
        m_nShowIndex = 0;
        UpdateLimit(0);
    }
    else if(nIndex_ == 5)
    {
        m_nShowIndex = 0;
        UpdateView(0);
    }

}


void QSysParamPage::SendSystemParam(int nPos_)
{
    if (nPos_ >= 0)
        return;

    Cmd06WriteKeepReg(GEN_PARAM::DRILL_AXIS_QTY,m_cGlbData->iDrillAxis);
    Cmd06WriteKeepReg(GEN_PARAM::RUN_QUERY_TIME,m_cGlbData->iAskTime);
}

void QSysParamPage::SendSpeedParam(int nPos_)
{

    if (nPos_ >= 0)
        return;

    CmdWriteKeepRegEx(GEN_PARAM::AXIS_SPPED_X, 6, (unsigned char*)m_cGlbData->fSpeed);
    CmdWriteKeepRegEx(GEN_PARAM::MANUAL_SPPED, 1, (unsigned char*)&m_cGlbData->fManuSpeed);
    CmdWriteKeepRegEx(GEN_PARAM::STEP_LENGHT, 1, (unsigned char*)&m_cGlbData->fManuStep);
}

void QSysParamPage::SendLimitParam(int nPos_)
{
    if (nPos_ >= 0)
        return;

    CmdWriteKeepRegEx(GEN_PARAM::AXIS_LINMIT_XP, 6, (unsigned char*)m_cGlbData->fLimitP);
    CmdWriteKeepRegEx(GEN_PARAM::AXIS_LINMIT_XN, 6, (unsigned char*)m_cGlbData->fLimitN);
}
