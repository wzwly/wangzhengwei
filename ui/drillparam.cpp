#include "drillparam.h"
#include "./../label/item.h"
#include "./../label/button.h"
#include "./../label/dlg.h"
#include "./../label/configset.h"
#include "mainframe.h"


QDrillParamPage::QDrillParamPage(QWidget* parent_)
 :QBasePage( parent_)
{
    for (int _i = 0; _i < 120; ++_i)
            m_cData.m_iDataY[_i] = _i * 10 % 300 + 1;

    for (int _i = 0; _i < 12; ++_i)
            m_cData.m_iDataX[_i] = _i *20 + 1;
    CreatePageInfo();
    m_nPagePos = 0;
}

#define LIST_LABEL_H   56
#define LIST_LABEL_W0  140
#define LIST_LABEL_W1  145

void QDrillParamPage::CreatePageInfo()
{
   int _nIndex = 0;
   for (int _i = 0; _i < 3; ++_i)
   {
     for (int _j = 0; _j < 11; ++_j)
     {
        m_aParamArray[_nIndex].pName = new QTipLabel(this, QItem::LABEL_LIST);
        m_aParamArray[_nIndex].pName->InitShow(_i * (LIST_LABEL_W0 + LIST_LABEL_W1),1 + LIST_LABEL_H * _j, LIST_LABEL_W0, LIST_LABEL_H, 20);

        m_aParamArray[_nIndex].pData = new QPushBtn(this, QItem::LABEL_TIP, _nIndex);
        m_aParamArray[_nIndex].pData->InitShow(LIST_LABEL_W0 + _i * (LIST_LABEL_W0 + LIST_LABEL_W1), 1 + LIST_LABEL_H * _j, LIST_LABEL_W1, LIST_LABEL_H, 20);
        m_aParamArray[_nIndex].pData->SetBdWide(5);
        connect(m_aParamArray[_nIndex].pData, SIGNAL(ClickedEvent(int)), this, SLOT(OnDataEdit(int)));
        _nIndex++;
     }
   }
}


void QDrillParamPage::OnDataEdit(int nId_)
{
    int _nPos = nId_ / 11;
    int _nIndex = nId_ % 11;

    QNuberInput _SoftKey(this);

    if (_nIndex == 0)
    {
            _SoftKey.SetRange(-1000, 2000, m_cData.GetXVal(_nPos));
    }
    else
    {
            _nPos = _nPos * 10 + _nIndex - 1;
            _SoftKey.SetRange(-1000, 2000, m_cData.GetYVal(_nPos));
    }

    if (!_SoftKey.exec())
    {
            return;
    }

    double _data = _SoftKey.GetDbData();
    if (_nIndex == 0)
    {
            m_cData.m_iDataX[_nPos] = _data * 100.0;
    }
    else
    {
            m_cData.m_iDataY[_nPos] = _data * 100.0;
    }

    m_aParamArray[nId_].pData->setText(QString::number(_data,'f', 2));
}

void QDrillParamPage::OnSndBtnClick(int nId_)
{
    if (nId_ == 0)
    {
            if (m_nPagePos > 0)
            {
                    m_nPagePos--;
                    OnShowPage();
            }
    }
    else if (nId_ == 1)
    {
            if (m_nPagePos < 3)
            {
                    m_nPagePos++;
                    OnShowPage();
            }
    }
    else if (nId_ == 2)
    {
          QStrInputDlg _dlg(this,QStrInputDlg::FILE_NAME_DLG);
          if (_dlg.exec())
          {
                QString _str = _dlg.GetInputText();
                if (_str.length() >= 10)
                {
                    QOkDlg _dlg("文件名长度超过10,请重新输入！", this);
                    _dlg.exec();
                    return;
                }
                QString _stPath = QString("./Flash/%1.xtf").arg(_str);
                QConfigSet::WriteToFile(_stPath, &m_cData, m_cData.nSize, 0);
          }
    }

}

void QDrillParamPage::OnShowPage()
{
    int _nIndex = 0;

    int _nPos = 0;
    for (int _i = 0; _i < 3; _i++)
    {
         _nPos = m_nPagePos * 3 + _i;
         for (int _j = 0; _j < 11; _j++)
         {           
              if (_j == 0)
              {
                 m_aParamArray[_nIndex].pName->setText(QString("第%1排:X%2").arg(_nPos + 1).arg(_nPos + 1));
                 m_aParamArray[_nIndex].pData->setText(QString::number(m_cData.GetXVal(_nPos), 'f', 2));
              }
              else
              {
                 m_aParamArray[_nIndex].pName->setText(QString("Y%1-%2").arg(_nPos + 1).arg(_j));
                 m_aParamArray[_nIndex].pData->setText(QString::number(m_cData.GetYVal(_nPos * 10 + _j - 1), 'f', 2));
              }
              _nIndex++;
        }
    }
    m_pMainFrame->SetTiltleLabel(QString("钻孔参数第%1页").arg(m_nPagePos + 1));

}


void QDrillParamPage::showEvent ( QShowEvent * event )
{
    OnShowPage();
    QBasePage::showEvent (event );
}

