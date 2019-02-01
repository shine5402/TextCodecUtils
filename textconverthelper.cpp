﻿#include "textconverthelper.h"

bool TextConvertHelper::backupFile(QFile* file,QWidget* messageBoxParent)
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

bool TextConvertHelper::processFileTextCodecConvert(const QString& path, QTextCodec *&sourceCodec, QTextCodec *&targetCodec, QWidget* messageBoxParent){

    bool isDone = false;
    QFile* file = new QFile(path);
    if (!file->exists())
    {
        file->deleteLater();
        QMessageBox::warning(messageBoxParent,QCoreApplication::translate("TextConvertHelper", "文件不存在"),QCoreApplication::translate("TextConvertHelper", "文件%1不存在").arg(path));
        return false;
    }

    QByteArray rawData;
    if (file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        rawData = file->readAll();
        file->close();
    }

    auto dialog = new TextCodecConvertDialog(path,rawData,sourceCodec,targetCodec,false,messageBoxParent);
    auto dialogCode = dialog->exec();
    if (dialogCode == QDialog::Accepted){
        auto infoDialogCode = QMessageBox::information(messageBoxParent,
                                                       QCoreApplication::translate("TextConvertHelper", "即将执行编码转换"),
                                                       QCoreApplication::translate("TextConvertHelper", "<h3>程序即将对%1执行编码转换（%2 -> %3）</h3>"
                                                          "<p>在您单击确定后，程序将会把转换后的结果保存至%1。</p>"
                                                          "<p>但是，程序有必要提醒您编码转换的<b>风险</b>："
                                                          "由于源编码和目标编码间的可能的映射不对等关系，这种转换可能<b>不可逆</b>，"
                                                          "并且可能使您<b>丢失数据</b>！</p>"
                                                          "<p>出于安全考虑，程序将保存一份源文件的备份副本（%1.bak），以便出现问题时您可以手动恢复。</p>"
                                                          "<p>确定要执行转换吗？</p>")
                                                       .arg(path)
                                                       .arg(QString::fromUtf8(dialog->getSourceCodec()->name()))
                                                       .arg(QString::fromUtf8(dialog->getTargetCodec()->name())),
                                                       QMessageBox::Ok | QMessageBox::Cancel);
        if (infoDialogCode == QMessageBox::Ok){
            targetCodec = dialog->getTargetCodec();
            if (backupFile(file,messageBoxParent)){
                if (file->open(QIODevice::WriteOnly | QIODevice::Text)){
                    auto fileWriteCode = file->write(dialog->getEncodedTargetByteArray());
                    if (fileWriteCode == -1){
                        QMessageBox::critical(messageBoxParent,QCoreApplication::translate("TextConvertHelper", "转换失败"),QCoreApplication::translate("TextConvertHelper", "<h3>程序无法对%1进行写入</h3><p>在写入时出现错误。Qt提供的错误描述为%2。</p><p>文件应该没有被修改。</p>").arg(path).arg(file->errorString()));
                    }
                    else
                    {
                        QMessageBox::information(messageBoxParent,QCoreApplication::translate("TextConvertHelper", "转换成功"),QCoreApplication::translate("TextConvertHelper", "<h3>文件编码转换完成</h3><p>程序将自动修改该文件的读取用文本编码，之后将实施重载（如果需要）。</p>"));
                        isDone = true;
                    }
                }
            }
        }
    }

    if (file->isOpen())
        file->close();
    file->deleteLater();
    dialog->deleteLater();
    return isDone;
}

bool TextConvertHelper::processFileNameConvert(const QByteArrayList &_fileNameRaw, const QStringList &_filePaths, const QString &title, QTextCodec *&rawCodec, QTextCodec *&targetCodec, QWidget* messageBoxParent)
{
    auto fileNameRaw = _fileNameRaw;
    auto showString = fileNameRaw.join("\n");
    auto dialog = new TextCodecConvertDialog(title,showString,rawCodec,targetCodec,true,messageBoxParent);
    auto dialogCode = dialog->exec();
    if (dialogCode == QDialog::Accepted){
        auto sourceCodec = dialog->getSourceCodec();
        auto _targetCodec = dialog->getTargetCodec();
        QTextEncoder encoder(_targetCodec);
        QTextDecoder decoder(sourceCodec);
        QTextDecoder decoderLocale(QTextCodec::codecForLocale());
        auto filePaths = _filePaths;
        auto it = filePaths.begin();
        QStringList unsucess;
        while (it != filePaths.end())
        {
            auto file = new QFile(*it);
            if (file->exists()) {
                QFileInfo fileInfo(*it);
                auto newName = decoderLocale.toUnicode(encoder.fromUnicode(decoder.toUnicode(fileInfo.fileName().toLocal8Bit())));
                auto newPath = fileInfo.absolutePath() + "/" + newName;
                if (newName != fileInfo.fileName())
                {
                    if (!file->rename(newPath)) {
                        unsucess.append(QCoreApplication::translate("TextConvertHelper", "%1（%2）").arg(file->fileName()).arg(file->errorString()));
                        LeafLogger::LogMessage(QString("文件重命名时发生错误。QFile的错误信息为%1。").arg(file->errorString()));
                    }
                }
            }
            file->deleteLater();
            ++it;
        }
        if (!unsucess.isEmpty()){
            QMessageBox::warning(messageBoxParent,QCoreApplication::translate("TextConvertHelper", "转换中出了些问题"),QCoreApplication::translate("TextConvertHelper", "<h3>程序在转换以下文件时出了些错误</h3><pre>%1</pre><p>这些文件应当都保持在原有的状态。您可以排查问题后重试。</p>").arg(unsucess.join("\n")));
            return false;
        }
        else
        {
            rawCodec = sourceCodec;
            targetCodec = _targetCodec;
            return true;
        }
    }
    dialog->deleteLater();
    return false;
}

void TextConvertHelper::writeTextFileInTextCodec(const QString& content, const QString& path, QTextCodec* textCodec)
{
    QFile* file = new QFile(path);
    if (file->open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextEncoder *encoder= textCodec->makeEncoder();
        file->write(encoder->fromUnicode(content));
        delete encoder;
        file->close();
        file->deleteLater();
    }
    else
    {
        LeafLogger::LogMessage(QString("写入模式打开%1时发生错误。错误描述为：%2").arg(path).arg(file->errorString()));
    }
}

void TextConvertHelper::setDefaultTextCodecName(const QByteArray& codecName)
{
    defaultTextCodecName = codecName;
}

const QByteArray& TextConvertHelper::getDefaultTextCodecName()
{
    return defaultTextCodecName;
}
