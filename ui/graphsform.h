#ifndef GRAPHSFORM_H
#define GRAPHSFORM_H

#include "protocols/mainprotocol.h"
#include <QMap>
#include <QVector>
#include <QWidget>

class GraphWindow;

namespace Ui
{
class GraphsForm;
}

class GraphsForm : public QWidget
{
    Q_OBJECT

public:
    explicit GraphsForm(QWidget* parent = nullptr);
    ~GraphsForm();

public slots:
    void onReceiveSensorsData(navigationPacket data);
    void onShowInSeparateWindow();
    void onUpdateGraphs();

private:
    Ui::GraphsForm* ui;

    QVector<double> _axData;
    QVector<double> _ayData;
    QVector<double> _azData;
    QVector<double> _gxData;
    QVector<double> _gyData;
    QVector<double> _gzData;
    QVector<double> _mxData;
    QVector<double> _myData;
    QVector<double> _mzData;
    QVector<double> _tData;

    QVector<double> _timePoints;
    QList<QVector<double>*> _graphsList;
    QList<GraphWindow*> _graphsWindowList;

    QTimer* _pUpdateGraphsTimer;

    quint64 _timeCounter;
};

#endif // GRAPHSFORM_H
