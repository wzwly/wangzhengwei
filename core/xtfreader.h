#ifndef XTFREADER_H
#define XTFREADER_H

#include "parseconfig.h"
#include "typedef.h"
#include  "./../ghead.h"

//1.read a valid line
//2.deal the xy pos
//3.add to vector
//4.if has erro log it and exit
//5. when vector's len is 10 or parse end , send data
class CXtfReader : public  CParseBase
{
    enum Axis{
        INVALID_LABEL = -1,
        LABEL_asX = 0,
        LABEL_asY,
        LABEL_asZ,
        axisNUM,
        VALID_LABEL,
    };
    enum T
    {
        SEND_PARAM_CNT = 15,
    };
    struct ModelInfo
    {
        char cValid[axisNUM];
        char cLastAxis;
        int nXYZ[axisNUM];

        void Reset()
        {
            for (int _i = 0; _i < axisNUM; ++_i)
            {
                cValid[_i] = INVALID_LABEL;
            }
            cLastAxis = INVALID_LABEL;
        }
    };
    typedef POINT_T<int> INT_PT;
public:
    CXtfReader();
    ~CXtfReader(){};
    bool StartParseFile(short nAddr_);

protected:
    bool GetXYXPos(INT_PT& p_);
    void WriteData();
private:
    short m_nParamAddr;
    int m_nReadedCnt;
    vector<INT_PT> m_ptDrillPos;
    ModelInfo m_cModeInfo;
};

/*
//==================
//======格式如下=====
//==================
y 100
x 100
34
45
56.1
56
x 33
x 66
y 88
88.4
x 44 y 200
//==============说明=========
1.第一次出现Y，之后的X共用同一个Y的值
2.出现X后，后面的参直接给出数值，表示使用最近一个的X或者Y
3.XY可以同时出现在一行
4.XY不区分大小写
//===========================
*/

#endif // XTFREADER_H
