#ifndef QCHARDET_H
#define QCHARDET_H

#include <QObject>
#include <QTextCodec>
//#include "./public_defines.h"
#include "textcodecutils_global.h"
#include <QFile>

class TEXTCODECUTILSSHARED_EXPORT QChardet
{
public:
    static QTextCodec *encodingForByteArray(QByteArray string, float *confidence = nullptr);
    static QTextCodec *encodingForFile(QFile* file,float *confidence = nullptr);
    static QTextCodec *encodingForFile(QString fileName,float *confidence = nullptr);
};

#endif // QCHARDET_H
