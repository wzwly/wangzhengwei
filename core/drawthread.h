#ifndef DRAWTHREAD_H
#define DRAWTHREAD_H

#include <QThread>


class QDrawThread : public QThread
{
    Q_OBJECT
public:
    static QDrawThread* Instance();
     ~QDrawThread();

     void StartThread(Priority p_) {m_nExit = 1; start(p_); }
     void ExitThread(){m_nExit = 0;}
 public
         slots:
        // void OnExit();
protected:
    QDrawThread(QObject * p_);
    virtual void	run ();

private:
    int m_nExit;
};

#endif // DRAWTHREAD_H
