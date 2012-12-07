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
    inline QString GetCodeText() const;
public slots:
    void OnKeyPushed(int nKey_);

protected:
    virtual void showEvent ( QShowEvent * event );
    virtual void hideEvent ( QHideEvent * event );
    void InitWind();
private:
    QEditKey* m_pKeyBd;
    QTextEdit* m_pTextEdit;
};

#endif // CODEEDIT_H
