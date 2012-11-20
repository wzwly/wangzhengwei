#ifndef  __PARSECONFIG_H__
#define __PARSECONFIG_H__
 #include <stack>
#include <QFile>
#include <QMap>

#include  "./../ghead.h"

//参数结构体
//该结构体不用每次保存
struct DataRangeMap
{
    bool bSysNo;  //true user system data
    union
    {
        int nNo;
        double dMaxMin;
    };
    DataRangeMap(int iMinMax_)
    {
        bSysNo = true;
        nNo = iMinMax_;
    }
    DataRangeMap(double dMinMax_)
    {
        bSysNo = false;
        dMaxMin = dMinMax_;
    }
};

struct DataMap
{
       int iNo; //参数号 ，索引参数
       int iAddr;//地址，发送地址
       int iGroup; //组属性
       int iFraction;//小数点位数
       DataRangeMap* pMin;
       DataRangeMap* pMax;
       double dMult; //显示发大倍数
       string strName;
       string  strUnit;  //单位
       string  strText;
       DataMap()
       {
           iNo = 0;
           iAddr = 1;
           iGroup = 0;
           iFraction = 0;
           dMult = 1.0;      
       };

       DataMap(int iNo_, int iAddr_, int iGroup_, int iFrac_,
               double dMul_, DataRangeMap* pMin_, DataRangeMap* pMax_,
               const string& szName_, const string& szUnit_, const string szText_)
       {
           iNo =  iNo_;
           iAddr = iAddr_;
           iGroup = iGroup_;
           iFraction = iFrac_;
           dMult = dMul_;
           pMin = pMin_;
           pMax = pMax_;
           strName = szName_;
           strUnit = szUnit_;
           strText = szText_;
       }

        ~DataMap()
        {
            if (pMin)
                delete pMin;
            if (pMax)
               delete pMax;
            qDebug() << "range freed";
        };
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
    QMap<int ,int> m_mapAddrToIndex;
    QMap<int, int> m_mapIndexToVector;
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

    void SetVal(int nNo_, double dVal_, double dMult_);

private:
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
//#参数号=初始值 ADDR=地址 NAME= "参数名" FRAC=小数 MUL=显示倍数  MIN = 1.0 MAX# = 1 UNIT="单位" TEXT="参数说明"
//--参数号必须写，且不能重复
//--初始值最好写，否则默认值可能不正确，一但参数保存之后，初始值将不读取
//--小数不写时默认0
//--倍数不写时默认1,否则显示将=初始值*倍数
//--MIN MAX分别是参数编辑时的最大最小值，直接跟参数表示或者用对应的参数号做上下限（取值显示值）。
//--单位和参数说明不写时，默认为空
//参数的Group，分为6大组
//1-4组用于从上至下发送的参数。参数号51-250
//5组用于下位机向上更新显示的参数，如坐标，报警等。参数号1-50
//6组用于按键向下发送的命令。参数号251-300
//
//报警命令格式：
//!=命令  TEXT = "报警提示"
//
//

DATABEGIN
GROUP 1
#1=1.12  ADDR = 1  NAME="X速度" FRAC = 2 MUL = 1.0  MIN = 1.0 MAX = 3600 UNIT = "mm/s"  TEXT = "X速度"
#2=2.12  ADDR = 2 NAME="Y速度"  FRAC = 2 MUL = 1.0  UNIT = "m/min" TEXT = "Y速度"
DATAEND

DATABEGIN
GROUP 2
#51=51.123  ADDR = 51  NAME="X中心" FRAC = 3 MUL = 1.0  UNIT = "mm" TEXT = "X原点"
#52=52.123  ADDR = 52 NAME="Y中心" FRAC = 3  MUL = 1.0  UNIT = "mm" TEXT = "Y原点"
DATAEND

DATABEGIN
GROUP 3
#101=101.1  ADDR = 101 NAME="循环时间" FRAC = 1 MUL = 1.0  UNIT = "ms" TEXT = "循环时间间隔"
#102=102.7  ADDR = 102 NAME="等待时间" FRAC = 1  MUL = 1.0  UNIT = "ms" TEXT = "等待时间间隔"
DATAEND

DATABEGIN
GROUP 4
#151=151.12  ADDR = 151  NAME="工件计数" FRAC = 0 MUL = 1.0  UNIT = "" TEXT = "加工工件计数"
#152=152.12  ADDR = 152 NAME="报警次数" FRAC = 0  MUL = 1.0  UNIT = "mm" TEXT = "报警次数"
DATAEND


DATABEGIN
!=1  TEXT= "系统错误"
DATAEND
*/

#endif // PARSECONFIG_H















