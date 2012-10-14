#include "configset.h"
#include <QFile>
#include "assert.h"

static const char*  g_szConfigPath = "./Config.cnf";

int GlbGetClassKey(const char* p_ )
{
    int _key = 0;
    int _n , _i = 0;
    while(*p_ != '\0')
    {
            _n = *p_++;
            _n = _n << _i++;
            _key |= _n;
    }
    return _key;
}

QConfigSet::QConfigSet(QIODevice * pD_)
 :QDataStream(pD_)
{

}


QConfigSet::~QConfigSet()
{

}

bool QConfigSet::LoadClassData(int * pBegin_, int* pEnd_, const char* Class_)
{
    int _nLen = int(pEnd_) - int(pBegin_);
    int _nRet = readRawData((char*)pBegin_, _nLen);
    return  _nRet == _nLen;
}


bool QConfigSet::SaveClassData(int * pBegin_, int* pEnd_, const char* Class_)
{
    int _nLen = int(pEnd_) - int(pBegin_);
    int _nRet = writeRawData((const char*)pBegin_, _nLen);
    return  _nRet == _nLen;
}


bool QConfigSet::ReadFromFile(const QString& szFilePath_, void* pAddr_, unsigned int nSize_, int nOffset_)
{
    bool _bRet = false;
     QFile _file(szFilePath_);

      _bRet = _file.open(QIODevice::ReadOnly);
      if (!_bRet)
     {
           assert( _bRet);
          return  _bRet;
     }
      _file.seek(nOffset_);
      QConfigSet  _out(&_file);  //we will serialize the data into the file    
      int _nRet = _out.readRawData((char*)pAddr_, nSize_);
      return  _nRet == nSize_;
}
bool QConfigSet::ReadFromFile(const char* szFilePath_, void* pAddr_, unsigned int nSize_, int nOffset_)
{
        return ReadFromFile(QString(szFilePath_),  pAddr_, nSize_,  nOffset_);
}


bool QConfigSet::WriteToFile(const QString& szFilePath_,const  void* pAddr_, unsigned int nSize_, int nOffset_)
{
    bool _bRet = false;
     QFile _file(szFilePath_);
      _bRet = _file.open(QIODevice::WriteOnly);
      if (!_bRet)
     {
           assert( _bRet);
          return  _bRet;
     }
      _file.seek(nOffset_);
      QConfigSet  _out(&_file);  //we will serialize the data into the file
      int _nRet = _out. writeRawData((const char*)pAddr_, nSize_);
      return  _nRet == nSize_;
}

bool QConfigSet::WriteToFile(const char* szFilePath_, const void* pAddr_, unsigned int nSize_, int nOffset_ )
{
        return WriteToFile(QString(szFilePath_),  pAddr_, nSize_,  nOffset_);
}
//===========================================
//=============================================
bool LoadClassData(int * pBegin_, int* pEnd_, const char* Class_)
{
        bool _bRet = false;
        static int _s_nLoadCfg = 0;
         QFile _file(g_szConfigPath);
          _bRet = _file.open(QIODevice::ReadOnly);
          if (!_bRet)
         {
              return  _bRet;
         }
         _file.seek(_s_nLoadCfg);
         QConfigSet  _out(&_file);  //we will serialize the data into the file
         _bRet = _out.LoadClassData(pBegin_,  pEnd_,  Class_);
           _s_nLoadCfg +=  (int(pEnd_) - int(pBegin_));
         _file.close();
         int _crc =  GlbGetClassKey(Class_);
         if (_crc != (*pBegin_))
              _bRet = false;

          return  _bRet;
}


 bool SaveClassData(int * pBegin_, int* pEnd_, const char* Class_)
 {
      bool _bRet = false;
      QFile _file(g_szConfigPath);
     static int _s_nSaveCfg = 0;
     if (_s_nSaveCfg == 0)
          _file.resize(0);

      _bRet =  _file.open(QIODevice::Append | QIODevice::WriteOnly);
      if (!_bRet)
     {
           assert( _bRet);
          return  _bRet;
      }
      *pBegin_ =  GlbGetClassKey(Class_);
       QConfigSet  _out(&_file);  //we will serialize the data into the file
       _bRet = _out.SaveClassData(pBegin_,  pEnd_,  Class_);
        _s_nSaveCfg += (int(pEnd_) - int(pBegin_));
       _file.close();
       return  _bRet;
 }

