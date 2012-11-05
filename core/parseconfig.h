#ifndef  __PARSECONFIG_H__
#define __PARSECONFIG_H__
 #include <stack>
#include <QFile>
#include  "./../ghead.h"

//参数结构体
//该结构体不用每次保存
struct DataMap
{
       int iNo; //参数号 ，索引参数
       int iAddr;//地址，发送地址
       int iGroup; //组属性
       int iFraction;//小数点位数
       double dMult; //显示发大倍数
       string  strUnit;  //单位
       string  strText;
};

//控件通过自身来关联和获取数据
//通过该结构体，可以获取具体数据
//保存和操作数据，索引最大值299 最小值0

struct ConfigData
{
    //for new protecol
    int  iParamRI[PAGE_PARAM_COUNT]; //用于输入
    int  iParamRO0[PAGE_PARAM_COUNT]; //用于输出
    int  iParamRO1[PAGE_PARAM_COUNT]; //用于输出
    int  iParamRO2[PAGE_PARAM_COUNT]; //用于输出
    int  iParamRO3[PAGE_PARAM_COUNT]; //用于输出
    int  iParamRBtn[PAGE_PARAM_COUNT]; //用于按键发送

    vector<DataMap*> m_pArrayData;
};


//过程如下：
//1.读取一个有效行
//2.跳过空行
//3.段开始，压栈，进入段
//4.段结束，弹栈，进入段
//5.数据，解释数据，数据插入队列
class CParseConfig
{
public:
    enum Token
    {
        INVALID_T = -1,
        PARA_LINE,
        DATA_BEGIN,
        DATA_END,
        BASE_ADDR,
        GROUP_INFO,
        TOKEN_NUM,
    };
    CParseConfig();
    ~ CParseConfig();
    bool OpenConfigFile(const char* szPath_);
    void CloseFile();
    bool StartLoadConfig(ConfigData* pData_);

private:
    bool LoadLine(); //读取一行。注释行跳过
    bool ReadToken(const string& strToken_);  //读取对应标记
    Token GetLineToken();

    void EnterDataBegin();
    void LeaveDataEnd();
    void GetParam();
    void GetGroupInfo();
    void GetBaseAddr();

    bool GetDouble(double& dRet_);
    bool GetInt(int& nRet_);

private:
    int m_nBaseAddr;
    int m_nGroup;
    stack<int> m_stkSeg;

    string m_strCurLine;
    long m_nFileSize;
    int m_nLinePos;
    QFile* m_pFile;
    ConfigData* m_pConfigData;
};

/*
DATABEGIN  //数据开始

GROUP       1
BASEADDR   111

#1=23.12  ADDR = 222 FRAC = 2 MUL = 1.0 UNIT = "mm/s" TEXT = "测试参数 速度"

#3=3.12  ADDR = 32 FRAC = 2 MUL = 1.0 UNIT = "mm" TEXT = "测试参数 距离"

DATAEND
*/
#endif // PARSECONFIG_H















