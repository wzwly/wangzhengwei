#include "filepage.h"
#include "./../label/item.h"
#include "./../label/button.h"
#include "./../label/dlg.h"
#include "mainframe.h"
#include <QDir>

#define FILE_ITEM_H0 60
#define FILE_ITEM_H  62
#define FILE_ITEM_W0 100
#define FILE_ITEM_W1 454
#define FILE_ITEM_W2 150
#define FILE_ITEM_W3 150

#define FILE_FONT_SIZE 18

QFilePage::QFilePage(QWidget* parent_)
        :QBasePage( parent_)
{
    m_nSelFile = 0;
    CreatePageInfo();
}

void QFilePage::_FileItem::SetActive()
{
        pIndex->SetStatus(ACTIVE_WIND);
        pFileName->SetStatus(ACTIVE_WIND);
        pFileSize->SetStatus(ACTIVE_WIND);
        pFilePath->SetStatus(ACTIVE_WIND);
}


void QFilePage::_FileItem::SetNormal()
{
        pIndex->SetStatus(NORMAL_WIND);
        pFileName->SetStatus(NORMAL_WIND);
        pFileSize->SetStatus(NORMAL_WIND);
        pFilePath->SetStatus(NORMAL_WIND);
}

void QFilePage::CreatePageInfo()
{
    QTipLabel* _pTemp = new QTipLabel(this,QItem::LABEL_DLG);
    _pTemp->InitShow("编号", 0, 0, FILE_ITEM_W0, FILE_ITEM_H0, FILE_FONT_SIZE);
     _pTemp = new QTipLabel(this,QItem::LABEL_DLG);
    _pTemp->InitShow("文件名", 100, 0, FILE_ITEM_W1, FILE_ITEM_H0, FILE_FONT_SIZE);
    _pTemp = new QTipLabel(this,QItem::LABEL_DLG);
    _pTemp->InitShow("文件大小", 554, 0, FILE_ITEM_W2, FILE_ITEM_H0, FILE_FONT_SIZE);
    _pTemp = new QTipLabel(this,QItem::LABEL_DLG);
    _pTemp->InitShow("路径", 704, 0, FILE_ITEM_W3, FILE_ITEM_H0, FILE_FONT_SIZE);

    for (int _i = 0; _i < FINE_ITEM_NUM; ++_i)
    {
            m_aFimeArray[_i].pIndex = new QTipLabel(this, QItem::LABEL_LIST);
            m_aFimeArray[_i].pIndex->InitShow(0, FILE_ITEM_H0 + FILE_ITEM_H * _i, FILE_ITEM_W0, FILE_ITEM_H, FILE_FONT_SIZE);
            m_aFimeArray[_i].pFileName = new QPushBtn(this, QItem::LABEL_LIST,_i,Qt::AlignLeft);
            m_aFimeArray[_i].pFileName->InitShow(100, FILE_ITEM_H0 + FILE_ITEM_H * _i, FILE_ITEM_W1, FILE_ITEM_H, FILE_FONT_SIZE);
            m_aFimeArray[_i].pFileSize = new QTipLabel(this, QItem::LABEL_LIST);
            m_aFimeArray[_i].pFileSize->InitShow(554, FILE_ITEM_H0 + FILE_ITEM_H * _i, FILE_ITEM_W2, FILE_ITEM_H, FILE_FONT_SIZE);
            m_aFimeArray[_i].pFilePath = new QTipLabel(this, QItem::LABEL_LIST);
            m_aFimeArray[_i].pFilePath->InitShow(704, FILE_ITEM_H0 + FILE_ITEM_H * _i, FILE_ITEM_W3, FILE_ITEM_H, FILE_FONT_SIZE);
            connect(m_aFimeArray[_i].pFileName, SIGNAL(ClickedEvent(int)), this, SLOT(OnListClick(int)));
    }
}


void QFilePage::ReadFile(const char* szDir_, int nPos_)
{
    QDir _dir(szDir_);
    QStringList _filters;
     _filters << "*.dxf" << "*.Dxf" << "*.DXF"<<"*.xtf";
     _dir.setNameFilters(_filters);
     _dir.setSorting(QDir::Size | QDir::Reversed);

     QFileInfoList _list = _dir.entryInfoList();
     QFileInfo _fileInfo;
     _FileInfo _Info;
     for (int _i = 0; _i < _list.size(); ++_i)
     {
         _fileInfo = _list.at(_i);
                 _Info.nSize = _fileInfo.size()/ 1024 + 1;
                 _Info.szName = _fileInfo.fileName();
                 _Info.nPath = nPos_;
                 m_lFileList.push_back(_Info);
     }
}

void QFilePage::ShowFile(int nIndex_)
{

        for (int _i = 0; _i < FINE_ITEM_NUM; ++_i)
        {
                m_aFimeArray[_i].pIndex->setText("");
                m_aFimeArray[_i].pFileName->setText("");
                m_aFimeArray[_i].pFileSize->setText("");
                m_aFimeArray[_i].pFilePath->setText("");
        }

        int _nLeft = m_lFileList.size() - nIndex_;
        m_nMaxShow = _nLeft >= FINE_ITEM_NUM ? FINE_ITEM_NUM : _nLeft;

        m_nShowIndex = nIndex_;
        _FileInfo _Info;
        for (int _i = 0; _i < m_nMaxShow; ++_i)
        {
                _Info = m_lFileList.at(_i + nIndex_);
                m_aFimeArray[_i].pIndex->setText(QString("%1").arg(_i + 1 + nIndex_));
                m_aFimeArray[_i].pFileName->setText(QString("%1").arg(_Info.szName));
                m_aFimeArray[_i].pFileSize->setText(QString("%1K").arg(_Info.nSize));
                m_aFimeArray[_i].pFilePath->setText(QString("%1").arg(_Info.nPath == 0 ? "系统" : "U盘"));
        }
        m_pMainFrame->SetInfoItem(QString("文件总数:%1").arg(m_lFileList.size()));
        m_aFimeArray[m_nSelFile].SetNormal();
        m_nSelFile = 0;
        m_aFimeArray[m_nSelFile].SetActive();
}


static const char* g_szPath[2] = {"./Flash/", "./sd0/"};
static const char* g_szPathName[2] = {"系统", "U盘"};
void QFilePage::showEvent ( QShowEvent * event )
{
        QWidget::showEvent (event);
        m_lFileList.erase(m_lFileList.begin(), m_lFileList.end());
        ReadFile(g_szPath[0], 0);
        ReadFile(g_szPath[1], 1);
        ShowFile(0);
        QBasePage::showEvent (event );
}

void QFilePage::OnSndBtnClick(int nId_)//响应mainframe 二级菜单点击
{
    if (0 == nId_)
    {
       if (m_nShowIndex > 0)
        {
            m_nShowIndex -= FINE_ITEM_NUM;
            ShowFile(m_nShowIndex);
        }
    }
    else if (1 == nId_)
    {
        if (m_nShowIndex + m_nMaxShow < m_lFileList.size())
        {
           m_nShowIndex += m_nMaxShow;
           ShowFile(m_nShowIndex);
         }

    }
    else if (2 == nId_)
    {
        LoadFile();
    }
    else if (3 == nId_) //拷贝
    {
        _FileInfo _Info = m_lFileList.at(m_nSelFile + m_nShowIndex);
        QDir _dir(g_szPath[_Info.nPath]);

        if (!_dir.exists(_Info.szName))
            return;

        int _nTo = (_Info.nPath + 1) % 2;
        QDir _dirTo(g_szPath[_nTo]);
        if (_dirTo.exists(_Info.szName))
        {

            QString _str = QString("【%1】已经存在文件【%2】").arg(g_szPathName[_nTo]).arg(_Info.szName);
            QOkDlg _dlg(_str, this);
            _dlg.exec();
            return;
        }
       QString _strSrc = QString("%1%2").arg(g_szPath[_Info.nPath]).arg(_Info.szName);
       QString _strDest = QString("%1%2").arg(g_szPath[_nTo]).arg(_Info.szName);


       if (!QFile::copy(_strSrc, _strDest))
       {
           QOkDlg _dlg("拷贝失败！", this);
           _dlg.exec();
           return;
       }

       m_lFileList.erase(m_lFileList.begin(), m_lFileList.end());
       ReadFile(g_szPath[0], 0);
       ReadFile(g_szPath[1], 1);
       ShowFile(0);
    }
    else if (4 == nId_) //删除
    {
            _FileInfo _Info = m_lFileList.at(m_nShowIndex + m_nSelFile);
            QDir _dir(g_szPath[_Info.nPath]);

            if (_dir.exists(_Info.szName))
            {
                QString _str = QString("确定删除文件：%1?").arg(_Info.szName);
                QOkDlg _dlg(_str, this);
                 if (!_dlg.exec())
                     return;
            }
            else
                return;

            if (_dir.remove(_Info.szName))
            {
                  m_lFileList.removeAt(m_nSelFile);
                  ShowFile(m_nShowIndex);
            }
    }
}

void QFilePage::LoadFile()
{
    _FileInfo _Info = m_lFileList.at(m_nSelFile + m_nShowIndex);
    QDir _dir(g_szPath[_Info.nPath]);

    if (!_dir.exists(_Info.szName))
        return;

    QString _str = QString("%1%2").arg(g_szPath[_Info.nPath]).arg(_Info.szName);
    m_pSysData->LoadFile(_str, _Info.szName);
}


void QFilePage::OnListClick(int nId_)
{
    if (m_nSelFile == nId_ || nId_ >= m_nMaxShow)
        return;
    m_aFimeArray[m_nSelFile].SetNormal();
    m_nSelFile = nId_;
    m_aFimeArray[m_nSelFile].SetActive();
}

