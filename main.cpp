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
        QMainFrame _Wind(NULL);
        _Wind.show();

        QFont _font = _Wind.font();
        _font.setPixelSize(20);
        _Wind.setFont (_font);
        return app.exec();
}










