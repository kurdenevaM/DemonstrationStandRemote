#ifndef DISPLAYFORM_H
#define DISPLAYFORM_H

#include "defines/navigationstructs.h"
#include "protocols/mainprotocol.h"
#include <QWidget>

namespace Ui
{
class DisplayForm;
}

class DisplayForm : public QWidget
{
    Q_OBJECT

public:
    explicit DisplayForm(QWidget* parent = nullptr);
    ~DisplayForm();

public slots:
    void onCrcError();
    void onUpdateForm(navigationPacket pack);
    void onShowOrientation(orientation value);

private slots:

    void onUpdateDataActuality();

private:
    Ui::DisplayForm* ui;
    bool _isWaitFirstPack;
    bool _lastActualityState;
    bool _thereIsPack;
    int _errorCounter;

    QTimer* _pUpdateActualityTimer;

    QPixmap* _pGreenIndicator;
    QPixmap* _pRedIndicator;
    QPixmap* _pGrayIndicator;

    void initObjects();
    void initConnections();
    void showSensorsData(const navigationPacket& data);
};

#endif // DISPLAYFORM_H
