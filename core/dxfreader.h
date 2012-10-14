#ifndef DXFREADER_H
#define DXFREADER_H
#include "typedef.h"
#include <QList>
#include <QFile>

enum TOKEN_T
{
        NONE = -1,
};

struct Node_Vale
{
        QString strCurLine;
        TOKEN_T eType;
        int nToken;
public:
        Node_Vale(){Reset();}
        void Reset();
};

inline void Node_Vale::Reset()
{
        strCurLine.clear();
        eType = NONE;
        nToken = -1;
}


class QDxfReader
{

    friend class QSysData;
public:
    QDxfReader();
    ~QDxfReader();
    typedef POINT_T<float> FLOAT_POINT ;

public:
        void AtachFile(const QString& file_){Open(file_);}//关联读取的文件
        bool StartAnalyse();//开始解析文件
        void PrintData(); //打印数据
        void DrawShape(); //绘制形状

protected:
        bool Open(const QString& szFileName_);
        void Close();
        bool ReadNextNode(int nToken_); //读标记为2 或者0
        bool Match(const char* pMark_);

        bool DealCircles(); //圆弧处理
        //bool DealLines(); //直线处理
        //bool DealPolyLine(); //直线多边形
protected:
        QString m_strFileName;
        Node_Vale m_cNodeVal;
        int  m_nLine;// the current line
        long m_nFileSize;
        QFile* m_pFile;
        QList<FLOAT_POINT> m_vShapeList;
};

#endif // DXFREADER_H
