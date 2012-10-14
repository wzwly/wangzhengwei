#ifndef _CMD_DEF_H__
#define _CMD_DEF_H__

#if 0
【1】定义
--假设存储空间是从ADDR_START开始，以下参数都是以该地址的偏移。

【2】通用参数
--全部是float类型，每个参数占4字节；
--分为：系统参数、限位参数、速度参数、显示参数；
  [1]数据大小为2K=2048字节；
  [2]则每组数据大小为512字节，对应存储128个参数；
  [3]偏移地址分别为0,512,1024,1536
  [4]组内偏移地址为参数的编号；

【3】控制信息
--全部为short类型；
  [1]数据大小分配1K=1024；
  [2]前面512字节存储控制数据,128个；
  [3]后面512字节存储控制命令，256个;
  [3]偏移地址2048；


【4】钻孔参数
--全部是float类型；
--每排孔最多512字节，128个孔；
--每排孔的实际个数由控制信息确定；
--如果单排孔查过128个，则分开为多排；
  [1]每排空512字节，最多128个孔；
  [2]N排孔连续存储，空间依次排开；
  [3]起始偏移地址3072；
#endif

//========================================
//========================================
//========================================
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
void Cmd02ReadInput(unsigned short wAddr_, unsigned short wNum_, unsigned char* pSrc_);

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
void Cmd05WriteOutput(unsigned short wAddr_, bool bOn_);

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
void Cmd03ReadKeepReg(unsigned short wAddr_, unsigned short wRegQty_, unsigned char* pDest_);


/*
06(0x06)写单个寄存器
【用来读取浮点数值和16位整形数值】
请求码格式：[0x06][地址高字节][地址低字节][2字节数据][Crc低字节][Crc高字节]
响应码格式：[0x06][地址高字节][地址低字节][2字节数据][Crc低字节][Crc高字节]
异常响应格式：[0x86][异常码]
*/
void Cmd06WriteKeepReg(unsigned short wAddr_, unsigned short wVal_);
void Cmd06WriteKeepReg(unsigned short wAddr_, float fVal_);
void Cmd06WriteKeepReg(unsigned short wAddr_, int nVal_);

/*
16(0x10)写多个寄存器
【用来读取浮点数值和16位整形数值】
请求码格式：[0x10][地址高字节][地址低字节][寄存器数量高字节][寄存器数量低字节][字节数][N字节数据][Crc低字
节][Crc高字节]
响应码格式：[0x10][地址高字节][地址低字节]寄存器数量高字节][寄存器数量低字节[Crc低字节][Crc高字节]
异常响应格式：[0x90][异常码]
*/
void CmdWriteKeepReg(unsigned short wAddr_, unsigned short wRegQty_, const unsigned char* pDest_);
void CmdWriteKeepRegEx(unsigned short wAddr_, unsigned short wRegQty_, const unsigned char* pDest_);
#endif
