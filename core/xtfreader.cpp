#include "xtfreader.h"

CXtfReader::CXtfReader()
{
    m_nParamAddr = -1;
    m_cModeInfo.Reset();
    m_nReadedCnt = 0;
    m_ptDrillPos.resize(SEND_PARAM_CNT);
}


bool CXtfReader::StartParseFile(short nAddr_)
{
    m_nParamAddr = nAddr_;
    
    INT_PT _pt;
    while(LoadLine())
    {       
       if (GetXYXPos(_pt))
            m_ptDrillPos.at(m_nReadedCnt++) = _pt;

       if (m_nReadedCnt >= SEND_PARAM_CNT)
           WriteData();
    }
    WriteData();
}


void CXtfReader::WriteData()
{
    INT_PT _pt;
    for (int _i = 0; _i < m_nReadedCnt; ++_i)
    {
        _pt = m_ptDrillPos.at(_i);
        qDebug() << m_nParamAddr + _i<< "X:" << float(_pt.x) / 100.0<<"Y:"<< float(_pt.y) / 100.0;
    }
    m_nParamAddr += m_nReadedCnt;

    m_nReadedCnt = 0;
}


bool CXtfReader::GetXYXPos(INT_PT& p_)
{
    double _dX, _dY;
    string::size_type  _tPosX ,_tPosY;

    _tPosY = FindLabel('Y');
    if (_tPosY != string::npos)
    {
        m_nLinePos = _tPosY + 1;
        GetDouble(_dY);
        m_cModeInfo.cLastAxis = LABEL_asY;
        m_cModeInfo.cValid[LABEL_asY] = VALID_LABEL;
        p_.y =  _dY * g_fDataMult;
        m_cModeInfo.nXYZ[LABEL_asY] = p_.y;
    }

    _tPosX = FindLabel('X');
    if (_tPosX != string::npos)
    {
        m_nLinePos = _tPosX + 1;
        GetDouble(_dX);
        m_cModeInfo.cLastAxis = LABEL_asX;
        m_cModeInfo.cValid[LABEL_asX] = VALID_LABEL;
        p_.x =  _dX * g_fDataMult;
        m_cModeInfo.nXYZ[LABEL_asX] = p_.x;
    }

    if ((_tPosY == string::npos) && (_tPosX == string::npos))
    {

        if (m_cModeInfo.cLastAxis != INVALID_LABEL && FindFirstDigit(m_nLinePos))
        { 
            double _dVal;
            if (!GetDouble(_dVal))
                return false;
            if (LABEL_asY == m_cModeInfo.cLastAxis)
            {
                p_.y =  _dVal * g_fDataMult;
                m_cModeInfo.nXYZ[LABEL_asY] = p_.y;
            }
            else if (LABEL_asX == m_cModeInfo.cLastAxis)
            {
                p_.x =  _dVal * g_fDataMult;
                m_cModeInfo.nXYZ[LABEL_asX] = p_.x;
            }
            else
                return false;
        }
    }
    else if (_tPosX == string::npos)
    {
        if (m_cModeInfo.cValid[LABEL_asX] == VALID_LABEL)
        {
            p_.x = m_cModeInfo.nXYZ[LABEL_asX];
        }
        else
            return false;
    }
    else if (_tPosY == string::npos)
    {
        if (m_cModeInfo.cValid[LABEL_asY] == VALID_LABEL)
        {
            p_.y = m_cModeInfo.nXYZ[LABEL_asY];
        }
        else
            return false;
    }
    return true;
}

