#ifndef __SERIAL_H__
#define __SERIAL_H__
#include <QObject>
#include <QVector>
#include <QSemaphore>

//该宏定义硬件的操作，因此，对硬件的操作函用这里的宏来写，就不用加编译条件了

class DevMaster;
class QSerial : public QObject
{
    Q_OBJECT
public:
    QSerial(QObject * p_);
    ~QSerial();
    enum
    {
        TIMER_OUT = 20,
        MAX_BUFFER_SIZE = 128,
    };
    struct RxBuffer
    {
        unsigned char szRxBuffer[MAX_BUFFER_SIZE];
        int  iRxLen;
    };

    struct CmdSend
    {
        unsigned char szTxBuffer[MAX_BUFFER_SIZE];
        unsigned int iTxLen;
        unsigned int iTimerOut;
        unsigned int iLoop;
        bool  bDealRecv;
        CmdSend(unsigned char* pBuffer_, unsigned int iLen_)
        {
            memcpy(szTxBuffer, pBuffer_, iLen_);
            iTxLen = iLen_;
            iTimerOut = TIMER_OUT;
            iLoop = 3;
            bDealRecv = false;
        }
        CmdSend()
        {
            iTimerOut = TIMER_OUT;
            iLoop = 3;
            bDealRecv = false;
        }
    };

protected:
    void InitModbus();
    void timerEvent(QTimerEvent *event_); //定时器响应函数
    void SendBuffer(CmdSend* pCmd_);
    void SendListCmd();
private slots:
    void OnReceiveChar();
private:
    int m_nFdModbus;//
    int m_nTimer;
    DevMaster* m_pMaster;
    QVector<CmdSend*> m_vCmdVector;
    int m_nTemMs;
    CmdSend* m_pCurCmd;
public:
    RxBuffer m_gRxBuffer;
    const CmdSend* GetCurSend() {return m_pCurCmd;}
    void  StopTimer() {m_pCurCmd->bDealRecv = true;}

    void SetModbus(DevMaster* p_){ m_pMaster = p_;}
    void AddCmdSend(CmdSend* pCmd_);
    void DeleteCmdSend();
};
#endif // SERIAL_H
