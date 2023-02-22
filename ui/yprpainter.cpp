#include "yprpainter.h"
#include <QDebug>

//#define USE_DEBUG

#ifdef USE_DEBUG
#include <qdatetime.h>
#include <qdebug.h>

#endif

#include <qgl.h>
#include <qpainter.h>
#include <qtimer.h>
//#define NEW_COLOR

#ifndef NEW_COLOR
const QColor COLOR_SKY = QColor(0x00, 0xA0, 0xC0);
const QColor COLOR_EARTH = QColor(0x00, 0x60, 0x00);
//const QColor COLOR_CIRCLE = QColor(0x33, 0x33, 0x33);
const QColor COLOR_CIRCLE = QColor(0x1B, 0x37, 0x5B);
const QColor COLOR_YAW_ARROW = QColor(0xBB, 0x00, 0x00);
const QColor COLOR_PEN = Qt::white;
const QColor COLOR_BACK = Qt::black;
const QColor COLOR_INDICATOR = /*Qt::black;*/ QColor(0x1B, 0x37, 0x5B);
#else
const QColor COLOR_SKY = QColor(0x00, 0x80, 0x90);
const QColor COLOR_EARTH = QColor(0x00, 0x40, 0x00);
const QColor COLOR_CIRCLE = QColor(0xA0, 0xA0, 0xA0);
const QColor COLOR_YAW_ARROW = QColor(0xA0, 0x00, 0x00);
const QColor COLOR_PEN = QColor(0x00, 0x00, 0x00);
const QColor COLOR_BACK = QColor(0xFF, 0xFF, 0xFF);
const QColor COLOR_INDICATOR = QColor(0xA0, 0xA0, 0xA0);
#endif

#ifndef USE_OPEN_GL
YPRPainter::YPRPainter(QWidget* parent) : QWidget(parent)
#else
YPRPainter::YPRPainter(QWidget* parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
#endif
{

#ifdef USE_OPEN_GL
    QGLFormat qglform;
//	qglform.setSampleBuffers(true);
#endif

    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    _pPixmapBackground = new QPixmap();
    _pPixmapEarth = new QPixmap();
    _pPixmapPitchLine = new QPixmap();
    _pPixmapRollTriangle = new QPixmap();
    _pPixmapFrontground = new QPixmap();
    _pPixmapYawPointer = new QPixmap();
    _pPixmapYawDegrees = new QPixmap();
    _pPixmapCrutch = new QPixmap();

    setYPR(0, 0, 0);
    _backgroundColor = Qt::white;

    _needRepaint = true;
}

YPRPainter::~YPRPainter()
{
    delete _pPixmapCrutch;
    delete _pPixmapBackground;
    delete _pPixmapEarth;
    delete _pPixmapPitchLine;
    delete _pPixmapRollTriangle;
    delete _pPixmapFrontground;
    delete _pPixmapYawPointer;
    delete _pPixmapYawDegrees;
}

//================= Public ============================================================================================
void YPRPainter::setYPR(float yaw, float pitch, float roll)
{
    this->_yaw = yaw;
    this->_pitch = pitch;
    this->_roll = roll;

    update();
}

#ifdef USE_OPEN_GL
void YPRPainter::initializeGL()
{
    qglClearColor(Qt::white);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
}
#endif

void YPRPainter::setRoll(float roll) { setYPR(_yaw, _pitch, roll); }

void YPRPainter::setPitch(float pitch) { setYPR(_yaw, pitch, _roll); }

void YPRPainter::setYaw(float yaw) { setYPR(yaw, _pitch, _roll); }

void YPRPainter::setBackgroundColor(QColor color)
{
    _backgroundColor = color;
    update();
}

//================ Events =============================================================================================
void YPRPainter::resizeEvent(QResizeEvent* evt)
{
    Q_UNUSED(evt);
    _needRepaint = true;
}

void YPRPainter::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
#ifdef USE_DEBUG
    QDateTime start = QDateTime::currentDateTime();
#endif

    _minSize = qMin(size().width(), size().height());
    _maxSize = qMax(size().width(), size().height());
    _intervalLength = _minSize / 16;

    QPainter painter;

    if (painter.begin(this))
    {
        if (_needRepaint)
        {
            _needRepaint = false;

            drawBackground();
            drawEarth();
            drawPitchLine();
            drawRollTriangle();
            drawFrontground();
            drawYawPointer();
            drawYawDegrees();
            drawCrutch();
        }

        //background (sky)
        painter.drawPixmap(0, 0, *_pPixmapBackground);

        //move to center
        painter.translate(size().width() / 2., size().height() / 2.);

        //earth
        painter.save();
        painter.rotate(_roll);
        painter.translate(0, _pitch * _intervalLength / 5.);
        painter.drawPixmap(-_pPixmapEarth->width() / 2., 0, *_pPixmapEarth);
        painter.restore();

        //pitch line and roll triangle
        painter.save();
        painter.rotate(_roll);
        painter.drawPixmap(-_minSize / 2., -_pPixmapPitchLine->height() / 2. + _pitch * _intervalLength / 5.,
                           *_pPixmapPitchLine);
        painter.drawPixmap(-_pPixmapRollTriangle->width() / 2., -_minSize / 2. + _pPixmapYawPointer->height() * 0.9,
                           *_pPixmapRollTriangle);
        painter.restore();

        //yaw degrees
        painter.drawPixmap(-_minSize / 2., -_minSize / 2., *_pPixmapYawDegrees);

        //yaw pointer
        painter.save();
        painter.rotate(_yaw);
        painter.drawPixmap(-_pPixmapYawPointer->width() / 2., -_minSize / 2. + _pPixmapYawPointer->height() * 0.9,
                           *_pPixmapYawPointer);
        painter.restore();

        //frontground
        painter.drawPixmap(-size().width() / 2. - 1, -size().height() / 2. - 1, *_pPixmapFrontground);

        // painter.drawPixmap(0, 0, *pixmapCrutch);
        painter.end();
    }

#ifdef USE_DEBUG
    qDebug() << start.msecsTo(QDateTime::currentDateTime());
#endif
}

void YPRPainter::mouseDoubleClickEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    emit switchedYprMode();
}
//=====================================================================================================================
//=
//=
//=
//=
//=
//=
//=========== Private =================================================================================================
void YPRPainter::drawBackground()
{
    if (_pPixmapBackground)
        delete _pPixmapBackground;
    _pPixmapBackground = new QPixmap(_maxSize, _maxSize);

    _pPixmapBackground->fill(COLOR_SKY);
}

void YPRPainter::drawCrutch()
{
    if (_pPixmapCrutch)
        delete _pPixmapCrutch;
    _pPixmapCrutch = new QPixmap(_maxSize, _maxSize);

    QPainter painter;
    if (painter.begin(_pPixmapFrontground))
    {
        QPainterPath pathHole;

        pathHole.addRect(-_minSize / 10., -_minSize / 10., _maxSize + 2 * _minSize / 10.,
                         _maxSize + 2 * _minSize / 10.);

        if (size().height() > _minSize)
            pathHole.addEllipse(0, (_maxSize - _minSize) / 2., _minSize, _minSize);
        else
            pathHole.addEllipse((_maxSize - _minSize) / 2., 0, _minSize, _minSize);

        // painter.setPen(QPen(COLOR_PEN, minSize / 200));
        painter.setBrush(_backgroundColor);
        painter.drawPath(pathHole);
        painter.end();
    }
}

void YPRPainter::drawEarth()
{
    if (_pPixmapEarth)
        delete _pPixmapEarth;
    _pPixmapEarth = new QPixmap(_minSize, _minSize * 2);

    _pPixmapEarth->fill(COLOR_EARTH);
}

void YPRPainter::drawPitchLine()
{
    if (_pPixmapPitchLine)
        delete _pPixmapPitchLine;

    _pPixmapPitchLine = new QPixmap(_minSize, 50 * _intervalLength);
    _pPixmapPitchLine->fill(Qt::transparent);

    QPainter painter;
    if (painter.begin(_pPixmapPitchLine))
    {
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(COLOR_PEN, _minSize / 200));
        painter.drawLine(0, _pPixmapPitchLine->height() / 2., _minSize, _pPixmapPitchLine->height() / 2.);

        QFont painterFont = painter.font();
        painterFont.setPixelSize(_minSize / 20);
        painter.setFont(painterFont);
        painter.translate(_pPixmapPitchLine->width() / 2., _pPixmapPitchLine->height() / 2.);

        for (int i = -180; i <= 180; i += 5)
        {
            if (i != 0)
            {
                if (i % 10 == 0)
                {
                    QRect leftNum(-_minSize * 5. / 32. - _minSize / 10., -i * _intervalLength / 5. - _minSize / 40.,
                                  _minSize / 10., _minSize / 20.);

                    QRect rightNum(_minSize * 5. / 32., -i * _intervalLength / 5. - _minSize / 40., _minSize / 10.,
                                   _minSize / 20.);

                    painter.drawLine(-_minSize / 8., -i * _intervalLength / 5., _minSize / 8.,
                                     -i * _intervalLength / 5.);

                    painter.drawText(rightNum, Qt::AlignLeft, QString::number(i, 'f', 0));
                    painter.drawText(leftNum, Qt::AlignRight, QString::number(i, 'f', 0));
                }
                else
                {
                    painter.drawLine(-_minSize / 16., -i * _intervalLength / 5., _minSize / 16.,
                                     -i * _intervalLength / 5.);
                }
            }
        }

        painter.end();
    }
}

void YPRPainter::drawRollTriangle()
{
    if (_pPixmapRollTriangle)
        delete _pPixmapRollTriangle;
    _pPixmapRollTriangle = new QPixmap(_minSize / 10, _minSize * 2 / 10);

    _pPixmapRollTriangle->fill(Qt::transparent);

    QPainter painter;
    if (painter.begin(_pPixmapRollTriangle))
    {
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(COLOR_PEN, _minSize / 200));

        painter.drawLine(0, _pPixmapRollTriangle->height() - _minSize / 200, _pPixmapRollTriangle->width() / 2., 0);
        painter.drawLine(_pPixmapRollTriangle->width() / 2., 0, _pPixmapRollTriangle->width(),
                         _pPixmapRollTriangle->height() - _minSize / 200);
        painter.drawLine(0, _pPixmapRollTriangle->height() - _minSize / 200, _pPixmapRollTriangle->width(),
                         _pPixmapRollTriangle->height() - _minSize / 200);

        painter.end();
    }
}

void YPRPainter::drawFrontground()
{
    if (_pPixmapFrontground)
        delete _pPixmapFrontground;
    _pPixmapFrontground = new QPixmap(_maxSize + 2, _maxSize + 2);

    _pPixmapFrontground->fill(Qt::transparent);

    QPainter painter;
    if (painter.begin(_pPixmapFrontground))
    {
        painter.setRenderHint(QPainter::Antialiasing);

        QPainterPath pathHole;

        pathHole.addRect(-_minSize / 10., -_minSize / 10., _maxSize + 2 * _minSize / 10.,
                         _maxSize + 2 * _minSize / 10.);

        if (size().height() > _minSize)
            pathHole.addEllipse(0, (_maxSize - _minSize) / 2., _minSize, _minSize);
        else
            pathHole.addEllipse((_maxSize - _minSize) / 2., 0, _minSize, _minSize);

        QPainterPath pathIndicator;
        QPolygonF leftIndicator;
        QPolygonF rightIndicator;

        pathIndicator.addRect(size().width() / 2. - _minSize / 80., size().height() / 2. - _minSize / 80.,
                              _minSize / 40., _minSize / 40.);

        leftIndicator << QPointF(size().width() / 2. - _minSize / 3., size().height() / 2. - _minSize / 80.)
                      << QPointF(size().width() / 2. - _minSize / 3. + _minSize / 10.,
                                 size().height() / 2. - _minSize / 80.)
                      << QPointF(size().width() / 2. - _minSize / 3. + _minSize / 10.,
                                 size().height() / 2. + _minSize / 30.)
                      << QPointF(size().width() / 2. - _minSize / 3. + _minSize / 10. - _minSize / 40.,
                                 size().height() / 2. + _minSize / 30.)
                      << QPointF(size().width() / 2. - _minSize / 3. + _minSize / 10. - _minSize / 40.,
                                 size().height() / 2. + _minSize / 80.)
                      << QPointF(size().width() / 2. - _minSize / 3., size().height() / 2. + _minSize / 80.)
                      << QPointF(size().width() / 2. - _minSize / 3., size().height() / 2. - _minSize / 80.);

        rightIndicator << QPointF(size().width() / 2. + _minSize / 3., size().height() / 2. - _minSize / 80.)
                       << QPointF(size().width() / 2. + _minSize / 3. - _minSize / 10.,
                                  size().height() / 2. - _minSize / 80.)
                       << QPointF(size().width() / 2. + _minSize / 3. - _minSize / 10.,
                                  size().height() / 2. + _minSize / 30.)
                       << QPointF(size().width() / 2. + _minSize / 3. - _minSize / 10. + _minSize / 40.,
                                  size().height() / 2. + _minSize / 30.)
                       << QPointF(size().width() / 2. + _minSize / 3. - _minSize / 10. + _minSize / 40.,
                                  size().height() / 2. + _minSize / 80.)
                       << QPointF(size().width() / 2. + _minSize / 3., size().height() / 2. + _minSize / 80.)
                       << QPointF(size().width() / 2. + _minSize / 3., size().height() / 2. - _minSize / 80.);

        pathIndicator.addPolygon(leftIndicator);
        pathIndicator.addPolygon(rightIndicator);

        painter.setPen(QPen(COLOR_PEN, _minSize / 200));
        painter.setBrush(COLOR_BACK);
        painter.drawPath(pathHole);

        painter.setBrush(COLOR_INDICATOR);
        painter.drawPath(pathIndicator);

        painter.end();
    }
}

void YPRPainter::drawYawPointer()
{
    if (_pPixmapYawPointer)
        delete _pPixmapYawPointer;
    _pPixmapYawPointer = new QPixmap(_minSize / 15., _minSize / 15.);

    _pPixmapYawPointer->fill(Qt::transparent);

    QPainter painter;
    if (painter.begin(_pPixmapYawPointer))
    {
        painter.setRenderHint(QPainter::Antialiasing);

        qreal penWidth = _pPixmapYawPointer->width() / 4;
        painter.setPen(QPen(COLOR_YAW_ARROW, penWidth));
        painter.drawLine(_pPixmapYawPointer->width() / 2., penWidth * 2., _pPixmapYawPointer->width() / 2.,
                         _pPixmapYawPointer->height() - penWidth);
        painter.drawLine(penWidth, _pPixmapYawPointer->height() / 2., _pPixmapYawPointer->width() / 2., penWidth);
        painter.drawLine(_pPixmapYawPointer->width() - penWidth, _pPixmapYawPointer->height() / 2.,
                         _pPixmapYawPointer->width() / 2., penWidth);

        painter.end();
    }
}

void YPRPainter::drawYawDegrees()
{
    if (_pPixmapYawDegrees)
        delete _pPixmapYawDegrees;
    _pPixmapYawDegrees = new QPixmap(_minSize, _minSize);

    _pPixmapYawDegrees->fill(Qt::transparent);

    QPainter painter;

    if (painter.begin(_pPixmapYawDegrees))
    {
        painter.setPen(QPen(COLOR_PEN, _minSize / 200));
        painter.setRenderHint(QPainter::Antialiasing);

        QPainterPath pathHole;

        float circleWidth = _minSize / 16.;
        pathHole.addRect(0, 0, _pPixmapYawDegrees->width(), _pPixmapYawDegrees->height());
        pathHole.addEllipse(circleWidth, circleWidth, _minSize - circleWidth * 2, _minSize - circleWidth * 2);

        painter.save();
        painter.setPen(QPen(COLOR_PEN, _minSize / 200));
        painter.setBrush(COLOR_CIRCLE);
        painter.drawPath(pathHole);
        painter.restore();

        painter.translate(_minSize / 2., _minSize / 2.);

        QFont painterFont = painter.font();
        painterFont.setPixelSize(_minSize / 33.);

        painter.setFont(painterFont);

        for (int i = -175; i <= 180; i += 5)
        {
            painter.save();
            painter.rotate(i);
            if (i % 10 == 0)
            {
                painter.drawLine(0, _minSize / 2. - _minSize / 28., 0, _minSize * 38 / 80. - _minSize / 29);
                painter.drawText(-_minSize / 20., -_minSize / 2., _minSize / 10., _minSize / 33., Qt::AlignCenter,
                                 QString::number(i, 'f', 0));
            }
            else
            {
                painter.drawLine(0, _minSize / 2. - _minSize / 22., 0, _minSize * 38 / 80. - _minSize / 29);
            }
            painter.restore();
        }

        painter.end();
    }
}
//=====================================================================================================================
