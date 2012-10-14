#ifndef __SOFTKEY_H__
#define __SOFTKEY_H__

#include <QInputContext>
class QClickBtn;
class QSoftKeyView ;
class QAllKeyView;


//软键盘管理以及实现
class QSoftKey : public QInputContext
{
    Q_OBJECT

public:
    ~QSoftKey();
     static QSoftKey* Instance();
protected:
     QSoftKey();
public:
    virtual QString	identifierName (){};
    bool	isComposing () const{return false;}
    virtual QString	language (){};
    virtual void	reset (){};

public:
    void Show(bool bShow_, bool bAllKey_ = false);

private slots:
    void SendKey(int nKey_);

private:
    QSoftKeyView* m_pSoftKey;
    QAllKeyView* m_pAllKeyView;
};


class QSoftKeyView : public QWidget
{
        Q_OBJECT
public:
        QSoftKeyView(QWidget* parent_ = NULL);
        ~QSoftKeyView();

public slots:
    void OnBtnPushed(int nIndex_);
    void SaveFocusWidget(QWidget *pOldFocus_, QWidget *pNewFocus_);
    signals:
    void SendCharacter(int nKey_);

 private:
    QClickBtn* m_pBtnPush[20];
    QWidget* m_pOldFocalWidget;
};


class QAllKeyView :  public QWidget
{
     Q_OBJECT
   public:
        QAllKeyView(QWidget* parent_ = NULL);
       ~QAllKeyView();

   public slots:
       void OnBtnPushed(int nIndex_);
       void SaveFocusWidget(QWidget *pOldFocus_, QWidget *pNewFocus_);
       signals:
       void SendCharacter(int nKey_);

 private:
       QClickBtn* m_pBtnPush[48];
       QWidget* m_pOldFocalWidget;
       bool  m_bUpCaps;

};
#endif // SOFTKEY_H
