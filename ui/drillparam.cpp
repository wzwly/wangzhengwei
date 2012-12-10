#include "drillparam.h"
#include "./../label/item.h"
#include "./../label/button.h"
#include "./../label/dlg.h"
#include "./../label/configset.h"
#include "mainframe.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>



QDrillParamPage::QDrillParamPage(QWidget* parent_)
 :QBasePage( parent_)
{
    CreatePageInfo();
    m_nPagePos = 0;
    m_sTatus = INVALID;
    m_cData.NewData();
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
        _nPos = m_nPagePos * 3 + _nPos;
        _SoftKey.SetRange(-1000, 2000, m_cData.GetXVal(_nPos));
    }
    else
    {
         _nPos = m_nPagePos * 30 + _nPos * 10 + _nIndex - 1;
         _SoftKey.SetRange(-1000, 2000, m_cData.GetYVal(_nPos));
    }

    if (!_SoftKey.exec())
    {
            return;
    }

    double _data = _SoftKey.GetDbData();
    if (_nIndex == 0)
    {
            m_cData.SetXVal(_data,_nPos);
    }
    else
    {
            m_cData.SetYVal(_data,_nPos);
    }

    m_aParamArray[nId_].pData->setText(QString::number(_data,'f', 2));
}

void QDrillParamPage::OnSndBtnClick(int nId_)
{
    if (nId_ == 4)
    {
            if (m_nPagePos > 0)
            {
                    m_nPagePos--;
                    OnShowPage();
            }
    }
    else if (nId_ == 5)
    {
            int _nPage = m_cData.iRow / 3 - 1;
            if (m_nPagePos < _nPage)
            {
                    m_nPagePos++;
                    OnShowPage();
            }
    }
    else if(0 == nId_) //new
    {
        if (ChangeNewStatus(NEW))
        {
            m_cData.NewData();
            m_nPagePos = 0;
            OnShowPage();
        }
    }
    else if(1 == nId_) //edit
    {
        if (m_pSysData->IsLoadFile() && m_pSysData->IsSelDefFile())
        {
             if (ChangeNewStatus(INEDIT))
             {
                LoadFile(m_pSysData->GetFileFullPath());
                OnShowPage();
             }
        }
    }
    else if (nId_ == 2) //save
    {
          if (m_sTatus == INEDIT)
          {
                SaveFile(m_pSysData->GetFileFullPath());
          }
          else
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
                QString _stPath = QString("./Flash/%1%2").arg(_str).arg(FILE_SUFFIX);
                //QConfigSet::WriteToFile(_stPath, &m_cData, m_cData.iSize, 0);
                SaveFile(_stPath);
            }
          }
    }
    else if (nId_ == 3)
    {
        if (m_sTatus == INEDIT)
            return;

        QNuberInput _SoftKey(this);
       _SoftKey.SetRange(3, 100, m_cData.iRow);

       if (!_SoftKey.exec())
        {
                return;
        }
        int _nRow = _SoftKey.GetDbData();

        if (m_cData.iRow != _nRow)
        {
            _nRow = _nRow / 3;
            _nRow *=3;
            m_nPagePos = 0;
            m_cData.ReSetRow(_nRow);
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
                 m_aParamArray[_nIndex].pName->setText(QString("第%1排:X").arg(_nPos + 1));
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
    m_pMainFrame->SetInfoItem(QString("钻孔参数第%1页").arg(m_nPagePos + 1));

}


void QDrillParamPage::showEvent ( QShowEvent * event )
{
    OnShowPage();
    QBasePage::showEvent (event );
}


bool QDrillParamPage::ChangeNewStatus(Status s_)
{
    bool _bRet = false;
    switch (m_sTatus)
    {
        case INEDIT:
            if (s_ == EDIT_SAVED || s_ == INVALID)
            {
                m_sTatus = INVALID;
                _bRet = true;
            }
            break;
        case NEW:
            if (s_ == NEW_SAVED || s_ == INVALID)
            {
                 m_sTatus = INVALID;
                _bRet = true;
            }
            break;
        case INVALID:
            m_sTatus = s_;
            _bRet = true;
            break;
        default:
        break;
    }
    return _bRet;
}

void QDrillParamPage::SaveFile(const QString& path_)
{
    int _fd = open(path_.toStdString().data(), O_CREAT | O_RDWR, S_IWUSR | S_IRUSR);
    if (_fd < 0)
    {

        return;
    }
    write(_fd, &m_cData,sizeof(DrillData));
    write(_fd, m_cData.piDataX,m_cData.iSizeX);
    write(_fd, m_cData.piDataY,m_cData.iSizeY);
    write(_fd, &m_cData.iCrc,sizeof(int));
    ::close(_fd);
}

void QDrillParamPage::LoadFile(const QString& path_)
{
    int _fd = open(path_.toStdString().data(), O_RDWR);
    if (_fd < 0)
    {

        return;
    }
    read(_fd, &m_cData,sizeof(DrillData));
    m_cData.NewData();
    read(_fd, m_cData.piDataX,m_cData.iSizeX);
    read(_fd, m_cData.piDataY,m_cData.iSizeY);
    read(_fd, &m_cData.iCrc,sizeof(int));
    ::close(_fd);
}
