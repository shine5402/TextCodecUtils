#include "fileiowithcodechelper.h"

bool FileIOWithCodecHelper::backupFile(QFile* file,QWidget* messageBoxParent)
{
    auto path = QFileInfo(*file).absoluteFilePath();
    QFile bakFile(path + ".bak");
    if (bakFile.exists())
        bakFile.remove();
    if (!file->copy(path + ".bak"))
    {
        QMessageBox::critical(messageBoxParent,QCoreApplication::translate("TextConvertHelper", "无法备份%1").arg(path + ".bak"),QCoreApplication::translate("TextConvertHelper", "<h3>程序无法对%1进行备份</h3><p>在备份时出现错误。Qt提供的错误说明为：%2</p><p>你仍可以令程序继续转换，但是之前提到的<b>风险</b>仍然存在，且出现问题时您无法恢复。</p><p>确定要继续转换吗？</p>").arg(path).arg(file->errorString()));
        return false;
    }
    return true;
}

QString FileIOWithCodecHelper::readTextFileInTextCodec(const QString& path, QTextCodec *textCodec)
{
    QFile* file = new QFile(path);
    if (!file->exists()){
        file->deleteLater();
        throw FileNotExists();}
    else
    {
        if (file->open(QIODevice::ReadOnly | QIODevice::Text)){
            auto RawData = file->readAll();
            file->close();
            file->deleteLater();
            QTextDecoder *decoder = textCodec->makeDecoder();
            auto String = decoder->toUnicode(RawData);
            delete decoder;
            return String;
        }
        else
        {
            qCritical() << QCoreApplication::translate("FileIOWithCodecHelper", "读取%1时发生错误。错误描述为：%2").arg(path).arg(file->errorString());
        }
    }
    return QString();
}

FileIOWithCodecHelper::FileNotExists::FileNotExists():std::runtime_error("File not exists."){}
