#ifndef FILEIOWITHCODECHELPER_H
#define FILEIOWITHCODECHELPER_H
#include "public_defines.h"
#include <QMessageBox>

class TEXTCODECUTILSSHARED_EXPORT FileIOWithCodecHelper
{
public:
    bool static backupFile(QFile *file, QWidget *messageBoxParent);
    QString static readTextFileInTextCodec(const QString& path, QTextCodec* textCodec);
    class FileNotExists : public std::runtime_error{
    public:
        FileNotExists();
    };
};

#endif // FILEIOWITHCODECHELPER_H
