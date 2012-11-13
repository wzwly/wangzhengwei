#include "cmddef.h"

#include <QWSServer>
#include <QSocketNotifier>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <termios.h>	//串口头文件
#include <assert.h>


#if 1 //ARM
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



//#define szModbusCom "/dev/ttySAC0"
#define szModbusCom "/dev/ttyUSB0"
//#define szModbusCom "/dev/ttyS2"

QSerial::QSerial(QObject * p_)
{
    m_nFdModbus = -1;
    m_pCurCmd = NULL;
    InitModbus();
    m_nTimer = startTimer(10); //开启定时器，1ms一次
}

QSerial::~QSerial()
{
    Close(m_nFdModbus);
}

void QSerial::InitModbus()
{
    if ((m_nFdModbus = Open(szModbusCom, O_RDWR | O_NONBLOCK)) < 0)
    {
        assert(false);
        perror(szModbusCom);
        return ;
    }
    struct termios _opt;
    bzero(&_opt, sizeof(_opt)); //清除结构体以放入新的序列埠设定值
    Tcgetattr(m_nFdModbus, &_opt);
    _opt.c_cc[VMIN] = 1;
    _opt.c_iflag = 0;
    _opt.c_oflag = 0;
    _opt.c_lflag = 0;
    _opt.c_cflag	= B115200 | CS8 | CLOCAL | CREAD;
    if (Tcsetattr(m_nFdModbus, TCSANOW, &_opt))
    {
        perror("tcsetattr   error");
        assert(false);
        exit(1);
    }
    //注册响应
    QSocketNotifier* notify = new QSocketNotifier(m_nFdModbus, QSocketNotifier::Read, this);
    connect(notify, SIGNAL(activated(int)), this, SLOT(OnReceiveChar()));
}


void QSerial::OnReceiveChar()
{
    unsigned char _cRead[10];
    int _ret = Read(m_nFdModbus, _cRead, 10);

    if (_ret <= 0)
        return;

    for (int _i = 0; _i < _ret; ++_i)
    {
        if (m_gRxBuffer.iRxLen < MAX_BUFFER_SIZE)
            m_gRxBuffer.szRxBuffer[m_gRxBuffer.iRxLen++] = _cRead[_i];
    }
    m_pMaster->CheckCommModbus();
    m_nTemMs = 10;
}

void QSerial::timerEvent(QTimerEvent *event_)
{
    if (m_nTemMs > 0 && m_gRxBuffer.iRxLen > 0)
    {
        --m_nTemMs;
        if (m_nTemMs == 0)
        {
            m_gRxBuffer.iRxLen = 0; //receive time out, start a new package
        }
    }

    if (m_pCurCmd != NULL)
    {
        if (m_pCurCmd->bDealRecv)
            return; //已经接正确
        if (m_pCurCmd->iTimerOut > 0)
            --m_pCurCmd->iTimerOut;
        if (m_pCurCmd->iTimerOut == 0)
        {
            if (m_pCurCmd->iLoop > 0)
            {
                m_pCurCmd->iTimerOut = TIMER_OUT;
                m_pCurCmd->iLoop--;
                SendBuffer(m_pCurCmd);
            }
            else
            {  //超时
                DeleteCmdSend();
            }
        }

    }
    else
        SendListCmd(); //理论上 这里很少进入

}

void QSerial::SendBuffer(CmdSend* pCmd_)
{
    int _nSendLen = pCmd_->iTxLen;
    if (_nSendLen >0)
    {
        int _nRet = Write(m_nFdModbus,pCmd_->szTxBuffer ,_nSendLen);
        if (_nRet != _nSendLen)
        {
            //deal send erro
        }
        _nRet++;
    }
    m_gRxBuffer.iRxLen = 0;
}

void QSerial::SendListCmd()
{
    if (m_pCurCmd != NULL)
        return;

    int _nSize = m_vCmdVector.size();
    if (_nSize > 0)
    {
        m_pCurCmd = m_vCmdVector.first();
        m_vCmdVector.pop_front();
    }
}


void QSerial::DeleteCmdSend()
{
    if (m_pCurCmd->iLoop > 0)
        qDebug() << "Delete One Cmd by Ok:" << int(m_pCurCmd->szTxBuffer[1]);
    else
        qDebug() << "Delete One Cmd by timeout:" << int(m_pCurCmd->szTxBuffer[1]);

    if (m_pCurCmd != NULL)
        delete m_pCurCmd;
    m_pCurCmd = NULL;
    SendListCmd();
}

 void QSerial::AddCmdSend(CmdSend* pCmd_)
 {
     m_vCmdVector.push_back(pCmd_);
     if (m_pCurCmd == NULL)
        SendListCmd();
 }

