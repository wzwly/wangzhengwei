#include "qsysdata.h"
#include "dxfreader.h"
#include "cmddef.h"
#include "xtfreader.h"

#include "./../ui/drillparam.h"
#include "./../ui/addrdef.h"
#include "./../label/label.h"
#include "./../ghead.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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
         m_cGlbData.bIsLoaded = false;
         ReSetLoad();
    }
    else
    {
         QString _path(m_szPath);
         QString _name(m_szName);
         m_cGlbData.bIsLoaded = true;

         if ((!_path.isEmpty()) && (!_name.isEmpty()))
            LoadFile(_path, _name);
    }
    m_cGlbData.pData = m_iParamRO0;
    InitParamData();
}

  QSysData::~QSysData()
 {
         SaveClassData(&__n_Save_Begin__,  &__n_Save_End__, "QSysData");
 }

//================param parse operate=================
 QString QSysData::GetValText(const DataMap* pMap_)
 {
    QString _str;
    if (pMap_ == NULL)
        return _str;
    int _nVal = ParamData(pMap_->iNo);
    double _fVal = double(_nVal) / pMap_->dMult;
    _str = QString("%1").arg(_fVal,0, 'f', pMap_->iFraction);
    return _str;
 }

 double QSysData::GetVal(const DataMap* pMap_)
 {
     if (pMap_ == NULL)
         return 0.0;
     int _nVal = ParamData(pMap_->iNo);
     double _fVal = double(_nVal) / pMap_->dMult;
     return _fVal;
 }

 void QSysData::SetVal(const DataMap* pMap_, double fVal_)
 {
     if (pMap_ == NULL)
         return ;
     int _nVal =  Round(fVal_ * pMap_->dMult);
     ParamData(pMap_->iNo) = _nVal;
 }

 bool QSysData::CheckValid(const DataMap* pMap_,double dVal_)
 {
     if (pMap_ == NULL)
         return false;
     int _nVal =  Round(dVal_ * pMap_->dMult);
     DataRangeMap* _pMin = pMap_->pMin;
     if (_pMin)
     {
            if (_pMin->bSysNo)
            {
                if (ParamData(_pMin->nNo) > _nVal)
                    return false;
            }
            else if (DB_G(_pMin->dMaxMin , dVal_))
                return false;
     }

     DataRangeMap* _pMax = pMap_->pMax;
     if (_pMax)
     {
            if (_pMax->bSysNo)
            {
                if (ParamData(_pMax->nNo) < _nVal)
                    return false;
            }
            else if (DB_L(_pMax->dMaxMin , dVal_))
                return false;
     }
     return true;
 }

 void QSysData::GetMaxMinRange(const DataMap*pMap_, double& dMin_, double& dMax_)
 {
     if (pMap_ == NULL)
      {
         dMin_ = -9999.0;
         dMax_ = 9999.0;
         return;
      }

     DataRangeMap* _pMin = pMap_->pMin;
     if (_pMin)
     {
         if (_pMin->bSysNo)
             dMin_ = ParamData(_pMin->nNo);
         else
             dMin_ = _pMin->dMaxMin;
     }
     else
         dMin_ = -9999.0;

     DataRangeMap* _pMax = pMap_->pMax;
     if (_pMax)
     {
         if (_pMax->bSysNo)
            dMax_ =  ParamData(_pMax->nNo);
         else
            dMax_ = _pMax->dMaxMin;
     }
     else
         dMax_ = 9999.0;
 }

void QSysData::InitParamData()
{
     CParseConfig _cfg;
     if(_cfg.OpenConfigFile("PR.info"))
        _cfg.StartLoadConfig(&m_cGlbData);
}

int QSysData::GetParamAddrNo(int nAddr_)
{
        int _nRet = -1;
        _nRet = m_cGlbData.m_mapAddrToIndex.value(nAddr_, _nRet);
        return _nRet;
}

 DataMap* QSysData::GetDataMapByPos(int nPos_)
 {
     int _nRet = -1;
     _nRet = m_cGlbData.m_mapIndexToVector.value(nPos_, _nRet);
     if (_nRet >= 0)
     {
        return m_cGlbData.m_pArrayData[_nRet];
     }
     else
         return NULL;

 }

//====================modbus operate==========================
void QSysData::SendToModelBus(const DataMap* pMap_)
{
   if (pMap_ == NULL)
       return;

   if (pMap_->iAddr < 0)
        return;

   BtyeToInt _Val, _ValSend;
   _Val.iVal = ParamData(pMap_->iNo);
   _ValSend.cByte[1] = _Val.cByte[0];
   _ValSend.cByte[0] = _Val.cByte[1];
   _ValSend.cByte[3] = _Val.cByte[2];
   _ValSend.cByte[2] = _Val.cByte[3];
   m_pModbus->PresetMultipleRegisters(pMap_->iAddr, 2, _ValSend.cByte);
}

const static int g_iByte2Int[] = {1, 0, 3, 2};
void QSysData::OnReadCoil(unsigned short addr_, unsigned short qty_,
                unsigned char*pData_, unsigned char byte_)
{
    //[高8bit][低8bit] = [short]
    //[低short][高short] = [int]
    //[byte1][byte0][byte3][byte2] = int
    int _nNo = GetParamAddrNo(addr_);
    if (_nNo >= 0 && qty_ > 1)
    {
        BtyeToInt _Val;
        if (qty_ > 32)
            qty_ = 32;
        if (byte_ > 4)
            byte_ = 4;

        _Val.iVal = 0;
        for (int _i = 0; _i < byte_; ++_i)
        {
            _Val.cByte[_i] = pData_[g_iByte2Int[_i]];
        }
        unsigned int _nMask = pow(2, qty_);
        _nMask -= 1;

        int _nOldVal = ParamData(_nNo);

        _nOldVal &= ~_nMask;
        _Val.iVal &= _nMask;
        _nOldVal = _nOldVal | _Val.iVal;
        ParamData(_nNo) = _nOldVal;
    }
}

void QSysData::OnReadRegisters(unsigned short addr_, unsigned short qty_,
                     unsigned char*pData_, unsigned char byte_)
{
    //[高8bit][低8bit] = [short]
    //[低short][高short] = [int]
    //[byte1][byte0][byte3][byte2] = int
    int _nNo = GetParamAddrNo(addr_);
    if (qty_ == byte_ * 2 && _nNo >= 0)
    {
        BtyeToInt _Val;
        int _nIndex = 0;
        for(int _i  = 0; _i < qty_ / 2; ++_i)
        {
            _Val.cByte[0] = pData_[_nIndex + 1];
            _Val.cByte[1] = pData_[_nIndex];
            _Val.cByte[2] = pData_[_nIndex + 3];
            _Val.cByte[3] = pData_[_nIndex + 2];
            ParamData(_nNo) = _Val.iVal;
            _nNo++;
            _nIndex += 4;
        }
    }
}




//===========================file operater===================
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

    QString _fix = path_.right(4);
    if (_fix == FILE_SUFFIX)
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
    int _nSize = _dxf.m_vShapeList.size();
    m_vDrillData.clear();
    m_vDrillData.resize(_nSize);

    for (int _i = 0; _i < _nSize; ++_i)
       m_vDrillData[_i] = _dxf.m_vShapeList.at(_i);

}

void QSysData::LoadFromXtfFile(const QString& path)
{
    CXtfReader _xtf;
    if( _xtf.OpenConfigFile(path))
        _xtf.StartParseFile(1);
}


void QSysData::SortDxfData()
{
     float  _fTemp,_fGap ;
     int _aRow[HOLE_POS::ROW_MAX] = {0};
     int _nClone = 0, _k = 0;
     INT_POINT _fSortData[HOLE_POS::ROW_MAX][HOLE_POS::HOLE_ROW_MAX];

     INT_POINT _fP;
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

     int _nAddr = HOLE_POS::ADDR_ROW0;
     for (int _i = 0; _i <_nClone; ++_i)
     {
            if (_aRow[_i] > 0)
            {
                 SortOneRow(_fSortData[_i] , _aRow[_i]);

                 for (int _j = 0; _j < _aRow[_i]; ++_j)
                 {
                     //Cmd06WriteKeepReg(_nAddr + _j, _fSortData[_i][_j].x);
                     //(_nAddr + _j + 1,  _fSortData[_i][_j].y);
                 }
            }
            _nAddr += HOLE_POS::HOLE_ROW_MAX;
      }

}

 void QSysData::SortOneRow(INT_POINT* p_,  int nSize_)
 {
        int _fMin;
        int _nPos;
        INT_POINT _fT;
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
