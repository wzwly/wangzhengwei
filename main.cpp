#include <QtGui>
#include <QTextCodec>
#include "ghead.h"
#include "./ui/mainframe.h"

#include "./core/parseconfig.h"
//#include "./core/xtfreader.h"

int main(int argc, char *argv[])
{
       //使能中文
        QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));//UTF-8
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

 #if 1
         QApplication app(argc, argv);
        QMainFrame _Wind(NULL);
        _Wind.show();

        QFont _font = _Wind.font();
        _font.setPixelSize(20);
        _Wind.setFont (_font);

        return app.exec();
#else

//       ConfigData _Data;
//       CParseConfig _config;
//       if( _config.OpenConfigFile("PR.info"))
//             _config.StartLoadConfig(&_Data);

//        CXtfReader _xtf;
//        if( _xtf.OpenConfigFile("testxtf.xtf"))
//            _xtf.StartParseFile(1);

 #endif
}










