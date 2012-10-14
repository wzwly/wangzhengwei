#ifndef __QSYSDATA_H__
#define __QSYSDATA_H__
#include <QString>
#include <QVector>
#include "typedef.h"
#include "./../ghead.h"

struct PARAM_DEF
{
    enum DATA_CLASS{   //属性
        SYSTEM_PARAM = 0, //系统参数设置
        SPEED_PARAM,  //速度参数
        LIMIT_PARAM,  //限位参数
        VIEW_PARM,    //试图参数
        DATA_QTY,
    };
    enum DATA_UNIT{    //单位
        UNIT_MM = 0, //mm
        UNIT_M,  //m
        UNIT_S,  //s
        UNIT_MIN,  //minite
        UNIT_MS,  //ms
        UNIT_MMPS, //mm/s
        UNIT_MMPMIN,//mm/min
        UNIT_MPS,  //m/s
        UNIT_MPMIN, //m/min
        UINT_NONE,
        UNIT_QTY,
    };

    enum DATA_P
    {
        DATA_INT = 1,//浮点数
        DATA_FLR,
    };

    DATA_CLASS cType; //参数分类
    DATA_UNIT  cUnit; //单位
    DATA_P    cDataP; //数据类型
    int  iIndex; //编号
    void *p;
    QString   strName; //参数名

    PARAM_DEF(float* pf_,DATA_CLASS T_,DATA_UNIT U_, const char* pName_);
    PARAM_DEF(int* pn_,DATA_CLASS T_,DATA_UNIT U_,const char* pName_);
    QString Value();
    float Data();
    void SetData(float f);
};

struct GlbConfig
{
    //显示类参数
    float fXoffsetView;
    float fYoffsetView;
    float fViewWidth;
    float fViewHeight;
    bool  bReDrawRule;
    float fRowGap;  //参数小于该值，认为是一排
    float fHoleRadius;

    //限位参数、
    float fLimitN[AXIS_NUM]; //x y z a b u
    float fLimitP[AXIS_NUM];
    //速度参
    float fSpeed[AXIS_NUM];
    float fManuSpeed;
    float fManuStep; //手动步长


    //系统参数
    int iDrillAxis; //
    int iAskTime;//ms
    //控制信息
    float fAxisPos[AXIS_NUM]; //x y z a b u
    int  iManuAxis;  //手动轴
    //控制命令
    unsigned short wRunStatus; // 0 停止 ，1 加工 ，2暂停，3断电继续
    unsigned short wBackOrg;  //回原点， -1 ，全部回， 1-6 分别每个轴回
    unsigned short wManulStep; // 1步进， 2连续开始， 3连续停止
    GlbConfig();
    void ReSet();
};

#define MAX_FILE_NAME_SIZE  64
class QSysData
{
   friend class QAutoPage;

public:
    static QSysData* Instance();
    typedef POINT_T<float> FLOAT_POINT ;
     ~QSysData();
protected:
    QSysData();

private:
    QString m_strFilePath;
    QString m_strFileName;
    QVector<FLOAT_POINT> m_vDrillData;
    int __n_Save_Begin__;
    char m_szPath[ MAX_FILE_NAME_SIZE + 1];
    char m_szName[ MAX_FILE_NAME_SIZE + 1];
    GlbConfig m_cGlbData;
     int __n_Save_End__;
public:
    void LoadFile(const QString& path_, const QString& name_);
    void ReSetData(){m_vDrillData.clear();}
    void ReSetLoad();
    bool IsLoadFile(const QString& path_){return path_ == m_strFilePath;}
    bool IsLoadFile(){return !m_strFilePath.isEmpty();}
    const QString& GetFileName() {return m_strFileName;}

    GlbConfig* GetCfgData() {return &m_cGlbData;}
private:
    void InitParamData();
    void LoadFromDxfFile(const QString& path);
    void LoadFromXtfFile(const QString& path);
    void SortDxfData();
    void SortOneRow(FLOAT_POINT* p_,  int nSize_);
};

#endif // QSYSDATA_H
