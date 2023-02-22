#ifndef MODELINGFORM_H
#define MODELINGFORM_H

#include "protocols/mainprotocol.h"
#include <QWidget>

namespace Ui
{
class ModelingForm;
}

class ModelingForm : public QWidget
{
    Q_OBJECT

public:
    explicit ModelingForm(QWidget* parent = nullptr);
    void loadSettings();
    ~ModelingForm();

signals:
    void sendedLedsMode(QByteArray pack);

private slots:
    void onLedsModeInfoChanged();

private:
    Ui::ModelingForm* ui;

    void sendLedsMode();

    void setupConnections();
    void saveSettings();
};

#endif // MODELINGFORM_H
