#ifndef EDITVIEW_H
#define EDITVIEW_H

class QEditView
{
public:
    QEditView();
};


//use qfile dang qstream to read write qstring
//qstring can be geted or setted to qeditview

/*
QFile file("file.dat");
file.open(QIODevice::WriteOnly);
QDataStream out(&file);   // we will serialize the data into the file
out << QString("the answer is");   // serialize a string
out << (qint32)42;        // serialize an integer

QString	toPlainText () const
void	setPlainText ( const QString & text )
*/

/*=======================
0   1   2   3   .   ^   BkSp
4   5   6   7   <   v   >
8   9   -   x   y   Enter
=======================*/
#endif // EDITVIEW_H
