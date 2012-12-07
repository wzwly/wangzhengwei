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
    m_nFileStatus = 0;
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

    m_pCodeEdit = new QCodeEdit(this);
    m_pCodeEdit->hide();
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
                 _Info.nSize = _fileInfo.size();
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
                   <<QString("%1k").arg(_Info.nSize / 1024 + 1) << QString("%1").arg(_Info.nPath == SYS_PATH ? "系统" : "U盘");
      m_pFileList->InsertRowText(_listFile);
    }
    m_pMainFrame->SetInfoItem(QString("文件总数:%1").arg(m_lFileList.size()));

}


static const char* g_szPath[2] = {"./Flash/", "./sd0/"};
static const char* g_szPathName[2] = {"系统", "U盘"};

 void QFilePage::ReListFile()
 {
     m_lFileList.erase(m_lFileList.begin(), m_lFileList.end());
     ReadFile(g_szPath[SYS_PATH], SYS_PATH);
     ReadFile(g_szPath[USB_PATH], USB_PATH);
     ShowFile();
 }


void QFilePage::showEvent ( QShowEvent * event )
{   
     ReListFile();
     QBasePage::showEvent (event );
}



void QFilePage::OnSndBtnClick(int nId_)//响应mainframe 二级菜单点击
{
    if (0 == nId_)
    {   
        if (0 == m_nSndBtnShow)
            LoadFile();
        else
            CpyFile(USB_PATH);
    }
    if (1 == nId_)
    {
         if (0 == m_nSndBtnShow)
            UnloadFile();//
         else
            CpyFile(SYS_PATH);
    }
    else if (2 == nId_) //
    {
        if (0 == m_nSndBtnShow)
             EditFile();//
        else
            DeleteFile();
    }
    else if (3 == nId_) //
    {
        if (0 == m_nSndBtnShow)
            NewFile();
    }
    else if (4 == nId_)
    {
        if (0 == m_nSndBtnShow)
            SaveFile();
    }
    else if (5 == nId_)
    {
        m_nSndBtnShow =  m_nSndBtnShow ? 0 : 1;
        m_pMainFrame->ReShowMenuBtn();
    }

}


 bool QFilePage::GetCurSelFile(_FileInfo& info_)
 {
     int _nSel = m_pFileList->GetCurSel();
     if (_nSel < 0)
         return false;

     info_ = m_lFileList.at(_nSel);
     return true;
 }

void QFilePage::LoadFile()
{  
    _FileInfo _Info;
    if (!GetCurSelFile(_Info))
        return;

    QDir _dir(g_szPath[_Info.nPath]);
    if (!_dir.exists(_Info.szName))
        return;
    QString _str = QString("%1%2").arg(g_szPath[_Info.nPath]).arg(_Info.szName);
    m_pSysData->LoadFile(_str, _Info.szName);
}


 void QFilePage::UnloadFile()
 {

 }

 void QFilePage::CpyFile(int nPath_)
 {
     _FileInfo _Info;
     if (!GetCurSelFile(_Info))
         return;
     if (nPath_ == _Info.nPath)
         return;
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
    ReListFile();
 }

 void QFilePage::DeleteFile()
 {
     int _nSel = m_pFileList->GetCurSel();
     if (_nSel < 0)
         return;

     _FileInfo _Info = m_lFileList.at(_nSel);
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
          m_lFileList.removeAt(_nSel);
          ShowFile();
      }
}

 void QFilePage::EditFile()
 {
     _FileInfo _Info;
     if (!GetCurSelFile(_Info))
         return;


      QDir _dir(g_szPath[_Info.nPath]);
      if (SYS_PATH == _Info.nPath && _dir.exists(_Info.szName))
      {
            m_FileInEdit = _Info;

            QString _FilePath = QString("%1%2").arg(g_szPath[_Info.nPath]).arg(_Info.szName);
            QFile _file(_FilePath);
            _file.open(QIODevice::ReadWrite | QIODevice::Text);
             char* _pbuffer = new char[_Info.nSize + 1];
            QDataStream _out(&_file);
            _out.readRawData(_pbuffer,_Info.nSize);
            _pbuffer[_Info.nSize] = 0;
            QString _strContent(_pbuffer);
            m_pCodeEdit->SetEdittext(_strContent);
            _file.close();
            delete [] _pbuffer;
      }
      else
          return;

     ShowEditView(true);
     m_nFileStatus = EDIT_FILE_STATE;
 }

 void QFilePage::NewFile()
 {
     m_pCodeEdit->SetEdittext("");
     ShowEditView(true);
     m_nFileStatus = NEW_FILE_STATE;
 }

 void QFilePage::SaveFile()
 {
    _FileInfo _Info;
    if (m_nFileStatus == EDIT_FILE_STATE)
    {
        _Info = m_FileInEdit;
    }
    else if (m_nFileStatus = NEW_FILE_STATE)
    {
         _Info.nPath = SYS_PATH;

         QStrInputDlg _dlg(this,QStrInputDlg::FILE_NAME_DLG);
         if (!_dlg.exec())
             return;

         _Info.szName = _dlg.GetInputText();
         if (_Info.szName.length() >= 15)
         {
                 QOkDlg _dlg("文件名长度超过15,请重新输入！", this);
                 _dlg.exec();
                 return;
         }

         _Info.szName += ".xtf";
         QDir _dir(g_szPath[_Info.nPath]);
         if (_dir.exists(_Info.szName))
         {
             QOkDlg _dlg("该文件名已经存在！请从新输入.", this);
             _dlg.exec();
             return;
         }
    }
    else
        return;

    QString _FilePath = QString("%1%2").arg(g_szPath[_Info.nPath]).arg(_Info.szName);
    QFile _file(_FilePath);
    _file.open(QIODevice::ReadWrite | QIODevice::Text);
    QDataStream _out(&_file);
    QString _strContent =  m_pCodeEdit->GetCodeText();
    std::string	_stdStr = _strContent.toStdString();
    _out.writeRawData(_stdStr.data(), _strContent.size());
    _file.close();
    ShowEditView(false);
    m_nFileStatus = NORMAL_FILE_STATUS;
    ReListFile();
 }


 void QFilePage::ShowEditView(bool bShow_)
 {
    if (bShow_)
    {
        m_pFileList->hide();
        m_pCodeEdit->show();
        m_pCodeEdit->EnterEditStatus();
    }
    else
    {
        m_pFileList->show();
        m_pCodeEdit->hide();
        m_pCodeEdit->LeaveEditStatus();
    }
 }
