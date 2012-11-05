#ifndef  __PARSECONFIG_H__
#define __PARSECONFIG_H__

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

class CParseConfig
{
public:
    CParseConfig();
    bool OpenConfigFile(const char* szPath_);




};

#endif // PARSECONFIG_H
