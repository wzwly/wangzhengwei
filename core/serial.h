#ifndef SERIAL_H
#define SERIAL_H
#include <QObject>
/*
关于数据收发的构思：
1.认为写入过程正确
2.读过程
--设置一个足够大的超时，一旦超时，认为没有读到数据；
--读到一个数据，则开始往下读完一帧数据；
*/

class QSerial:public QObject
{
public:
    static QSerial* Instance(int nId_ = 0);
    ~QSerial();
private:
    QSerial(int nId_ = 0);

private:
    static int m_nFdCom;
private:
    void InitSerial(int nId_);

public:
   static bool WriteData(const unsigned char* p_, int size_);
   static int ReadData(unsigned char* p_, int size_);
   static bool ReadChar(unsigned char* p_);
};

#endif // SERIAL_H
