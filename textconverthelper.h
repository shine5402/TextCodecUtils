#ifndef TEXTCONVERTHELPER_H
#define TEXTCONVERTHELPER_H

#include <QObject>
#include "textcodecconvertdialog.h"
#include <QTextCodec>
#include <QMessageBox>
#include <QCoreApplication>
#include "./public_defines.h"
#include "fileiowithcodechelper.h"

class TEXTCODECUTILSSHARED_EXPORT TextConvertHelper
{
public:
    bool static processFileTextCodecConvert(const QString& path, QTextCodec *&sourceCodec, QTextCodec *&targetCodec, QWidget* messageBoxParent);
    bool static processFileNameConvert(const QByteArrayList& _fileNameRaw, const QStringList& _filePaths, const QString& title, QTextCodec *&rawCodec, QTextCodec *&targetCodec, QWidget* messageBoxParent);
    void static writeTextFileInTextCodec(const QString& content, const QString& path, QTextCodec* textCodec);
    void static setDefaultTextCodecName(const QByteArray& codecName);
    const QByteArray& getDefaultTextCodecName();
};

#endif // TEXTCONVERTHELPER_H
