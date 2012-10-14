#include <QtGui>
#include <QTextCodec>
#include "ghead.h"
#include "./ui/mainframe.h"
//#include "./label/glabel.h"

int main(int argc, char *argv[])
{
       QApplication app(argc, argv);

       //使能中文
        QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

        //QSoftKey* _g_pSoftKey = QSoftKey::Instance();
        //app.setInputContext( _g_pSoftKey);
        QMainFrame _Wind(NULL);
        _Wind.show();

        QFont _font = _Wind.font();
        _font.setPixelSize(20);
        _Wind.setFont (_font);
        //Q_INIT_RESOURCE(Resource);//包含资源
        return app.exec();
}










