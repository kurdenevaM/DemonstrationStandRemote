#include "graphsform.h"
#include "graphwindow.h"
#include "ui_graphsform.h"
#include <QTimer>
const int kMaxGraphPoints = 1000;
const int kUpdatePeriod = 100;

GraphsForm::GraphsForm(QWidget* parent) : QWidget(parent), ui(new Ui::GraphsForm)
{
    ui->setupUi(this);
    _timeCounter = 0;

    ui->plotWidget->xAxis->setLabel("Время");
    ui->plotWidget->yAxis->setLabel("Данные");
    ui->plotWidget->addGraph(ui->plotWidget->xAxis, ui->plotWidget->yAxis);
    _graphsList << &_axData << &_ayData << &_azData << &_gxData << &_gyData << &_gzData << &_mxData << &_myData
                << &_mzData << &_tData;
    foreach (QVector<double>* dummy, _graphsList)
        _graphsWindowList.append(new GraphWindow());

    for (int i = 0; i < _graphsWindowList.count(); i++)
        _graphsWindowList.at(i)->setWindowTitle(ui->comboBox->itemText(i));

    _pUpdateGraphsTimer = new QTimer();
    _pUpdateGraphsTimer->setInterval(kUpdatePeriod);
    connect(_pUpdateGraphsTimer, &QTimer::timeout, this, &GraphsForm::onUpdateGraphs);
    _pUpdateGraphsTimer->start();

    connect(ui->pushButton, &QPushButton::clicked, this, &GraphsForm::onShowInSeparateWindow);
}

GraphsForm::~GraphsForm() { delete ui; }

void GraphsForm::onReceiveSensorsData(navigationPacket data)
{
    _timeCounter++;
    _timePoints.append(_timeCounter);
    _axData.append(data.ax);
    _ayData.append(data.ay);
    _azData.append(data.az);
    _gxData.append(data.gx);
    _gyData.append(data.gy);
    _gzData.append(data.gz);
    _mxData.append(data.mx);
    _myData.append(data.my);
    _mzData.append(data.mz);
    _tData.append(data.t);

    if (_timePoints.size() > kMaxGraphPoints)
        _timePoints.takeFirst();
    if (_axData.size() > kMaxGraphPoints)
        _axData.takeFirst();
    if (_ayData.size() > kMaxGraphPoints)
        _ayData.takeFirst();
    if (_azData.size() > kMaxGraphPoints)
        _azData.takeFirst();
    if (_gxData.size() > kMaxGraphPoints)
        _gxData.takeFirst();
    if (_gyData.size() > kMaxGraphPoints)
        _gyData.takeFirst();
    if (_gzData.size() > kMaxGraphPoints)
        _gzData.takeFirst();
    if (_mxData.size() > kMaxGraphPoints)
        _mxData.takeFirst();
    if (_myData.size() > kMaxGraphPoints)
        _myData.takeFirst();
    if (_mzData.size() > kMaxGraphPoints)
        _mzData.takeFirst();
    if (_tData.size() > kMaxGraphPoints)
        _tData.takeFirst();
}

void GraphsForm::onShowInSeparateWindow() { _graphsWindowList.at(ui->comboBox->currentIndex())->show(); }

void GraphsForm::onUpdateGraphs()
{
    if (_timePoints.isEmpty())
        return;

    ui->plotWidget->graph(0)->setData(_timePoints, *_graphsList.at(ui->comboBox->currentIndex()));
    ui->plotWidget->xAxis->rescale();
    ui->plotWidget->yAxis->rescale();
    ui->plotWidget->replot();
    for (int i = 0; i < ui->comboBox->count(); i++)
        if (_graphsWindowList.at(i)->isVisible())
            _graphsWindowList.at(i)->setData(_timePoints, *_graphsList.at(i));
}
