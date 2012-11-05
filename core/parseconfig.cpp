#include "parseconfig.h"

CParseConfig::CParseConfig()
{
    m_pFile = NULL;
    m_nFileSize = 0;
    m_pConfigData = NULL;

    m_nBaseAddr = -1;
    m_nGroup = -1;
}

 CParseConfig::~CParseConfig()
 {
          CloseFile();
 }


bool CParseConfig::OpenConfigFile(const char* szPath_)
{
    if (m_pFile != NULL)
       CloseFile();

    m_pFile = new QFile(szPath_);


    if (m_pFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_nFileSize = m_pFile->size();
        return true;
    }
    else
    {
        delete m_pFile;
        m_pFile = NULL;
       return false;
    }
}

void CParseConfig::CloseFile()
{
    if (m_pFile)
    {
            m_pFile->close();
            delete m_pFile;
            m_pFile = NULL;
    }
    m_nFileSize = 0;
}


 bool CParseConfig::LoadLine()
 {
        char _aRead[MAX_LINE_READ + 1];

        m_strCurLine.clear();
        int _i = 0;
        bool  _bHasChar = false;
        bool _bGetLine = false;
        while (1)
        {
                 _aRead[0] = '\0';
                 int _nRet = m_pFile->readLine(_aRead, MAX_LINE_READ);
                 if (_nRet == 1) //空白行
                     continue;
                 else if (_nRet > 1)//非空白
                 {
                        _bHasChar = false;
                        for (_i = 0; _i < _nRet - 1; ++_i)  //去掉注释
                        {
                                if (isspace(_aRead[_i]))
                                    continue;
                                else if (_aRead[_i] == '\/' && _aRead[_i + 1] == '\/')
                                {
                                        if (_bHasChar)
                                        {
                                                _aRead[_i] = '\0';
                                                _bGetLine = true;
                                                break;
                                        }
                                        else //注释行
                                        {
                                            m_strCurLine.clear();
                                            break;
                                        }
                                }
                                else
                                    _bHasChar = true;
                        }
                        if (_i == (_nRet - 1))
                        {
                                 _bGetLine = true;
                                 break;
                        }

                 }
                 else   
                     break;

                 if (_bGetLine)
                     break;
        }
        m_strCurLine. append(_aRead);  
        return  !m_strCurLine.empty();
 }



 bool CParseConfig::StartLoadConfig(ConfigData* pData_)
 {
        m_pConfigData = pData_;

        while(LoadLine())
        {       
            switch(GetLineToken())
            {
                    case PARA_LINE:
                       GetParam();
                        break;
                     case DATA_BEGIN:
                         EnterDataBegin();
                        break;
                     case DATA_END:
                        LeaveDataEnd();
                        break;
                     case BASE_ADDR:
                        GetBaseAddr();
                        break;
                     case GROUP_INFO:
                        GetGroupInfo();
                        break;
                     default:
                        assert(false);
                          break;
            }
        }

        return m_stkSeg.empty();
 }


 static const char* g_szTokenValue[] = {"#" , "DATABEGIN", "DATAEND", "BASEADDR", "GROUP"};

CParseConfig::Token CParseConfig::GetLineToken()
{
        Token  _token = INVALID_T;
        string::size_type  _tPos;

        for (int _i = 0; _i < CParseConfig::TOKEN_NUM; ++_i)
        {
             _tPos = m_strCurLine.find(g_szTokenValue[_i]);
             if (_tPos != string::npos)
             {
                    _token = (Token)_i;     
                    break;
             }
        }

        m_nLinePos = _tPos;
        return  _token;
}


 void CParseConfig::EnterDataBegin()
 {
        m_stkSeg.push(m_nBaseAddr);
        m_stkSeg.push(m_nGroup);
 }

 void CParseConfig::LeaveDataEnd()
 {
      m_nGroup =  m_stkSeg.top();
      m_stkSeg.pop();

      m_nBaseAddr = m_stkSeg.top();
      m_stkSeg.pop();
 }

 void CParseConfig::GetBaseAddr()
 {
        m_nLinePos += 8;  //"BASEADDR"
        int _addr;
        if (GetInt(_addr))
            m_nBaseAddr =_addr;
        qDebug() << "addr" << _addr;
 }

 void CParseConfig::GetGroupInfo()
 {
     m_nLinePos += 5; //"GROUP"
     int _grp;
     if (GetInt(_grp))
         m_nGroup = _grp;
     qDebug() << "group" << _grp;
 }
static const char* g_szItem[5] = {"ADDR", "FRAC", "MUL", "UNIT", "TEXT"};
 void CParseConfig::GetParam()
 {
    m_nLinePos += 1;
    int _nNo;
    double _dVal;
    if (GetInt(_nNo) && GetDouble(_dVal))
    {
        int _addr, _frac = 0;
        double _dMul = 1.0;
        int _i = 0;
        string _strUnit, _strText;
        string::size_type  _tPos;
        _tPos = m_strCurLine.find(g_szItem[_i++]);
        if (_tPos != string::npos)
        {
          m_nLinePos =  _tPos + 4;
          GetInt(_addr);
        }
        _tPos = m_strCurLine.find(g_szItem[_i++]);
        if (_tPos != string::npos)
        {
          m_nLinePos =  _tPos + 4;
          GetInt(_frac);
        }
        _tPos = m_strCurLine.find(g_szItem[_i++]);
        if (_tPos != string::npos)
        {
          m_nLinePos =  _tPos + 3;
          GetDouble(_dMul);
        }

        _tPos = m_strCurLine.find(g_szItem[_i++]);
        if (_tPos != string::npos)
        {
          m_nLinePos =  _tPos + 4;
          GetText(_strUnit);
        }

        _tPos = m_strCurLine.find(g_szItem[_i++]);
        if (_tPos != string::npos)
        {
          m_nLinePos =  _tPos + 4;
          GetText(_strText);
        }

        qDebug() <<"#" <<_nNo << "="<<_dVal << _addr << _frac << _dMul<< QString(_strUnit.data())<<QString(_strText.data());
    }
 }

 bool CParseConfig::GetDouble(double& dRet_)
 {
        char _ch;
        bool _bFind = false;
        char _cArray[32];
        int _i = 0;
        while(_i < 31)
        {
            _ch = m_strCurLine.at(m_nLinePos++);

            if (_ch == ' ' || _ch ==  '\t')
            {
                    if (_bFind)
                        break;
            }
            else if (isdigit(_ch))
            {
                 _cArray[_i++] = _ch;
                _bFind = true;
            }
            else
            {
                if (_bFind)
                    break;
                else if (_ch != '=')
                    return false;
            }
        }

        if (_i <= 0)
            return false;
        _cArray[_i] = '\0';
        dRet_ =  atof(_cArray);
        return true;
 }

bool CParseConfig::GetInt(int& nRet_)
{
    char _ch;
    bool _bFind = false;
    char _cArray[32];
    int _i = 0;
    while(_i < 31)
    {
        _ch = m_strCurLine.at(m_nLinePos++);

        if (_ch == ' ' || _ch ==  '\t')
        {
                if (_bFind)
                    break;
        }
        else if (isdigit(_ch))
        {
             _cArray[_i++] = _ch;
            _bFind = true;
        }
        else
        {
            if (_bFind)
                break;
            else if (_ch != '=')
                return false;
        }
    }
    if (_i <= 0)
        return false;
    _cArray[_i] = '\0';
    nRet_ =  atoi(_cArray);
    return true;
}


bool CParseConfig::GetText(string& str_)
{
    char _ch;
    bool _bFind = false;
    char _cArray[64];
    int _i = 0;
    while(_i < 63)
    {
        _ch = m_strCurLine.at(m_nLinePos++);

        if (_ch == '\n' || _ch == '\r')
            return false;
        if (_bFind) // find out the first '\"'
        {
            if (_ch == '\"')
                break;
            _cArray[_i++] = _ch;
        }
        else if (_ch == '\"')
            _bFind = true;
    }
    if (_i <= 0)
        return false;
    _cArray[_i] = '\0';
    str_.append(_cArray);
    return true;

}












