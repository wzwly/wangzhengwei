#include "cmddef.h"
#include <unistd.h>
#include "serial.h"
#include "./../ghead.h"

// CRC 高位字节值表
static unsigned char g_auchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
} ;


// CRC低位字节值表
static char g_auchCRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
0x43, 0x83, 0x41, 0x81, 0x80, 0x40
} ;

#define MAX_COMBUFF_LEN   264
#define MAX_REPEAT_COUNT   1


static unsigned char g_sRxBuff[MAX_COMBUFF_LEN];
static unsigned char g_sTxBuff[MAX_COMBUFF_LEN];


static unsigned char g_sComBuff[MAX_COMBUFF_LEN];
//CRC简单函数如下：
unsigned short GetCrc16(unsigned char *puchMsg,int usDataLen) // 消息中字节数
{
    unsigned char uchCRCHi = 0xFF ; // 高CRC字节初始化
    unsigned char uchCRCLo = 0xFF ; // 低CRC 字节初始化
    unsigned uIndex ; // CRC循环中的索引
    while (usDataLen--) // 传输消息缓冲区
    {
        uIndex = uchCRCHi ^ *puchMsg++ ; // 计算CRC
        uchCRCHi = uchCRCLo ^ g_auchCRCHi[uIndex] ;
        uchCRCLo = g_auchCRCLo[uIndex] ;
    }
    return (uchCRCHi << 8 | uchCRCLo) ;
}

void ExchangeByte(unsigned char* p_, int nSize_)
{
    unsigned char _tW;
    for(int _i = 0; _i < nSize_; _i += 2)
    {
        _tW = p_[_i];
        p_[_i] = p_[_i + 1];
        p_[_i + 1] = _tW;
    }
}

unsigned char GetAddr()
{
    return 1;
}

//=========================================
//=========================================
///========================================
/*
02(0x02)读离散量输入
【用了读取输入端口的状态】

请求码格式：[0x02][地址高字节][地址低字节][输入数量高字节][输入数量低字节][Crc低字节][Crc高字节]
响应码格式：[0x02][字节数][输入状态N*1个字节][Crc低字节][Crc高字节]
异常响应格式：[0x82][异常码]

备注：
--N = 输入数量/8,如果余数不为零，则N=N+1；
--输入数量为1-2000，超过算是错误；
*/
void Cmd02ReadInput(unsigned short wAddr_, unsigned short wNum_, unsigned char* pSrc_)
{
    int _nRpt = MAX_REPEAT_COUNT;
    int _nN = wNum_;
    wNum_ = _nN/ 8;
    if (_nN % 8 != 0)
        wNum_++;

    g_sTxBuff[0] = GetAddr();
    g_sTxBuff[1] = 0x02;
    g_sTxBuff[2] = (wAddr_ & 0xff00) >> 8;
    g_sTxBuff[3] = (wAddr_ & 0x00ff);
    g_sTxBuff[4] = (wNum_ & 0xff00) >> 8;
    g_sTxBuff[5] = (wNum_ & 0x00ff);

    unsigned short _crc, _crcT;
    _crc = GetCrc16(g_sTxBuff, 6);
    g_sTxBuff[6] = (_crc & 0x00ff);
    g_sTxBuff[7] = (_crc & 0xff00) >> 8;

    while(_nRpt--)
    {
        if(!QSerial::WriteData(g_sTxBuff, 8))
            assert(false);

        g_sRxBuff[0] = '\0';
        g_sRxBuff[1] = '\0';
        //读取前两个字节
        if (QSerial::ReadChar(&g_sRxBuff[0]))
        {
           QSerial::ReadChar(&g_sRxBuff[1]);
           QSerial::ReadChar(&g_sRxBuff[2]);
           if (g_sRxBuff[0] != g_sTxBuff[0] || g_sRxBuff[1] != g_sTxBuff[1])
               continue;
           QSerial::ReadData(&g_sRxBuff[3],g_sRxBuff[2] + 2);
           int _nLen = 3 + g_sRxBuff[2];
            _crc = GetCrc16(g_sRxBuff, _nLen);
            _crcT = g_sTxBuff[_nLen] | (g_sTxBuff[_nLen + 1] << 8);
            if (_crcT == _crc)
            {
                memcpy(pSrc_, &g_sTxBuff[3],wNum_);
                break;
            }
        }
    }
}

/*
05(0x05)写单个线圈
【用来写输出端口的状态】
请求码格式：[0x05][地址高字节][地址低字节][FF/00][00][Crc低字节][Crc高字节]
响应码格式：[0x05][地址高字节][地址低字节][FF/00][00][Crc低字节][Crc高字节]
异常响应格式：[0x85][异常码]

备注：
--ff00表示On，0000表示offf；
--输入数量为1-ffff，超过算是错误；
*/
void Cmd05WriteOutput(unsigned short wAddr_, bool bOn_)
{
    int _nRpt = MAX_REPEAT_COUNT;

    g_sTxBuff[0] = GetAddr();
    g_sTxBuff[1] = 0x05;
    g_sTxBuff[2] = (wAddr_ & 0xff00) >> 8;
    g_sTxBuff[3] = (wAddr_ & 0x00ff);
    g_sTxBuff[4] = bOn_? 0xff : 0;
    g_sTxBuff[5] = 0;

    unsigned short _crc, _crcT;
    _crc = GetCrc16(g_sTxBuff, 6);
    g_sTxBuff[6] = (_crc & 0x00ff);
    g_sTxBuff[7] = (_crc & 0xff00) >> 8;

    while(_nRpt--)
    {
        if(!QSerial::WriteData(g_sTxBuff, 8))
            assert(false);

        g_sRxBuff[0] = '\0';
        g_sRxBuff[1] = '\0';
        //读取前两个字节
        if (QSerial::ReadChar(&g_sRxBuff[0]))
        {
            QSerial::ReadData(&g_sRxBuff[1],7);
            _crcT = g_sTxBuff[6] | (g_sTxBuff[7] << 8);
            if (_crcT == _crc)
            {
                break;
            }
        }
    }
}


/*
03(0x03)读保持寄存器
【用来读取浮点数值和16位整形数值】
请求码格式：[0x03][地址高字节][地址低字节][寄存器数量高字节][寄存器数量低字节][Crc低字节][Crc高字节]
响应码格式：[0x03][字节数][N字节][Crc低字节][Crc高字节]
异常响应格式：[0x83][异常码]

备注：
--对于浮点数，N=4 * 寄存器数量， 对于16位整形， N=2*寄存器数量；
--返回是根据地址来区别，如果是浮点数地址，则返回4字节；
--对应16位整形，最多一次读125个；浮点数最多读60个；
*/
void Cmd03ReadKeepReg(unsigned short wAddr_, unsigned short wRegQty_, unsigned char* pDest_)
{
    int _nRpt = MAX_REPEAT_COUNT;

    g_sTxBuff[0] = GetAddr();
    g_sTxBuff[1] = 0x03;
    g_sTxBuff[2] = (wAddr_ & 0xff00) >> 8;
    g_sTxBuff[3] = (wAddr_ & 0x00ff);
    g_sTxBuff[4] = (wRegQty_ & 0xff00) >> 8;
    g_sTxBuff[5] = (wRegQty_ & 0x00ff);

    unsigned short _crc, _crcT;
    _crc = GetCrc16(g_sTxBuff, 6);
    g_sTxBuff[6] = (_crc & 0x00ff);
    g_sTxBuff[7] = (_crc & 0xff00) >> 8;

    while(_nRpt--)
    {
        if(!QSerial::WriteData(g_sTxBuff, 8))
            assert(false);

        g_sRxBuff[0] = '\0';
        g_sRxBuff[1] = '\0';
        //读取前两个字节
        if (QSerial::ReadChar(&g_sRxBuff[0]))
        {
           QSerial::ReadChar(&g_sRxBuff[1]);
           QSerial::ReadChar(&g_sRxBuff[2]);
           if (g_sRxBuff[0] != g_sTxBuff[0] || g_sRxBuff[1] != g_sTxBuff[1])
               continue;
           QSerial::ReadData(&g_sRxBuff[3],g_sRxBuff[2] + 2);
           int _nLen = 3 + g_sRxBuff[2];
            _crc = GetCrc16(g_sRxBuff, _nLen);
            _crcT = g_sTxBuff[_nLen] | (g_sTxBuff[_nLen + 1] << 8);
            if (_crcT == _crc)
            {
                ExchangeByte(&g_sTxBuff[3],g_sRxBuff[2]);//高低字节交换
                memcpy(pDest_, &g_sTxBuff[3], g_sRxBuff[2]);
                break;
            }
        }
    }
}

/*
06(0x06)写单个寄存器
【用来读取浮点数值和16位整形数值】
请求码格式：[0x06][地址高字节][地址低字节][2字节数据][Crc低字节][Crc高字节]
响应码格式：[0x06][地址高字节][地址低字节][2字节数据][Crc低字节][Crc高字节]
异常响应格式：[0x86][异常码]
*/
void Cmd06WriteKeepReg(unsigned short wAddr_, unsigned short wVal_)
{
    int _nRpt = MAX_REPEAT_COUNT;

    g_sTxBuff[0] = GetAddr();
    g_sTxBuff[1] = 0x06;
    g_sTxBuff[2] = (wAddr_ & 0xff00) >> 8;
    g_sTxBuff[3] = (wAddr_ & 0x00ff);
    g_sTxBuff[4] = (wVal_ & 0xff00) >> 8;
    g_sTxBuff[5] = (wVal_ & 0x00ff);

    unsigned short _crc, _crcT;
    _crc = GetCrc16(g_sTxBuff, 6);
    g_sTxBuff[6] = (_crc & 0x00ff);
    g_sTxBuff[7] = (_crc & 0xff00) >> 8;

    while(_nRpt--)
    {
        if(!QSerial::WriteData(g_sTxBuff, 8))
            assert(false);

        g_sRxBuff[0] = '\0';
        g_sRxBuff[1] = '\0';
        //读取前两个字节
        if (QSerial::ReadChar(&g_sRxBuff[0]))
        {
            QSerial::ReadData(&g_sRxBuff[1],7);
            _crcT = g_sTxBuff[6] | (g_sTxBuff[7] << 8);
            if (_crcT == _crc)
            {
                break;
            }
        }
    }
}

void Cmd06WriteKeepReg(unsigned short wAddr_, float fVal_)
{
    CmdWriteKeepRegEx(wAddr_, 1, (unsigned char*)&fVal_);
}

void Cmd06WriteKeepReg(unsigned short wAddr_, int nVal_)
{
    CmdWriteKeepRegEx(wAddr_, 1, (unsigned char*)&nVal_);
}

/*
16(0x10)写多个寄存器
【用来读取浮点数值和16位整形数值】
请求码格式：[0x10][地址高字节][地址低字节][寄存器数量高字节][寄存器数量低字节][字节数][N字节数据][Crc低字
节][Crc高字节]
响应码格式：[0x10][地址高字节][地址低字节]寄存器数量高字节][寄存器数量低字节[Crc低字节][Crc高字节]
异常响应格式：[0x90][异常码]
*/
void CmdWriteKeepReg(unsigned short wAddr_, unsigned short wRegQty_, const unsigned char* pDest_)
{
    //QSerial::WriteData(pDest_, wRegQty_);
    //return; //wzwtest
    int _nRpt = MAX_REPEAT_COUNT;

    assert(wRegQty_ < 125);
    g_sTxBuff[0] = GetAddr();
    g_sTxBuff[1] = 0x10;
    g_sTxBuff[2] = (wAddr_ & 0xff00) >> 8;
    g_sTxBuff[3] = (wAddr_ & 0x00ff);
    g_sTxBuff[4] = (wRegQty_ & 0xff00) >> 8;
    g_sTxBuff[5] = (wRegQty_ & 0x00ff);
    unsigned char _cLen = wRegQty_ * 2;
    g_sTxBuff[6] = _cLen;
    memcpy(&g_sTxBuff[7],pDest_,_cLen);
    ExchangeByte(&g_sTxBuff[7],wRegQty_ * 2);//高低字节交换
    _cLen += 7;

    unsigned short _crc, _crcT;
    _crc = GetCrc16(g_sTxBuff, _cLen);
    g_sTxBuff[_cLen] = (_crc & 0x00ff);
    g_sTxBuff[_cLen + 1] = (_crc & 0xff00) >> 8;

    while(_nRpt--)
    {
        if(!QSerial::WriteData(g_sTxBuff, _cLen + 2))
            assert(false);

        g_sRxBuff[0] = '\0';
        g_sRxBuff[1] = '\0';
        //读取前两个字节
        if (QSerial::ReadChar(&g_sRxBuff[0]))
        {
            QSerial::ReadChar(&g_sRxBuff[1]);
            if (g_sRxBuff[0] != g_sTxBuff[0] || g_sRxBuff[1] != g_sTxBuff[1])
                continue;
            QSerial::ReadData(&g_sRxBuff[2],6);
            _crc = GetCrc16(g_sRxBuff, 6);
            _crcT = g_sTxBuff[6] | (g_sTxBuff[7] << 8);
            if (_crcT == _crc)
            {
                break;
            }
        }
    }
}

void CmdWriteKeepRegEx(unsigned short wAddr_, unsigned short wRegQty_, const unsigned char* pDest_)
{
    int _nRpt = MAX_REPEAT_COUNT;

    assert(wRegQty_ < 60);
    g_sTxBuff[0] = GetAddr();
    g_sTxBuff[1] = 0x10;
    g_sTxBuff[2] = (wAddr_ & 0xff00) >> 8;
    g_sTxBuff[3] = (wAddr_ & 0x00ff);
    g_sTxBuff[4] = (wRegQty_ & 0xff00) >> 8;
    g_sTxBuff[5] = (wRegQty_ & 0x00ff);
    unsigned char _cLen = wRegQty_ * 4;
    g_sTxBuff[6] = _cLen;
    memcpy(&g_sTxBuff[7],pDest_,_cLen);
    ExchangeByte(&g_sTxBuff[7],wRegQty_ * 4);//高低字节交换
    _cLen += 7;

    unsigned short _crc, _crcT;
    _crc = GetCrc16(g_sTxBuff, _cLen);
    g_sTxBuff[_cLen] = (_crc & 0x00ff);
    g_sTxBuff[_cLen + 1] = (_crc & 0xff00) >> 8;

    while(_nRpt--)
    {
        if(!QSerial::WriteData(g_sTxBuff, _cLen + 2))
            assert(false);

        g_sRxBuff[0] = '\0';
        g_sRxBuff[1] = '\0';
        //读取前两个字节
        if (QSerial::ReadChar(&g_sRxBuff[0]))
        {
            QSerial::ReadChar(&g_sRxBuff[1]);
            if (g_sRxBuff[0] != g_sTxBuff[0] || g_sRxBuff[1] != g_sTxBuff[1])
                continue;
            QSerial::ReadData(&g_sRxBuff[2],6);
            _crc = GetCrc16(g_sRxBuff, 6);
            _crcT = g_sTxBuff[6] | (g_sTxBuff[7] << 8);
            if (_crcT == _crc)
            {
                break;
            }
        }
    }
}
