#include "filewriterhelper.h"
#include <QFile>
#include <QTimer>

const int kFlushPeriod = 1000;
const int kUpdateSizePeriod = 250;

FileWriterHelper::FileWriterHelper(QObject* parent) : ThreadDependObject(parent) { _isWrite = false; }

FileWriterHelper::~FileWriterHelper() {}

void FileWriterHelper::onWrite(QByteArray data)
{
    if (_isWrite == true)
        _writtingSize += _pFile->write(data);
}

void FileWriterHelper::onWriteNavigationPack(navigationPacket data)
{
    if (_isWrite == true)
    {
        QString strToWrite = QString("%1;%2;%3;%4;%5;%6;%7;%8;%9;%10\r\n")
                                 .arg(data.header.counter)
                                 .arg(data.ax)
                                 .arg(data.ay)
                                 .arg(data.az)
                                 .arg(data.gx)
                                 .arg(data.gy)
                                 .arg(data.gz)
                                 .arg(data.mx)
                                 .arg(data.my)
                                 .arg(data.mz);
        _writtingSize += _pFile->write(strToWrite.toLocal8Bit());
    }
}

void FileWriterHelper::onStartWrite(QString fName)
{
    _writtingSize = 0;
    emit updateWrittingSize(_writtingSize);

    _pFile->setFileName(fName);
    _isWrite = _pFile->open(QIODevice::WriteOnly);
    emit updateFileState(_isWrite);

    if (_isWrite == true)
    {
        _pFlushTimer->start();
        _pUpdateSizeTimer->start();
    }
    else
        emit showError(QString("Не удалось открыть файл: %1").arg(fName));
}

void FileWriterHelper::onStopWrite()
{
    _isWrite = true;
    _pFile->flush();
    _pFile->close();
    if (_pFile->size() == 0)
        _pFile->remove();

    emit updateFileState(_isWrite);
    emit updateFileState(_writtingSize);
    _pFlushTimer->stop();
    _pUpdateSizeTimer->stop();
}

void FileWriterHelper::onFlush() { _pFile->flush(); }

void FileWriterHelper::onUpdateSize() { emit updateWrittingSize(_writtingSize); }

int FileWriterHelper::startProcess(QStringList args)
{
    Q_UNUSED(args);
    _pFile = new QFile();
    setupTimers();
    return kStandardSuccessCode;
}

int FileWriterHelper::quitProcess()
{
    if (_pFile != nullptr)
    {
        _pFile->close();
        delete _pFile;
    }
    if (_pFlushTimer != nullptr)
    {
        _pFlushTimer->stop();
        delete _pFlushTimer;
    }
    if (_pUpdateSizeTimer != nullptr)
    {
        _pUpdateSizeTimer->stop();
        delete _pUpdateSizeTimer;
    }
    return kStandardSuccessCode;
}

void FileWriterHelper::setupTimers()
{
    _pFlushTimer = new QTimer();
    _pFlushTimer->setSingleShot(false);
    _pFlushTimer->setTimerType(Qt::VeryCoarseTimer);
    _pFlushTimer->setInterval(kFlushPeriod);
    connect(_pFlushTimer, &QTimer::timeout, this, &FileWriterHelper::onFlush);

    _pUpdateSizeTimer = new QTimer();
    _pUpdateSizeTimer->setSingleShot(false);
    _pUpdateSizeTimer->setTimerType(Qt::VeryCoarseTimer);
    _pUpdateSizeTimer->setInterval(kUpdateSizePeriod);
    connect(_pUpdateSizeTimer, &QTimer::timeout, this, &FileWriterHelper::onUpdateSize);
}
