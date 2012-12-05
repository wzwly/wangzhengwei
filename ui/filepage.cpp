#include "filepage.h"
#include "./../label/label.h"
#include "mainframe.h"

#include <QDir>
#include <QHeaderView>
#include <QStandardItemModel>

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

    m_pFileList = new QListBox(this);
    m_pFileList->SetWindPos(0, 0,853, 618);
    QStringList _listHead;
    _listHead << "编号"<<"文件名"<<"路径"<<"文件大小";
    m_pFileList->SetColumnHeadName( _listHead);

    m_pFileList->SetColumnWidth(0, 135);
    m_pFileList->SetColumnWidth(1, 300);
    m_pFileList->SetColumnWidth(2, 200);
    m_pFileList->SetColumnWidth(3, 200);
    m_pFileList->SetColumnHeight(18);
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

void QFilePage::ShowFile()
{
     m_pFileList->ClearAllRow();
     int _nSize = m_lFileList.size();
     _FileInfo _Info;

    for (int _i = 0; _i <  _nSize; ++_i)
    {
      _Info = m_lFileList.at(_i);
      QStringList _listFile;
      _listFile << QString("%1").arg(_i + 1) << QString("%1").arg(_Info.szName)
                   <<QString("%1K").arg(_Info.nSize) << QString("%1").arg(_Info.nPath == 0 ? "系统" : "U盘");
      m_pFileList->InsertRowText(_listFile);
    }
    m_pMainFrame->SetInfoItem(QString("文件总数:%1").arg(m_lFileList.size()));

}


static const char* g_szPath[2] = {"./Flash/", "./sd0/"};
static const char* g_szPathName[2] = {"系统", "U盘"};
void QFilePage::showEvent ( QShowEvent * event )
{
        QWidget::showEvent (event);
        m_lFileList.erase(m_lFileList.begin(), m_lFileList.end());
        ReadFile(g_szPath[0], 0);
        ReadFile(g_szPath[1], 1);
        ShowFile();
        QBasePage::showEvent (event );
}

void QFilePage::OnSndBtnClick(int nId_)//响应mainframe 二级菜单点击
{
    if (4 == nId_)
    {

    }
    else if (5 == nId_)
    {


    }
    else if (0 == nId_)
    {
        LoadFile();
    }
    else if (2 == nId_) //拷贝
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
       ShowFile();
    }
    else if (3 == nId_) //删除
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
                  ShowFile();
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


