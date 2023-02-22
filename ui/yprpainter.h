#ifndef YPRPAINTER_H
#define YPRPAINTER_H

//#define USE_OPEN_GL

#include <QWidget>

/**
  * Виджет для отрисовки индикатора курса\крена\тангажа
  * Если не используется флаг USE_OPEN_GL, то для отрисовки используются нативные средства, иначе - OpenGL
  */
#ifndef USE_OPEN_GL
class YPRPainter : public QWidget
#else
class YPRPainter : public QGLWidget
#endif
{
    Q_OBJECT

public:
    /**
	  * Конструктор виджета
	  * @param parent - указатель на родителя
	  */
    YPRPainter(QWidget* parent = nullptr);
    ~YPRPainter();

    /**
	  * Задает отрисовываемый курс
	  * @param yaw - курс, градусы
	  */
    void setYaw(float _yaw);
    /**
	  * Задает отрисовываемый тангаж
	  * @param pitch - тангаж, градусы
	  */
    void setPitch(float _pitch);
    /**
	  * Задает отрисовываемый крен
	  * @param roll - крен, градусы
	  */
    void setRoll(float _roll);
    /**
	  * Задает отрисовываемый курс, крен и тангаж
	  * @param yaw - курс, градусы
	  * @param pitch - тангаж, градусы
	  * @param roll - крен, градусы
	  */
    void setYPR(float _yaw, float _pitch, float _roll);

    void setBackgroundColor(QColor color);
signals:
    void switchedYprMode();

protected:
    /**
	  * Эвент отрисовки виджета.
	  * В первый раз рисует все элементы виджета. 
	  * В последующих - меняет расположение.
	  * @param event - указатель на эфвент
	  */
    void paintEvent(QPaintEvent* event);
    /**
	  * Эвент изменения размера виджета
	  * Перерисовывает элементы виджета
	  * @param evt - указатель на эвент
	  */
    void resizeEvent(QResizeEvent* evt);

    void mouseButtonDblClickEvent();
    void mouseDoubleClickEvent(QMouseEvent* event);

#ifdef USE_OPEN_GL
    /**
	  * Функция инициализации и настройки OpenGL
	  */
    void initializeGL();
#endif

private:
    float _roll;  //!< отображаемый крен
    float _pitch; //!< отображаемый тангаж
    float _yaw;   //!< отображаемый курс

    int _minSize; //!< минимальный размер виджета (высота или ширина)
    int _maxSize; //!< максимальный размер виджета (высота или ширина
    bool _needRepaint; //!< флаг определяющий нужно ли перерисовывать все элементы
    float _intervalLength; //!< интервал между линиями тангажа

    QPixmap* _pPixmapBackground;   //!< указатель на изображение заднего фона
    QPixmap* _pPixmapEarth;        //!< указатель на изображение земли
    QPixmap* _pPixmapPitchLine;    //!< указатель на изображение линии тангажа
    QPixmap* _pPixmapRollTriangle; //!< указатель на изображение крена тангажа
    QPixmap* _pPixmapFrontground; //!< указатель на изображение переднего фона (корпуса)
    QPixmap* _pPixmapYawPointer; //!< указатель на изображение указателя курса
    QPixmap* _pPixmapYawDegrees; //!< указатель на изображение градусов курса
    QPixmap* _pPixmapCrutch;

    QColor _backgroundColor;

    void drawCrutch();
    /**
	  * Функция перерисовывания заднего фона
	  */
    void drawBackground();
    /**
	  * Функция перерисовывания земли
	  */
    void drawEarth();
    /**
	  * Функция перерисовывания линии тангажа
	  */
    void drawPitchLine();
    /**
	  * Функция перерисовывания крена тангажа
	  */
    void drawRollTriangle();
    /**
	  * Функция перерисовывания переднего фона (корпуса)
	  */
    void drawFrontground();
    /**
	  * Функция перерисовывания указателя курса
	  */
    void drawYawPointer();
    /**
	  * Функция перерисовывания градусов курса
	  */
    void drawYawDegrees();
};

#endif //YPRPAINTER_H
