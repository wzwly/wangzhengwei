#ifndef __QSYSDATA_H__
#define __QSYSDATA_H__
#include <QString>
#include <QVector>
#include "typedef.h"
#include "./../ghead.h"
#include "parseconfig.h"

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
    //for new protecol 
    int  m_iParamRO0[PAGE_PARAM_COUNT]; //用于输出
    int  m_iParamRO1[PAGE_PARAM_COUNT]; //用于输出
    int  m_iParamRO2[PAGE_PARAM_COUNT]; //用于输出
    int  m_iParamRO3[PAGE_PARAM_COUNT]; //用于输出
    int  m_iParamRI[PAGE_PARAM_COUNT]; //用于输入
    int  m_iParamRBtn[PAGE_PARAM_COUNT]; //用于按键发送
    int __n_Save_End__;
    ConfigData m_cGlbData;

public:
    //文件加载操作
    void LoadFile(const QString& path_, const QString& name_);
    void ReSetData(){m_vDrillData.clear();}
    void ReSetLoad();
    bool IsLoadFile(const QString& path_){return path_ == m_strFilePath;}
    bool IsLoadFile(){return !m_strFilePath.isEmpty();}
    const QString& GetFileName() {return m_strFileName;}

    ConfigData* GetCfgData() {return &m_cGlbData;}

    //参数操作
    int& ParamData(int i_) {return m_iParamRO0[i_];}
    QString GetValText(const DataMap* pMap_);
    double GetVal(const DataMap* pMap_);
    void SetVal(const DataMap* pMap_, double fVal_);    
    bool CheckValid(const DataMap* pMap_,double dVal_);
    void GetMaxMinRange(const DataMap*pMap_, double& dMin_, double& dMax_);   
    void SetToModelBus(const DataMap* pMap_);

private:
    void InitParamData();
    void LoadFromDxfFile(const QString& path);
    void LoadFromXtfFile(const QString& path);
    void SortDxfData();
    void SortOneRow(FLOAT_POINT* p_,  int nSize_);
};

#endif // QSYSDATA_H
