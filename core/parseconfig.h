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
       float dVal;
       double dMult; //显示发大倍数

       string  strUnit;  //单位
       string  strText;
       DataMap()
       {
           iNo = 0;
           iAddr = 1;
           iGroup = 0;
           iFraction = 0;
           dMult = 1.0;
           dVal = 1.0;
       };

       DataMap(int iNo_, int iAddr_, int iGroup_, int iFrac_,
               double dMul_, double dVal_,
               const string& szUnit_, const string szText_)
       {
           iNo =  iNo_;
           iAddr = iAddr_;
           iGroup = iGroup_;
           iFraction = iFrac_;
           dMult = dMul_;
           strUnit = szUnit_;
           strText = szText_;
           dVal = dVal_;
       }
};

struct AlarmCmd
{
        int nCmd;
        string szText;
        AlarmCmd(int nCmd_, string& str_)
        {
            nCmd = nCmd_;
            szText = str_;
        }
};

//控件通过自身来关联和获取数据
//通过该结构体，可以获取具体数据
//保存和操作数据，索引最大值299 最小值0

struct ConfigData
{
    int* pData;
    bool bIsLoaded;
    vector<DataMap*> m_pArrayData;
    vector<AlarmCmd*> m_pAlarmInfo;
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
        ALARM_INFO,
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
    void GetAlarmInfo();

    bool GetDouble(double& dRet_);
    bool GetInt(int& nRet_);
    bool GetText(string& str_);

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
//注释行
// ‘//’注释符号
//
//整体格式
//#参数号=初始值 ADDR=地址 FRAC=小数 MUL=显示倍数 UNIT="单位" TEXT="参数说明"
//--参数号必须写，且不能重复
//--初始值最好写，否则默认值可能不正确，一但参数保存之后，初始值将不读取
//--小数不写时默认0
//--倍数不写时默认1
//--单位和参数说明不写时，默认为空
//参数的Group，分为6大组
//1组用于下位机向上更新显示的参数，如坐标，报警等。参数号1-50
//2-5组用于从上至下发送的参数。参数号51-250
//6组用于按键向下发送的命令。参数号251-300
//
//报警命令格式：
//!=命令  TEXT = "报警提示"
//
//

DATABEGIN  //数据开始

GROUP       1

#1=23.12  ADDR = 222 FRAC = 2 MUL = 1.0 UNIT = "mm/s" TEXT = "测试参数 速度"
#3=3.12  ADDR = 32 FRAC = 2  MUL = 1.0 UNIT = "mm" TEXT = "测试参数 距离"

DATAEND
*/

#endif // PARSECONFIG_H















