#ifndef ANALYSISPAGE_H
#define ANALYSISPAGE_H

#include "basepage.h"
#include "./../ghead.h"

class QTipLabel;
class QPushBtn;
class QShape;
class QDrawArea;

class QAutoPage : public QBasePage
{
     Q_OBJECT
public:
    QAutoPage(QWidget* parent_);

public slots:
    void OnViewBtnClick(int nId_);
    void OnStepBtnClick(int nId_);
    signals:

public:
    virtual void OnSndBtnClick(int nIndex_);//响应mainframe 二级菜单点击
    virtual void OnUpDate(int nFlag_);

private:
    void CreatePageInfo();

protected:
    virtual void showEvent ( QShowEvent * event );
private:
    QTipLabel* m_pXYZCoor[AXIS_NUM ];
    //QTipLabel* m_pCurDrillPos[2];

    QPushBtn* m_pStepBtn[4];
    QPushBtn* m_pStepOp[2];

    //QShape*   m_pIO[4];
    QPushBtn* m_pViewOp[6];
    QDrawArea* m_pDrawArea;

    QPainter* m_pPaint;
    int m_nWidth;
    int m_nHeight;

    POINT_T<int> m_nOrg;
    float m_fZoom;

    double m_dDeltH;
    double m_dDeltW;
    GlbConfig* m_pGlbCfg;

    float m_fStep;
    int  m_nAxisSel;
 private:
    void DrawSimulate();
    void DrawRuler();
    void ClearArea();
};

//========================
class QDrawArea :public QWidget
{
    Q_OBJECT
public:
    QDrawArea(QWidget* p_);
    ~QDrawArea(){};

protected:
    virtual void paintEvent(QPaintEvent * e_);
    double GetRuleInterval(const double& fMin_, const double& fMax_, const int& nLen_);
private:
        QPixmap* m_pPixMap;
        QPainter* m_pPaint;     
public:
       void ClearShape();
       QPainter* GetPainter(){ return m_pPaint;}
       void GetWidthHeight(int&w_,int&h_);
       void DrawRightRuler(const double fMin_, const double fMax_, QString strUnit_);
       void DrawBottomRuler(const double fMin_, const double fMax_, QString strUnit_);
};
#endif // ANALYSISPAGE_H
