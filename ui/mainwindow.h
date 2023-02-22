#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QListWidgetItem;
class ListItemDelegate;
class DisplayForm;
class ModelingForm;
class SettingsForm;
class GraphsForm;

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    DisplayForm* getDisplayForm();
    ModelingForm* getModellingForm();
    SettingsForm* getSettingsForm();
    GraphsForm* getGraphsForm();

public slots:
    void onPortStateChanged(bool isOpen);
    void onShowTransmitMainPort(bool isExist);
    void onShowReceiveMainPort(bool isExist);
    void onShowFileSize(quint64 size);
    void onShowFileWritting(bool isWritting);
    void onShowError(QString errorStr);
    void onShowInfo(QString infoStr);
    void onShowFrequency(int freq);

private slots:
    void onOpenLogDir();
    void onDownNavigation();
    void onUpNavigation();

private:
    Ui::MainWindow* ui;
    bool _isMainPortOpen;
    QPixmap* _pGreenIndicator;
    QPixmap* _pRedIndicator;
    QPixmap* _pGrayIndicator;

    QListWidgetItem* _pItemPortSettings;
    QListWidgetItem* _pItemReceiving;
    QListWidgetItem* _pItemTransmit;
    QListWidgetItem* _pItemGraphs;
    QListWidgetItem* _pItemAboutProgram;

    ListItemDelegate* _pWidgetPortSettings;
    ListItemDelegate* _pWidgetReceiving;
    ListItemDelegate* _pWidgetTransmit;
    ListItemDelegate* _pWidgetGraphs;
    ListItemDelegate* _pWidgetAboutProgram;

    void initNavigationBar();
    void initIndicatorsPixmaps();
    void initShortcuts();
    void setupConnection();
    void saveSettings();
    void loadSettings();

    void closeEvent(QCloseEvent* event);
};
#endif // MAINWINDOW_H
