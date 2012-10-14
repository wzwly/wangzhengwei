#ifndef SYSPAGE_H
#define SYSPAGE_H
#include <QDebug>
#include "basepage.h"
#define INPUT_IO_NUM  16
#define OUTPUT_IO_NUM 16
#define LIMIT_IO_NUM   6


class QShape;
class QTipLabel;

class QSysPage : public QBasePage
{
    Q_OBJECT

public:
    QSysPage(QWidget* parent_);

public slots:
    signals:

protected:
    //virtual void showEvent ( QShowEvent * event );
    void CreatePageInfo();

public:
    virtual void OnSndBtnClick(int nIndex_);//响应mainframe 二级菜单点击
    virtual void OnUpDate(int nFlag_){};
 private:

    QShape* m_pIOInput[INPUT_IO_NUM];
    QShape* m_pIOOutput[INPUT_IO_NUM];
    QShape* m_pLimitPort[LIMIT_IO_NUM];
    QTipLabel* m_pInputTip;
    QTipLabel* m_pOutPutTip;
    QTipLabel* m_pLinitTip;
};

#endif // SYSPAGE_H
