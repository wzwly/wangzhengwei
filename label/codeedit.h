#ifndef CODEEDIT_H
#define CODEEDIT_H

#include <QWidget>
#include <QTextEdit>
#include "button.h"


class QEditKey;

class QCodeEdit : public QWidget
{
    Q_OBJECT
public:
    QCodeEdit(QWidget * parent);

    //
    void SetEdittext(const QString& str_);
    inline QString GetCodeText();
    void EnterEditStatus() {m_bIsEditStatus = true;}
    void LeaveEditStatus() {m_bIsEditStatus = false;}
public slots:
    void OnKeyPushed(int nKey_);

protected:
    virtual void showEvent ( QShowEvent * event );
    virtual void hideEvent ( QHideEvent * event );
    void InitWind();
private:
    QEditKey* m_pKeyBd;
    QTextEdit* m_pTextEdit;
    bool m_bIsEditStatus;
};

inline QString QCodeEdit::GetCodeText()
{
    return m_pTextEdit->toPlainText ();
}


#endif // CODEEDIT_H
