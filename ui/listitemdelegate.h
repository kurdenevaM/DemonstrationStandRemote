#ifndef LISTITEMDELEGATE_H
#define LISTITEMDELEGATE_H

#include <QWidget>

namespace Ui
{
class ListItemDelegate;
}

const int kDefaultHeight = 65;
const int kDefaultWidth = 0;
/**
 * @brief The ListItemDelegate class - класс, формирующий виджеты для навигационной панели. Иконка + надпись
 */
class ListItemDelegate : public QWidget
{
    Q_OBJECT

public:
    explicit ListItemDelegate(const QString& pixPath, const QString& nameItem, QWidget* parent = nullptr);
    ~ListItemDelegate();
    /**
     * @brief setTextColor - установка палитры виджета.
     * Используется, для преобразования виджета при смене темы приложения
     * @param palette - необходимая палитра
     */
    void setTextColor(QPalette palette);
    /**
     * @brief getItemSize - получение размеров виджета
     * @return - объект QSize, содержащий информацию о размерах виджета
     */
    QSize getItemSize();

private:
    Ui::ListItemDelegate* ui;

    QString _name;
    QPixmap _pixMap;
    int _width;
    int _height;
};

#endif // LISTITEMDELEGATE_H
