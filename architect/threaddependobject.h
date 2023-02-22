#ifndef THREADDEPENDOBJECT_H
#define THREADDEPENDOBJECT_H

#include <QObject>

const int kStandardSuccessCode = 0;
const int kStandardUnsuccessCode = -1;

class ThreadDependObject : public QObject
{
    Q_OBJECT
public:
    explicit ThreadDependObject(QObject* parent = nullptr);
    QString getLastErrorString() const;
    void clearLastErrorString();

public slots:
    void onStartProcess(QStringList args = QStringList());
    void onStartProcessWithoutArgs();
    void onQuitProcess();

signals:
    void ready(int operationReturnCode);
    void showError(QString errorStr);
    void showInfo(QString infoString);

protected:
    QString _lastErrorString;
    virtual int startProcess(QStringList args) = 0;
    virtual int quitProcess() = 0;
};

#endif // THREADDEPENDOBJECT_H
