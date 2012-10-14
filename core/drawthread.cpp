#include "drawthread.h"
#include "./../ghead.h"


 QDrawThread* QDrawThread::Instance()
 {
        static QDrawThread* _s_pTread = NULL;
        if (_s_pTread == NULL)
       {
            _s_pTread  = new QDrawThread(NULL);
            g_pListObject.push_back(_s_pTread );
       }
        return _s_pTread;
 }

QDrawThread::QDrawThread(QObject * p_)
{    
         m_nExit = 1;       
         setTerminationEnabled(true);     
}


QDrawThread::~QDrawThread()
{

    while(wait(200))
           msleep(10);
}


 void QDrawThread::run ()
 {
            msleep(3000);  //加速启动          
            while(m_nExit)
            {
                 usleep(10000);
            }
            m_nExit = -1;
 }




