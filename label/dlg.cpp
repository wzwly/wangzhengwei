#include "dlg.h"
#include  "./../ui/mainframe.h"
//#include "./../lib/configset.h"
#include <QDir>
#include "item.h"
#include "button.h"
#include "softkey.h"

#define ID_OK   1
#define ID_CANCEL 0

QBaseDlg::QBaseDlg(QWidget * parent_)
 :QDialog(parent_)
{
    setModal(true);
    setFixedSize(450, 250);				//set size
    QPoint _point(250, 230);
    move(GetMainFrame()->mapToGlobal(_point));
    setAutoFillBackground(true);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    setStyleSheet("QDialog{border: 2px solid transparent;\
                           background: rgba(150, 172, 213);border-color: rgba(80, 40, 1);}");

     m_eLang  =  GetMainFrame()->GetLangType();
     m_pOkBtn = new QClickBtn(this,  ID_OK);
     m_pOkBtn->InitShow("Ok",80, 160, 75, 60);

     m_pCancelBtn = new QClickBtn(this, ID_CANCEL);
     m_pCancelBtn->InitShow("Cancel", 295, 160,75, 60);
     connect(m_pOkBtn, SIGNAL(ClickedEvent(int)), this, SLOT(OnBtnClicked(int)));
     connect(m_pCancelBtn, SIGNAL(ClickedEvent(int)), this, SLOT(OnBtnClicked(int)));
}


void QBaseDlg::OnBtnClicked(int nId_)
{
     if (!OnOkCancel(nId_))
             nId_ =ID_CANCEL;
     done(nId_);
}
//===========================================
//===========================================

QOkDlg::QOkDlg(const QString& str_,QWidget* parent_)
    :QBaseDlg(parent_)
{

    m_pInputTip = new QTipLabel(this,  QItem::LABEL_DLG);
    m_pInputTip->InitShow(2,1, 446, 150, 16);
    m_pInputTip->setText(str_);
}

QOkDlg::QOkDlg(const char*str_,QWidget* parent_)
{
    m_pInputTip = new QTipLabel(this,  QItem::LABEL_DLG);
    m_pInputTip->InitShow(str_, 2,1, 446, 150, 16);
}

QOkDlg::~QOkDlg()
{

}

//============================================
//============================================
QStrInputDlg::QStrInputDlg(QWidget * parent_, STR_DLG_TYPE eDlg_ )
 :QBaseDlg(parent_)
{
    setFixedSize(350, 180);				//set size
    QPoint _point(260, 150);
    move(GetMainFrame()->mapToGlobal(_point));
    setModal(false);
    m_pInputTip = new QTipLabel(this,  QItem::LABEL_DLG);
    m_pInputTip->InitShow(2,1, 346, 48, 16);
    m_pInputText = new QLineEdit(this);
    m_pInputText->move(50, 50);
    m_pInputText->setFixedSize(250, 50);
    m_pInputText->setStyleSheet("QLineEdit{font-size:18px;}");

    m_pOkBtn->InitShow(50, 110, 60, 50);
    m_pCancelBtn->InitShow(240, 110,60, 50);

     m_pKeyBoard = NULL;
     InitialData(eDlg_);
}

 QStrInputDlg::~QStrInputDlg()
 {
        delete m_pKeyBoard;
 }

 const static char* g_szStrInputTip[LANG_NUM] = {"请输入字符串名", "Please input a string"};
 const static char* g_szNameInputTip[LANG_NUM] = {"请输入文件名", "Please input a string"};
 void QStrInputDlg::InitialData(STR_DLG_TYPE eDlg_)
 {
            if (eDlg_ == STR_INPUT_DLG)
            {
                m_pInputTip->setText(g_szStrInputTip[m_eLang]);
            }
            else if (eDlg_ == FILE_NAME_DLG)
            {
                m_pInputTip->setText(g_szNameInputTip[m_eLang]);
            }


             m_pKeyBoard = new  QAllKeyView(this);

             QPoint _P(60, 335);
             _P = GetMainFrame()->mapToGlobal(_P);
             m_pKeyBoard->move(_P);
             m_pKeyBoard->show();
             connect(m_pKeyBoard, SIGNAL(SendCharacter(int)), this, SLOT(OnKeyPushed(int)));
 }


void QStrInputDlg::OnKeyPushed(int nKey_)
{
        if (nKey_ == Qt::Key_Escape || nKey_ == Qt::Key_Return)
        {
                OnBtnClicked(nKey_ == Qt::Key_Return ? ID_OK : ID_CANCEL);
                return;
        }

        if (nKey_ == Qt::Key_Backspace)
        {
                m_pInputText->end(false);
                m_pInputText->backspace();
        }
        else if ((nKey_ >= Qt::Key_0 && nKey_ <= Qt::Key_9 )||
                 (nKey_ >= Qt::Key_A && nKey_ <= Qt::Key_Z )||
                 (nKey_ >= Qt::Key_A + 32 && nKey_ <= Qt::Key_Z + 32 )
            || (nKey_  == Qt::Key_Minus))
        {
                m_pInputText->end(false);
                m_pInputText->insert(QString(QChar(nKey_))) ;
        }

}
//=======================================
//=======================================

#define ENTERN_KEY 15
#define ESC_KEY  7
#define BKSPACE_KEY 3

#define COLOR_BACK  QColor(82, 107, 107 , COLOR_F)

static const char* g_szKeyVal[16] =
{"7", "8", "9", "回格",
"4", "5", "6", "取消",
"1", "2", "3","+",
"0", ".", "-", "确认"};

static const int g_cKeyVal[16] =
{Qt::Key_7, Qt::Key_8, Qt::Key_9, Qt::Key_Backspace,
Qt::Key_4, Qt::Key_5, Qt::Key_6, Qt::Key_Escape,
Qt::Key_1, Qt::Key_2, Qt::Key_3, Qt::Key_Plus,
Qt::Key_0,  Qt::Key_Period, Qt::Key_Minus,  Qt::Key_Return};


QNuberInput::QNuberInput(QWidget* parent_)
:QDialog(parent_, Qt::Widget | Qt::FramelessWindowHint)
{
    setFixedSize(300, 382);	//set size
    setAutoFillBackground(true);
    QPalette _pal = this->palette();	//set back ground
    _pal.setColor(QPalette::Background, COLOR_BACK);
    this->setPalette(_pal);
        setModal(true);
    //按钮
    for (int _i = 0; _i < 16 ; ++_i)
    {
         m_pBtnPush[_i] = new QClickBtn(this, _i);
         m_pBtnPush[_i]->setFixedSize(71, 64);
         m_pBtnPush[_i]->move(2 + 75 * (_i % 4), 120 + 66 * (_i / 4) );
         m_pBtnPush[_i]->setStyleSheet("QPushButton {background-color:rgba(38, 54, 61); color:white; font-size:18px}\
                          QPushButton:pressed{background-color: rgba(115,95,65);}");
         m_pBtnPush[_i]->setText(g_szKeyVal[_i]);
            connect(m_pBtnPush[_i],SIGNAL(ClickedEvent(int)), this, SLOT(OnBtnPushed(int)));
    }

        m_pEdit = new QLineEdit(this);
        m_pEdit->setFixedSize(296,54);
        m_pEdit->move(1,62);
        m_pEdit->setStyleSheet("QLineEdit{font-size:18px;}");

        m_pRange = new QTipLabel(this, QItem::LABEL_DLG);
        m_pRange->InitShow(1,1,298,60,15);
        //m_pRange->move(1, 1);

        SetRange(0, 1000.0, 12);

}
QNuberInput::~QNuberInput()
{

}


void QNuberInput::OnBtnPushed(int nIndex_)
{
         //qDebug() << nIndex_;
         if (nIndex_ == ENTERN_KEY || ESC_KEY == nIndex_)
         {
                 done(nIndex_ == ENTERN_KEY ? 1 : 0);
                 return;
         }

         m_pEdit->end(false);

         if (BKSPACE_KEY == nIndex_)
                 m_pEdit->backspace();

         int _nKeyVal = g_cKeyVal[nIndex_];
         QString _strOld = m_pEdit->text();
         if (_nKeyVal >= Qt::Key_0 && _nKeyVal <= Qt::Key_9)
         {
                 m_pEdit->insert(g_szKeyVal[nIndex_]);

         }
         else if (_nKeyVal == Qt::Key_Period)
         {
                 if (m_pEdit->text().contains("."))
                         return;
                 m_pEdit->insert(g_szKeyVal[nIndex_]);
         }
         else if (_nKeyVal == Qt::Key_Plus)
         {
                 if (m_pEdit->text().contains("-"))
                 {
                        m_pEdit->home(false);
                        m_pEdit->del();
                 }
         }
         else if (_nKeyVal == Qt::Key_Minus)
         {
                 if (m_pEdit->text().contains("-"))
                         return;
                        m_pEdit->home(false);
                        m_pEdit->insert(g_szKeyVal[nIndex_]);
         }

         IsValidData( _strOld);

}

void QNuberInput::SetRange(double dMin_, double dMax_, const QString& str_)
{
        m_dMin = dMin_; m_dMax = dMax_;
        m_pRange->setText(QString("最小值:%1 最大值:%2").arg(m_dMin).arg(m_dMax));
        m_pEdit->setText(str_);
}


void QNuberInput::SetRange(double dMin_, double dMax_, double dVal_)
{
        m_dMin = dMin_; m_dMax = dMax_;

        m_pRange->setText(QString("最小值:%1   最大值:%2").arg(m_dMin).arg(m_dMax));
        m_pEdit->setText(QString::number(dVal_));
}


bool QNuberInput::IsValidData(QString& strOld_)
{

        double _dData =  GetDbData();
        if (_dData < m_dMin || _dData > m_dMax)
        {
                m_pEdit->setText(strOld_);
                return false;
        }
        return true;
}



void QNuberInput::SetRange(double dMin_, double dMax_, float fVal_)
{
        m_dMin = dMin_; m_dMax = dMax_;

        m_pRange->setText(QString("最小值:%1   最大值:%2").arg(m_dMin).arg(m_dMax));
        m_pEdit->setText(QString::number(fVal_));
}

void QNuberInput::SetRange(double dMin_, double dMax_, int nVal_)
{
        m_dMin = dMin_; m_dMax = dMax_;

        m_pRange->setText(QString("最小值:%1   最大值:%2").arg(m_dMin).arg(m_dMax));
        m_pEdit->setText(QString::number(nVal_));
}

//=======================================
//=======================================
/*
QStreeViewDlg::QStreeViewDlg(QWidget * parent_, QStreeViewDlg::TREE_DLG_TYPE eDlg_)
  :QBaseDlg(parent_)
{
         m_eDlgType =  eDlg_;
         m_pWedge = NULL;
         m_pProbe = NULL;

         setFixedSize(450, 450);	//set size
         m_pOkBtn->InitShow(105, 403, 60, 45);
         m_pCancelBtn->InitShow(295, 403,60, 45);

         m_pMainList = new  QListWidget(this);
         m_pSubList =  new  QListWidget(this);

        m_pMainList->setFixedSize(220,315);
        m_pMainList->move(3, 2);

        m_pSubList ->setFixedSize(220,315);
        m_pSubList ->move(226, 2);

        m_pInputTip = new QTipLabel(this,  LABEL_DLG, Qt::AlignLeft);
        m_pInputTip->InitShow(3,320, 444, 80, 14);

        connect(m_pMainList, SIGNAL(currentRowChanged(int)), this, SLOT(OnMainSelChange(int)));
        connect(m_pSubList, SIGNAL(currentRowChanged(int)), this, SLOT(OnSubSelChange(int)));

        m_pMainList->setStyleSheet("QListWidget{border: 2px solid transparent;background: rgba(220, 222, 213);}\
                                                QListView::item:selected{ background: rgba(120, 142, 113);}");
       m_pSubList->setStyleSheet("QListWidget{border: 2px solid transparent;background: rgba(220, 222, 213);}\
                                                QListView::item:selected{ background: rgba(120, 142, 113);}");

       QPoint _point(150, 90);
       move(GetMainFrame()->mapToGlobal(_point));
}


QStreeViewDlg::~QStreeViewDlg()
{
    if (m_pWedge != NULL)
        delete m_pWedge;
    if (m_pProbe != NULL)
        delete m_pProbe;
}


void QStreeViewDlg::OnMainSelChange(int nRow_)
{
    if (m_pMainList->count() > 0)
    {
           QListWidgetItem* _p;
           _p =  m_pMainList->currentItem();
            if (_p != NULL)
                ListSubViewFile(QString("%1/%2/").arg(m_strBasePath).arg(_p->text()));
    }
}

void QStreeViewDlg::OnSubSelChange(int nRow_)
{
    if (m_pSubList->count() > 0)
    {
           QListWidgetItem* _p;
            _p =  m_pSubList->currentItem();

            if (_p != NULL)
             {
                    m_strFileName = _p->text();
                    GetProbeWedgeInfo();
                    return;
             }
    }

    m_strFileName.clear();
}

void QStreeViewDlg::InitfileList(const QString& strPath_)
{
        QDir _dir(strPath_);
        _dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

         QFileInfoList _list = _dir.entryInfoList();
         QFileInfo _fileInfo;
         QListWidgetItem* _p;
         for (int _i = 0; _i < _list.size(); ++_i)
         {
                 _fileInfo = _list.at(_i);            
                 _p =  new QListWidgetItem(_fileInfo.fileName(), m_pMainList);
         }

         m_strBasePath = strPath_;
         if (m_pMainList->count() > 0)
         {
                m_pMainList->setCurrentRow(0);
                 _p =  m_pMainList->currentItem();
                ListSubViewFile(QString("%1/%2/").arg(strPath_).arg(_p->text()));
         }
}

 void QStreeViewDlg::ListSubViewFile(const QString& strPath_)
 {
      QDir _dir(strPath_);
      _dir.setFilter(QDir::Files | QDir::Readable);
        m_strFullPath = strPath_;
      QFileInfoList _list = _dir.entryInfoList();
      QFileInfo _fileInfo;
      QListWidgetItem* _p;
      m_pSubList->clear();
      for (int _i = 0; _i < _list.size(); ++_i)
      {
              _fileInfo = _list.at(_i);
              _p =  new QListWidgetItem(_fileInfo.fileName(), m_pSubList);
      }

      if (m_pSubList->count() > 0)
      {
             m_pSubList->setCurrentRow(0);
             OnSubSelChange(0);
             GetProbeWedgeInfo();
       }
      else
      {
         m_strFullPath.clear();
         m_pInputTip->setText("");
       }
 }


bool QStreeViewDlg::OnOkCancel(int nKey_)
 {
       if (nKey_ == ID_OK)
      {
           QListWidgetItem* _p;
            _p =  m_pSubList->currentItem();
            if (_p == NULL)
                return false;
           m_strFileName = _p->text();
      }
       return true;
 }

const static char* g_szUtProbeShow[LANG_NUM] = {"型号: %1     频率: %2MHz\n阵元大小: %3 ",""};
const static char* g_szPaProbeShow[LANG_NUM] = {"型号: %1     频率: %2MH z\n阵元数量: %3     阵元间距: %4mm\n参考点: %5mm",""};

const static char* g_szUtWeldgeShow[LANG_NUM] = {"型号: %1     角度: %2°\n波形: %3    契块延时: %4us\n参考点: %5mm",""};
const static char* g_szPaWeldgeShow[LANG_NUM] = {"型号: %1     角度: %2°\n方向: %3    速度: %4m/s   高度: %5mm\n主偏置: %6mm      次偏置: %7mm",""};

void QStreeViewDlg::GetProbeWedgeInfo()
{
      if (m_strFullPath.isEmpty() || m_strFileName.isEmpty())
          goto EMPTY_SEL;

        if (m_eDlgType ==  PROBE_SEL_DLG)
        {            
            if (m_pProbe == NULL)
                m_pProbe = (PROBE*)malloc(sizeof(PROBE));
            int _nOffset = 0;
            if (m_strFullPath.contains("PA"))
                _nOffset += 4;

            if (!QConfigSet::ReadFromFile( m_strFullPath + m_strFileName, m_pProbe, sizeof(PROBE) - _nOffset ,_nOffset))
            {
                   memset(m_pProbe, 0, sizeof(PROBE));
                    assert(false);
            }

            QString _str;
            if (_nOffset == 0)
             {
                    int _nFreq = m_pProbe->cProbeRW.ucFreq2 * 256 + m_pProbe->cProbeRW.ucElemQty;
                    _str =  QString(g_szUtProbeShow[m_eLang]).arg(m_pProbe->cProbeRW.cModel).
                     arg(float( _nFreq) / 1000.0).arg(float(m_pProbe->cProbeRW.iPitch)/ 1000.0);
              }
            else
            {
                    _str =  QString(g_szPaProbeShow[m_eLang]).arg(m_pProbe->cProbeRW.cModel).
                    arg(float( m_pProbe->cProbeRW.wFrequency) / 1000.0).arg(m_pProbe->cProbeRW.ucElemQty).
                    arg(float( m_pProbe->cProbeRW.iPitch) / 1000.0).arg(-float(m_pProbe->cProbeRW.wRefPoint) / 1000.0);
            }

            m_pInputTip->setText(_str);

        }
        else  if (m_eDlgType ==  WELDGE_SEL_DLG)
        {
             if (m_pWedge == NULL)
                 m_pWedge = (WEDGE*)malloc(sizeof(WEDGE));

             if (!QConfigSet::ReadFromFile( m_strFullPath + m_strFileName, m_pWedge, 52 ,0)
                 || !QConfigSet::ReadFromFile( m_strFullPath + m_strFileName, (char*)(m_pWedge) + 52, 64 ,53))
            {
                 memset(m_pWedge, 0, sizeof(WEDGE));
                  assert(false);
            }
            QString _str;
             if (!m_strFullPath.contains("PA"))
             {
                 _str =  QString(g_szUtWeldgeShow[m_eLang]).arg(m_pWedge->cWedgeRW.szModel).
                  arg(float(m_pWedge->cWedgeRW.wAngle) / 10.0).arg(m_pWedge->cWedgeRW.cWaveType == 1 ? "Longitudinal" : "Shear").
                  arg(float(m_pWedge->cWedgeRW.wProbeDelay)/1000.0).arg(float(m_pWedge->cWedgeRW.iRefPoint)/1000.0);
             }
             else
             {
                 _str =  QString(g_szPaWeldgeShow[m_eLang]).arg(m_pWedge->cWedgeRW.szModel).
                  arg(float(m_pWedge->cWedgeRW.wAngle) / 10.0).arg(m_pWedge->cWedgeRW.cOrientation == 0 ? "Reversal" : "Normal").
                  arg(float(m_pWedge->cWedgeRW.iVelocityPa)/1000.0).arg(float(m_pWedge->cWedgeRW.iHeight)/1000.0).
                  arg(float(m_pWedge->cWedgeRW.iPrimaryOffset)/1000.0).arg(float(m_pWedge->cWedgeRW.iSecondaryOffset)/1000.0);
             }
             m_pInputTip->setText(_str);
        }

        return ;

 EMPTY_SEL:
         m_pInputTip->setText("");
}


bool QStreeViewDlg::GetWedgeParam(WEDGE* pWedge_)
{
        if (m_pWedge == NULL)
           return false;
        pWedge_->cWedge.cWaveType = m_pWedge->cWedgeRW.cWaveType;
        pWedge_->cWedge.cOrientation = m_pWedge->cWedgeRW.cOrientation;        
        pWedge_->cWedge.fAngle = float(m_pWedge->cWedgeRW.wAngle) / 10.0;
        pWedge_->cWedge.dWedgeDelay = double(m_pWedge->cWedgeRW.wProbeDelay) / 1000.0;
        pWedge_->cWedge.dRefPoint = float(m_pWedge->cWedgeRW.iRefPoint)/1000.0;       
        pWedge_->cWedge.dVelocityUt = double(m_pWedge->cWedgeRW.iVelocityUt) / 1000.0;
        pWedge_->cWedge.dVelocityPa = double(m_pWedge->cWedgeRW.iVelocityPa) / 1000.0;
        pWedge_->cWedge.dHeight = float(m_pWedge->cWedgeRW.iHeight)/1000.0;
        pWedge_->cWedge.dPrimaryOffset = float(m_pWedge->cWedgeRW.iPrimaryOffset)/1000.0;
        pWedge_->cWedge.dSecondOffset = float(m_pWedge->cWedgeRW.iSecondaryOffset)/1000.0;

        memcpy( pWedge_->cWedge.szModel, m_pWedge->cWedgeRW.szModel, 20);
        memcpy( pWedge_->cWedge.szSerial, m_pWedge->cWedgeRW.szSerial, 20);
        return true;
}

bool QStreeViewDlg::GetProbeParam(PROBE* pProbe_)
{
        if (m_pProbe == NULL)
             return false;
        pProbe_-> cProbe.PaProbeType = m_pProbe->cProbeRW.PaProbeType;
        pProbe_-> cProbe.UtProbeType = m_pProbe->cProbeRW.UtProbeType;        
        pProbe_-> cProbe.ucElemQty = m_pProbe->cProbeRW.ucElemQty;

        pProbe_-> cProbe.fFreqUt = float(m_pProbe->cProbeRW.ucFreq2 * 256 + m_pProbe->cProbeRW.ucElemQty) / 1000.0;
        pProbe_-> cProbe.fFreqPa = float(m_pProbe->cProbeRW.wFrequency) / 1000.0;
        pProbe_-> cProbe.dPich = float(m_pProbe->cProbeRW.iPitch)/ 1000.0;
        pProbe_-> cProbe.dPichSize =  pProbe_-> cProbe.dPich;
        pProbe_-> cProbe.dRefPoint = float(m_pProbe->cProbeRW.wRefPoint) / 1000.0;
        memcpy( pProbe_->cProbe.cModel, m_pProbe->cProbeRW.cModel, 20);
        memcpy( pProbe_->cProbe.cSerial, m_pProbe->cProbeRW.cSerial, 20);
}
*/
