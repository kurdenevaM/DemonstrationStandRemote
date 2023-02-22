#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QWidget>

namespace Ui
{
class GraphWindow;
}

class GraphWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GraphWindow(QWidget* parent = nullptr);
    ~GraphWindow();
    void setData(QVector<double>& time, QVector<double>& data);

private:
    Ui::GraphWindow* ui;
};

#endif // GRAPHWINDOW_H
