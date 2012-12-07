#ifndef CODEEDIT_H
#define CODEEDIT_H

#include <QWidget>
#include "button.h"

class QCodeEdit : public QWidget
{
    Q_OBJECT
public:
    QCodeEdit(QWidget * parent);

    enum {
        EDIT_KEY_NUM = 20,
    };
private:
    void InitWind();
private:
    QClickBtn* m_pKeyBtn[EDIT_KEY_NUM];
};

#endif // CODEEDIT_H
