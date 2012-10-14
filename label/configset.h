#ifndef     __CONFIGSET_H__
#define     __CONFIGSET_H__

#include <QDataStream>


class QConfigSet : public QDataStream
{
    public:
     QConfigSet(QIODevice * pD_);
     ~QConfigSet();

private:
     int* m_pBeginAddr;
     int* m_pEndAddr;
     const char* m_szClassName;

public:
     bool LoadClassData(int * pBegin_, int* pEnd_, const char* Class_);
     bool SaveClassData(int * pBegin_, int* pEnd_, const char* Class_);
     static bool ReadFromFile(const QString& szFilePath_, void* pAddr_, unsigned int nSize_, int nOffset_ = 0);
     static bool ReadFromFile(const char* szFilePath_, void* pAddr_, unsigned int nSize_, int nOffset_ = 0);
     static bool WriteToFile(const QString& szFilePath_,const  void* pAddr_, unsigned int nSize_, int nOffset_ = 0);
     static bool WriteToFile(const char* szFilePath_, const void* pAddr_, unsigned int nSize_, int nOffset_ = 0);
};


extern bool LoadClassData(int * pBegin_, int* pEnd_, const char* Class_);
extern bool SaveClassData(int * pBegin_, int* pEnd_, const char* Class_);

#endif // CONFIGSET_H
