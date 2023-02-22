#include "loghelper.h"
#include "defines/applicationconstants.h"
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMutex>

static QFile* logFile;
static QMutex* fileMutex;

void logOutput(QtMsgType type, const QMessageLogContext& contex, const QString& msg)
{
    Q_UNUSED(contex);
    if (type < kLogLevel)
        return;

    QByteArray notePrefix = QByteArray(24, ' ');
    QString timeMark = QString("[%1] ").arg(QDateTime::currentDateTime().toString("dd.MM.yy hh:mm:ss:z"));
    notePrefix.replace(0, timeMark.length(), timeMark.toLocal8Bit());
    fileMutex->lock();
    switch (type)
    {
        case QtDebugMsg:
            notePrefix.append("(Debug)   ");
            break;
        case QtWarningMsg:
            notePrefix.append("(Warning) ");
            break;
        case QtCriticalMsg:
            notePrefix.append("(Critical)");
            break;
        case QtInfoMsg:
            notePrefix.append("(Info)    ");
            break;
        case QtFatalMsg:
            notePrefix.append("(Fatal)   ");
            logFile->write(notePrefix + msg.toLocal8Bit() + "\r\n");
            logFile->flush();
            logFile->close();
            abort();
    }

    logFile->write(notePrefix + msg.toLocal8Bit() + "\r\n");
    logFile->flush();
    fileMutex->unlock();
}

void setupLog()
{
    QDir dir;
    dir.setPath(kLogPath);
    if (!dir.exists())
        dir.mkpath(kLogPath);

    fileMutex = new QMutex();
    fileMutex->lock();
    logFile = new QFile(kLogPath + QDate::currentDate().toString("log(dd_MM_yyyy).log"));
    logFile->open(QIODevice::Append);
    fileMutex->unlock();

    qInstallMessageHandler(logOutput);
    qInfo() << "Настроен хэндлер вывода в лог";
}

QString getLogPath()
{
    fileMutex->lock();
    QFileInfo info(*logFile);
    fileMutex->unlock();
    return info.absolutePath();
}
