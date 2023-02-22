#include "graphwindow.h"
#include "ui_graphwindow.h"

GraphWindow::GraphWindow(QWidget* parent) : QWidget(parent), ui(new Ui::GraphWindow)
{
    ui->setupUi(this);
    ui->plotWidget->xAxis->setLabel("Время");
    ui->plotWidget->yAxis->setLabel("Данные");
    ui->plotWidget->addGraph(ui->plotWidget->xAxis, ui->plotWidget->yAxis);
}

GraphWindow::~GraphWindow() { delete ui; }

void GraphWindow::setData(QVector<double>& time, QVector<double>& data)
{
    ui->plotWidget->graph(0)->setData(time, data);
    ui->plotWidget->xAxis->rescale();
    ui->plotWidget->yAxis->rescale();
    ui->plotWidget->replot();
}
