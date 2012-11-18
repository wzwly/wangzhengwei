#include "dxfreader.h"
#include "../ghead.h"

QDxfReader::QDxfReader()
{
    m_pFile = NULL;
    m_nFileSize = 0;
    m_nLine = 0;
}


QDxfReader::~QDxfReader()
{
    Close();
}


bool QDxfReader::Open(const QString& szFileName_)
{
    if (m_pFile != NULL)
        Close();

    m_pFile = new QFile(szFileName_);
    if (m_pFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_strFileName = szFileName_;
        m_nFileSize = m_pFile->size();
        return true;
    }
    else
       return false;
}

void QDxfReader::Close()
{
    if (m_pFile)
    {
            m_pFile->close();
            m_pFile = NULL;
    }
    m_nFileSize = 0;

    m_vShapeList.clear();
    return;
}



//读一行的时候 包含了回车换行
bool QDxfReader::ReadNextNode(int nToken_)
{
        m_cNodeVal.Reset();

        char _aRead[MAX_LINE_READ + 1];
        QString _strLine;
        int _nToken = -1;
        int _nTemp = 0;
        bool _bNum = false;

        for (; ;) //读到一个标记为0 或者为2的值
        {
                int _nRet = m_pFile->readLine(_aRead, MAX_LINE_READ);
                if (_nRet > 0)
                {
                        _strLine = QString(_aRead);
                        _strLine = _strLine.trimmed();
                        _nRet = _strLine.size();

                        if (_nRet == 1 || _nRet == 2)
                        {
                                _nTemp = _strLine.toInt(&_bNum);
                                if (_bNum && _nTemp == nToken_)
                                {
                                        m_cNodeVal.nToken = nToken_;
                                        break;
                                }

                        }

                }
                else
                        break;
        }
        //读取标记的具体名字
        if (m_cNodeVal.nToken >= 0)
        {
                m_pFile->readLine(_aRead, MAX_LINE_READ);
                _strLine = QString(_aRead);
                m_cNodeVal.strCurLine = _strLine.trimmed();
                //qDebug() <<m_cNodeVal.strCurLine;
        }

        return m_cNodeVal.nToken != -1;
}

bool QDxfReader::Match(const char* pMark_)
{
        QString _str(pMark_);
        return _str == m_cNodeVal.strCurLine;
}

bool QDxfReader::StartAnalyse()
{
    bool _bMach = true;

    do{
            _bMach = ReadNextNode(2);
    }while(_bMach && !Match("ENTITIES"));

    if (!_bMach) //错误文件
            return false;
    do
    {
            _bMach = ReadNextNode(0);
            if (Match("CIRCLE"))
            {
                    //处理圆弧
                    _bMach = DealCircles();
                    if (!_bMach)
                        return false;
            }
            else if (Match("LINE"))
            {
                    //处理直线
                    // _bMach =DealLines();
            }
            else if (Match("ENDSEC"))
            {
                    //处理矩形
            }
            else if (Match("LWPOLYLINE"))
            {
                    //处理多边形
                    // _bMach = DealPolyLine();
            }
            else if (Match("ENDSEC"))
                    _bMach = false;

    }while(_bMach);//之前不匹配 且到了文件结束
    return true;
}


bool QDxfReader::DealCircles()//圆弧处理
{
        bool _bRet = true;
        bool _bDigit = false;
        float _x = 0, _y = 0, _z = 0, _r = 0;

        _bRet = ReadNextNode(10);
        if (_bRet)
                _x = m_cNodeVal.strCurLine.toFloat(&_bDigit);


        if (_bRet && _bDigit)
        {
                _bRet = ReadNextNode(20);
                if (_bRet)
                        _y = m_cNodeVal.strCurLine.toFloat(&_bDigit);
        }

        if (_bRet && _bDigit)
        {
                _bRet = ReadNextNode(30);
                if (_bRet)
                        _z = m_cNodeVal.strCurLine.toFloat(&_bDigit);
        }

        if (_bRet && _bDigit)
        {
                _bRet = ReadNextNode(40);
                if (_bRet)
                        _r = m_cNodeVal.strCurLine.toFloat(&_bDigit);
                if(_bDigit)
                {
                        INT_POINT _p(_x,_y);
                        m_vShapeList.push_back(_p);
                }
        }
        return _bRet && _bDigit;
}
