#include "serial.h"
//#include "./../ghead.h"
//
//#include "cmddef.h"

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


QSerial::TxRxBuffer QSerial::m_gTxRxBuffer;

const static char* g_szSerialName[] = {"/dev/ttyUSB0", "/dev/ttySAC0", "/dev/ttySAC1"};

QSerial::QSerial(DevMaster* pSlave_, QObject * p_)
{
    m_nFdModbus = -1;
    m_pModbus = pSlave_;
    InitModbus();
    m_nTimer = startTimer(1); //开启定时器，1ms一次
}

QSerial::~QSerial()
{
    Close(m_nFdModbus);
}

void QSerial::InitModbus()
{
    if ((m_nFdModbus = Open(g_szSerialName[0], O_RDWR | O_NOCTTY)) < 0)
    {
        assert(false);
        perror(g_szSerialName[0]);
        return ;
    }
    struct termios _opt;

    bzero(&_opt, sizeof(_opt)); //清除结构体以放入新的序列埠设定值
    Tcgetattr(m_nFdModbus, &_opt);
    _opt.c_cc[VMIN] = 1;
    _opt.c_iflag = 0;
    _opt.c_oflag = 0;
    _opt.c_lflag = 0;
    _opt.c_cflag	= B115200 | CS8 | PARENB | CLOCAL | CREAD;
    if (Tcsetattr(m_nFdModbus, TCSANOW, &_opt))
    {
        perror("tcsetattr   error");
        assert(false);
        exit(1);
    }   

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
        m_gTxRxBuffer.szRxBuffer[m_gTxRxBuffer.iRxLen++] = _cRead[_i];
    }
    m_nTemMs = 10;
}


void QSerial::SendBuffer()
{
    int _nSendLen = m_gTxRxBuffer.iTxLen;
    if (_nSendLen >0)
    {
        int _nRet = Write(m_nFdModbus,m_gTxRxBuffer.szTxBuffer ,_nSendLen);
        if (_nRet != _nSendLen)
        {
            //deal send erro
        }
    }
}


void QSerial::timerEvent(QTimerEvent *event_)
{
    if (m_gTxRxBuffer.m_nEchoTimeOut > 0)
        m_gTxRxBuffer.m_nEchoTimeOut--;

    if (m_gTxRxBuffer.bRxTimerEn)
    {
        //in receive mode
        if (m_nTemMs > 0 && m_gTxRxBuffer.iRxLen > 0)
        {
            --m_nTemMs;
            if (m_nTemMs == 0)
            {
                m_gTxRxBuffer.iRxLen = 0;
                //receive time out, start a new package
            }
        }
    }
}



