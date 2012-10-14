#include "qsysdata.h"
#include "./../ui/drillparam.h"
#include "./../label/configset.h"
#include "./../label/dlg.h"
#include "dxfreader.h"
#include "./../core/cmddef.h"
#include "./../ui/addrdef.h"
#include "./../ghead.h"

static int g_sIndex[PARAM_DEF::DATA_QTY] = {0};
PARAM_DEF::PARAM_DEF(float* pf_,DATA_CLASS T_,DATA_UNIT U_,const char* pName_)
:strName(pName_)
{
    p = pf_;
    cType = T_;
    cUnit = U_;
    cDataP = DATA_FLR;
    iIndex = g_sIndex[T_]++;
}

PARAM_DEF::PARAM_DEF(int* pn_,DATA_CLASS T_,DATA_UNIT U_,const char* pName_)
:strName(pName_)
{
   p = pn_;
   cType = T_;
   cUnit = U_;
   cDataP = DATA_INT;
   iIndex = g_sIndex[T_]++;
}


 QString PARAM_DEF::Value()
 {
    if (cDataP == DATA_INT)
        return QString::number(*((int*)p));
    else
         return QString::number(*((float*)p), 'f', 3);
 }


 float PARAM_DEF::Data()
 {
    if (cDataP == DATA_INT)
        return *((int*)p);
    else
        return *((float*)p);
 }
 void PARAM_DEF::SetData(float f)
 {
     if (cDataP == DATA_INT)
         *((int*)p) = int(f);
     else
       *((float*)p) = f;
 }

GlbConfig::GlbConfig()
{
}

void GlbConfig::ReSet()
{
    //=============
    fXoffsetView = 0.0;
    fYoffsetView = 0.0;
    fViewWidth = 1500.0;
    fViewHeight = 1000.0;
    fRowGap = 10.0;
    fHoleRadius = 5.0;

    bReDrawRule = true;

    //===========================
    for (int _i = 0; _i < AXIS_NUM; ++_i)
    {
         fLimitN[_i] = 0.0;
         fLimitP[_i] = 1500;

         fSpeed[_i] = 30.0;
         fAxisPos[_i] = 0.0;
    }

    //===================
    iDrillAxis = 4;
    iAskTime = 2000;
}

QSysData* QSysData::Instance()
{
    static QSysData* _s_pSysData = NULL;
    if (_s_pSysData == NULL)
        _s_pSysData = new QSysData();
    return _s_pSysData;
}

QSysData::QSysData()
{
    bool _bRet =  LoadClassData(&__n_Save_Begin__,  &__n_Save_End__, "QSysData");
    if (!_bRet)
    {
        m_cGlbData.ReSet();
         ReSetLoad();
    }
    else
    {
         QString _path(m_szPath);
         QString _name(m_szName);
          LoadFile(_path, _name);
    }

    InitParamData();
}

  QSysData::~QSysData()
 {
         SaveClassData(&__n_Save_Begin__,  &__n_Save_End__, "QSysData");
 }

void QSysData::InitParamData()
{

}

void QSysData::LoadFile(const QString& path_, const QString& name_)
{
    m_strFilePath = path_;
    m_strFileName = name_;

    std::string _str = m_strFilePath.toStdString ();
    _str.copy(m_szPath, _str.size());
    m_szPath[_str.size()] = '\0';

    std::string _str1 = m_strFileName.toStdString ();
    _str1.copy(m_szName, _str1.size());
    m_szName[_str1.size()] = '\0';

    QString _fix = path_.right(3);
    if (_fix == "xtf")
        LoadFromXtfFile(path_);
    else
        LoadFromDxfFile(path_);

}


void QSysData::ReSetLoad()
{
    m_strFilePath.clear();
    m_szPath[0] = '\0';
    m_szName[0] = '\0';
    ReSetData();
};

void QSysData::LoadFromDxfFile(const QString& path)
{
    QDxfReader _dxf;
    _dxf.AtachFile(path);
    if (!_dxf.StartAnalyse())
    {
        QString _str = QString("载入文件【%1】失败！").arg(m_strFileName);
        QOkDlg _dlg(_str);
        _dlg.exec();
        return;
    }
    ReSetData();
    for (int _i = 0; _i < _dxf.m_vShapeList.size(); ++_i)
            m_vDrillData.push_back(_dxf.m_vShapeList.at(_i));

}

void QSysData::LoadFromXtfFile(const QString& path)
{
    QDrillParamPage::_SaveData _pData;
    QConfigSet::ReadFromFile(path, &_pData, _pData.nSize, 0);
    if (!_pData.IsValid())
    {
         QString _str = QString("载入文件【%1】失败！").arg(m_strFileName);
         QOkDlg _dlg(_str);
         _dlg.exec();
         return;
    }
    ReSetData();
    m_vDrillData.resize(120);
    int _nPos = 0;
    int _nAddr = HOLE_POS::ADDR_ROW0;
    float _fX, _fY;
    for(int _i = 0; _i < 12; ++_i)
    {
        for (int _j = 0; _j < 10; ++_j)
        {
            _fX = _pData.m_fDataX[_i];
            _fY = _pData.m_fDataY[_i * 12 + _j];
            m_vDrillData[_nPos++] = FLOAT_POINT(_fX, _fY);
            Cmd06WriteKeepReg(_nAddr + _j, _fX);
            Cmd06WriteKeepReg(_nAddr + _j + 1, _fY);
        }
         _nAddr += HOLE_POS::HOLE_ROW_MAX;
         Cmd06WriteKeepReg(CTL_PARAM_ADDR::ROW0_HOLE_QTY + _i, 10);
    }
     Cmd06WriteKeepReg(CTL_PARAM_ADDR::ROW_QTY, 12);
}


void QSysData::SortDxfData()
{
     float  _fTemp,_fGap = m_cGlbData.fRowGap;
     int _aRow[HOLE_POS::ROW_MAX] = {0};
     int _nClone = 0, _k = 0;
     FLOAT_POINT _fSortData[HOLE_POS::ROW_MAX][HOLE_POS::HOLE_ROW_MAX];

     FLOAT_POINT _fP;
     for (int _i = 0; _i < m_vDrillData.size(); ++_i)
     {
            _fP = m_vDrillData[_i];
            _k = 0;
            for (int _j = 0; _j < _nClone; ++_j)
            {
                 _fTemp = abs(_fP.x - _fSortData[_j][0].x);
                 if (FLT_L(_fTemp, _fGap) && _aRow[_j] < HOLE_POS::HOLE_ROW_MAX)
                 {
                     _k = _aRow[_j]++;
                     _fSortData[_j][_k] = _fP;
                    break;
                 }
            }
            if (_k == 0 && _nClone < HOLE_POS::ROW_MAX)
            {
                _aRow[_nClone]++;
                 _fSortData[_nClone][0] = _fP;
                 _nClone++;
            }
     }

     Cmd06WriteKeepReg(CTL_PARAM_ADDR::ROW_QTY, _nClone);
     int _nAddr = HOLE_POS::ADDR_ROW0;
     for (int _i = 0; _i <_nClone; ++_i)
     {
            if (_aRow[_i] > 0)
            {
                 SortOneRow(_fSortData[_i] , _aRow[_i]);

                 for (int _j = 0; _j < _aRow[_i]; ++_j)
                 {
                     Cmd06WriteKeepReg(_nAddr + _j, _fSortData[_i][_j].x);
                     Cmd06WriteKeepReg(_nAddr + _j + 1,  _fSortData[_i][_j].y);
                 }
            }
            _nAddr += HOLE_POS::HOLE_ROW_MAX;
           Cmd06WriteKeepReg(CTL_PARAM_ADDR::ROW0_HOLE_QTY + _i,  _aRow[_i]);
     }

}

 void QSysData::SortOneRow(FLOAT_POINT* p_,  int nSize_)
 {
        float _fMin;
        int _nPos;
        FLOAT_POINT _fT;
        for(int _i = 0; _i < nSize_  - 1; ++_i)
        {
               _nPos = _i;
               _fMin = p_[_i].y;
                for (int _j = _i + 1; _j < nSize_; ++_j)
                {
                        if (_fMin > p_[_j].y)
                        {
                                _fMin = p_[_j].y;
                                _nPos = _j;
                        }
                }

                if (_i != _nPos)
                {
                       _fT = p_[_i];
                       p_[_i] = p_[_nPos];
                       p_[_nPos] = _fT;
                }
        }
 }
