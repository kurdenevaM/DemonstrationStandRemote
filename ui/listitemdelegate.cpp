#include "listitemdelegate.h"
#include "ui_listitemdelegate.h"

ListItemDelegate::ListItemDelegate(const QString& pixPath, const QString& nameItem, QWidget* parent)
    : QWidget(parent),
      ui(new Ui::ListItemDelegate)
{
    ui->setupUi(this);
    _name = nameItem;
    _pixMap = QPixmap(pixPath);
    ui->name->setText(_name);
    ui->nameImage->setPixmap(_pixMap);
    _width = kDefaultWidth;
    _height = kDefaultHeight;
}

ListItemDelegate::~ListItemDelegate() { delete ui; }

void ListItemDelegate::setTextColor(QPalette palette) { ui->name->setPalette(palette); }

QSize ListItemDelegate::getItemSize() { return QSize(_width, _height); }
