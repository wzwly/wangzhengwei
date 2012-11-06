#include "serial.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>


#include "./../ghead.h"

int QSerial::m_nFdCom = -1;

QSerial* QSerial::Instance(int nId_)
{
    static QSerial* _s_pCom = NULL;
    if (_s_pCom == NULL)
    {
        _s_pCom = new QSerial(nId_);
        g_pListObject.push_back(_s_pCom);
    }

    return _s_pCom;
}

QSerial::QSerial(int nId_)
{
    InitSerial(nId_);
}

QSerial::~QSerial()
{

    close(m_nFdCom);
}

const static char* g_szSerialName[] = {"/dev/ttyUSB0", "/dev/ttySAC0", "/dev/ttySAC1"};

void QSerial::InitSerial(int nId_)
{
    struct termios _opt;
    int _fd = 1;
#if PC_BEBUG
    _fd = 0;
    return;
#else
    _fd = 1;
#endif
    _fd = m_nFdCom = open(g_szSerialName[_fd], O_RDWR | O_NONBLOCK);    //默认为阻塞读方式
   if(_fd == -1)
   {
       perror("open serial 0\n");
       assert(false);
   }

  tcgetattr(_fd, &_opt);
  cfsetispeed(&_opt, B9600);
  cfsetospeed(&_opt, B9600);

  if(tcsetattr(_fd, TCSANOW, &_opt) != 0 )
  {
      perror("tcsetattr error");
      assert(false);
  }

  _opt.c_cflag &= ~CSIZE;
  _opt.c_cflag |= CS8;  //8bit
  _opt.c_cflag &= ~CSTOPB; //1bit stop bit
  _opt.c_cflag &= ~PARENB; //无校验
  _opt.c_cflag |= (CLOCAL | CREAD);

  _opt.c_cc[VTIME] = 1;
  _opt.c_cc[VMIN] = 0;

  //tcflush(_fd, TCIOFLUSH);


  if(tcsetattr(_fd, TCSANOW, &_opt) != 0)
  {
      perror("serial error");
      assert(false);
  }
  qDebug() << "Com ID" <<_fd;
}


bool QSerial::WriteData(const unsigned char* p_, int size_)
{
#if PC_BEBUG
    int _nRet = size_;
#else
   int _nRet = write(m_nFdCom, p_, size_);
#endif
    return (size_ == _nRet);
}

int QSerial::ReadData(unsigned char* p_, int size_)
{

    int _nRet = 0, _nCnt = 0;
#if PC_BEBUG
     memset(p_, 1,size_);
     return size_;
#else
    _nRet = read(m_nFdCom, p_++, 1);
#endif
    while(_nCnt < size_ && _nRet > 0)
    {
        ++_nCnt;
        _nRet = read(m_nFdCom, p_++, 1);
    }
    return _nCnt;
}

bool QSerial::ReadChar(unsigned char* p_)
{
#if PC_BEBUG
    *p_ = '1';
    return true;
#else
    int _nRet = read(m_nFdCom, p_, 1);
    return (_nRet > 0);
#endif
}




