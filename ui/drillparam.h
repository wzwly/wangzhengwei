#ifndef SCANPAGE_H
#define SCANPAGE_H

#include "basepage.h"
#define DRILL_PARAM_NUM  33

class QTipLabel;
class QPushBtn;

class QDrillParamPage : public QBasePage
{
    Q_OBJECT
    struct _ParamItem
    {
            QTipLabel* pName; //文件名
            QPushBtn* pData;  //
    };


public:
    enum Status
    {
        INEDIT,
        EDIT_SAVED,
        NEW,
        NEW_SAVED,
        INVALID,
    };
     struct DrillData
     {
         int  iSizeX; //save file size
         int  iSizeY; //save file size
         int  iRow; //排数
         int* piDataY; //默认两位小数 放大了100倍
         int* piDataX;
         unsigned int iCrc;
         DrillData(){
             piDataY = NULL;
             piDataX = NULL;
             iRow = 12;
             iCrc = 0x3535fefe;
         }
         ~DrillData()
         {
            if (piDataY != NULL)
                delete[]  piDataY;
            if (piDataX != NULL)
                delete[]  piDataX;
         }
         bool IsValid() {return iCrc == 0x3535fefe;}
         bool ReSetRow(int iRow_ = 12)
         {
             int* _pOldX = piDataX;
             int* _pOldY = piDataY;

             int _iOldRow = MIN(iRow, iRow_);
             iRow = iRow_;
             iSizeX =  sizeof(int) * iRow;
             iSizeY =  sizeof(int) * iRow * 10;
             piDataX = new int[iRow];
             piDataY = new int[iRow * 10];
             if (_pOldX != NULL)
             {
                 memset(piDataX, 0, sizeof(int) * iRow);
                 memcpy(piDataX, _pOldX, sizeof(int) * _iOldRow);
                 delete[]  _pOldX;
             }
             else
                 memset(piDataX, 0, sizeof(int) * iRow);

             if (_pOldY != NULL)
             {
                 memset(piDataY, 0, sizeof(int) * iRow * 10);
                 memcpy(piDataY, _pOldY, sizeof(int) * _iOldRow * 10);
                 delete[]  _pOldY;
             }
             else
                 memset(piDataY, 0, sizeof(int) * iRow * 10);
             assert(piDataY);
             assert(piDataX);
         }
         void NewData()
         {
             iSizeX =  sizeof(int) * iRow;
             iSizeY =  sizeof(int) * iRow * 10;
             piDataY = new int[iRow * 10];
             piDataX = new int[iRow];

             memset(piDataX, 0, sizeof(int) * iRow);
             memset(piDataY, 0, sizeof(int) * iRow * 10);
         };
         float GetXVal(int nPos_) {return float(piDataX[nPos_]) / 100.0;}
         float GetYVal(int nPos_) {return float(piDataY[nPos_]) / 100.0;}
         void SetXVal(float fVal_,int nPos_) {piDataX[nPos_] =  Round(fVal_ * 100.0);}
         void SetYVal(float fVal_,int nPos_) {piDataY[nPos_] =  Round(fVal_ * 100.0);}
     };

    QDrillParamPage(QWidget* parent_);

public slots:
    void OnDataEdit(int nId_);//响应
    signals:

public:
    virtual void OnSndBtnClick(int nIndex_);//响应mainframe 二级菜单点击
    virtual void OnUpDate(int nFlag_){};

private:
    void CreatePageInfo();
    void OnShowPage();
    bool ChangeNewStatus(Status s_);
    void SaveFile(const QString& path_);
    void LoadFile(const QString& path_);
protected:
    virtual void showEvent ( QShowEvent * event );

private:
    _ParamItem m_aParamArray[DRILL_PARAM_NUM];
    DrillData m_cData;
    int m_nPagePos;
    Status m_sTatus;
};








#endif // SCANPAGE_H
