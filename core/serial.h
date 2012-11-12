#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <QObject>

#if ARM
    #define  Write(a, b, c)  write(a, b, c)
    #define  Ioctl(a,b,c)   ioctl(a, b, c)
    #define  Open(a,b) open(a, b)
    #define  Read(a, b, c) read(a, b, c)
    #define  Close(a)       close(a)
    #define Mmap(a, b, c, d, e, f)  mmap(a, b, c, d, e, f)
    #define Munmap(a, b)     munmap(a, b)
    #define Tcgetattr(a, b)     tcgetattr(a, b)
    #define Tcsetattr(a, b, c)  tcsetattr(a, b,c)
#else
    #define  Write(a, b, c)  (c)
    #define  Ioctl(a,b,c)   0
    #define  Open(a, b)   1
    #define  Read(a, b, c)  c; memset(b, 1, c)
    #define  Close(a)
    #define Mmap(a, b, c, d, e, f)  malloc(b)
    #define Munmap(a, b)     free(a)
    #define Tcgetattr(a, b)
    #define Tcsetattr(a, b, c)  0
#endif


class DevMaster;
class QSerial : public QObject
{
   Q_OBJECT
public:
    QSerial(DevMaster* pSlave_,QObject * p_);
    ~QSerial();
    enum
    {
        MAX_BUFFER_SIZE = 128,
    };

    struct TxRxBuffer
    {
        unsigned char szTxBuffer[MAX_BUFFER_SIZE];
        unsigned char szRxBuffer[MAX_BUFFER_SIZE];
        int  iRxLen;
        int  iTxLen;
        bool bRxTimerEn;
        int m_nEchoTimeOut;
        TxRxBuffer()
        {
            iRxLen = 0; iTxLen = 0;
            bRxTimerEn = true;
        };
    };

protected:
    void InitModbus();
    void timerEvent(QTimerEvent *event_); //定时器响应函数

private slots:
    void OnReceiveChar();

private:
    int m_nFdModbus;//
    int m_nTimer;
    int m_nTemMs;
    DevMaster* m_pModbus;
public:
    static TxRxBuffer m_gTxRxBuffer;
    void SendBuffer();
    //void ClearReceive(){ m_gTxRxBuffer.iRxLen = 0;}
};

#endif // SERIAL_H
