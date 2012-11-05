#include <QtGui>
#include <QTextCodec>
#include "ghead.h"
#include "./ui/mainframe.h"

#include "./core/parseconfig.h"

int main(int argc, char *argv[])
{
       //QApplication app(argc, argv);
       //使能中文
        QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

 #if 0

        QMainFrame _Wind(NULL);
        _Wind.show();

        QFont _font = _Wind.font();
        _font.setPixelSize(20);
        _Wind.setFont (_font);

        return app.exec();
#endif
       CParseConfig _config;
       if( _config.OpenConfigFile("PR.info"))
             _config.StartLoadConfig(NULL);
}










