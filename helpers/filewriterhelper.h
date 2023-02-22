#ifndef FILEWRITERHELPER_H
#define FILEWRITERHELPER_H
#include "architect/threaddependobject.h"
#include "protocols/mainprotocol.h"
class QFile;
class QTimer;

class FileWriterHelper : public ThreadDependObject
{
    Q_OBJECT
public:
    explicit FileWriterHelper(QObject* parent = nullptr);
    ~FileWriterHelper();

public slots:
    void onWrite(QByteArray data);
    void onWriteNavigationPack(navigationPacket data);
    void onStartWrite(QString fName);
    void onStopWrite();

signals:
    void updateWrittingSize(quint64 size);
    void updateFileState(bool isWriting);

private slots:
    void onFlush();
    void onUpdateSize();

private:
    QFile* _pFile;
    quint64 _writtingSize;
    bool _isWrite;

    QTimer* _pFlushTimer;
    QTimer* _pUpdateSizeTimer;

    virtual int startProcess(QStringList args);
    virtual int quitProcess();
    void setupTimers();
};

#endif // FILEWRITERHELPER_H
