#ifndef ARCHITECT_H
#define ARCHITECT_H

#include <QObject>
#include <QVariant>
class QApplication;
class MainWindow;
class AbstractAdapter;
class AdapterFactory;
class OpenChannelHelper;
class FileWriterHelper;
class MainProtocolHelper;
class NavigationManager;

class Architect : public QObject
{
    Q_OBJECT
public:
    explicit Architect(QObject* parent = nullptr, int argc = 0, char* argv[] = nullptr);
    int startApplication();

signals:
    void openPort(AbstractAdapter* adapter, const QVariant settings, const QVariant standType);
    void showError(QString error);

    void portStateChanged(bool isOpen);

private slots:
    void onCloseApplication();

    void onConnectToMainChannel(QVariant type, QVariant settings);
    void onDisconnectToMainChannel();

    void onSuccessOpenChannel(QVariant standType);
    void onNotSuccessOpenChannel(QString errorStr, QVariant standType);

private:
    QApplication* _pApplication;
    MainWindow* _pMainWindow;
    MainProtocolHelper* _pMainProtocolHelper;
    FileWriterHelper* _pFileHelper;
    AdapterFactory* _pFactory;
    AbstractAdapter* _pChannelAdapter;
    OpenChannelHelper* _pOpenHelper;
    NavigationManager* _pNaviManager;
    QThread* _pOpenHelperThread;
    QThread* _pFileWriteThread;
    QThread* _pMainProtocolThread;
    QThread* _pNaviManagerThread;
    QList<QMetaObject::Connection> _connections;

    void initialisation();
    void initObjects();
    void initConnections();

    void registrateApplication();
    void registerMetaTypes();
};

#endif // ARCHITECT_H
