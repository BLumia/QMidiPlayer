/******************************************************************************

   Skulpture - Classical Three-Dimensional Artwork for Qt 4

   Copyright (c) 2007-2009 Christoph Feck <christoph@maxiom.de>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*****************************************************************************/
#include <cmath>
#include <functional>
#include <QStyleOptionSlider>
#include <QPixmapCache>
#include <QPainter>
#include <QPointF>
#include <QRectF>

#include "qdialskulpturestyle.hpp"

#ifndef M_PI
#define M_PI 3.141592653589793f
#endif

static const bool UsePixmapCache = true;

static void
paintIndicatorCached(QPainter *painter, const QStyleOption *option, std::function<void(QPainter *, const QStyleOption *)> paintFunc, bool useCache, const QString &pixmapName)
{
    QRect rect = option->rect;
    QPixmap internalPixmapCache;
    QImage imageCache;
    QPainter *p = painter;
    int txType = painter->deviceTransform().type() | painter->worldTransform().type();
    bool doPixmapCache = useCache && (!option->rect.isEmpty())
        && ((txType <= QTransform::TxTranslate) || (painter->deviceTransform().type() == QTransform::TxScale));
    if (doPixmapCache && QPixmapCache::find(pixmapName, &internalPixmapCache))
    {
        painter->drawPixmap(option->rect.topLeft(), internalPixmapCache);
    }
    else
    {
        if (doPixmapCache)
        {
            rect.setRect(0, 0, option->rect.width(), option->rect.height());
            qreal pixelRatio = painter->device()->devicePixelRatioF();
            imageCache = QImage(option->rect.size() * pixelRatio, QImage::Format_ARGB32_Premultiplied);
            imageCache.setDevicePixelRatio(pixelRatio);
            imageCache.fill(0);
            p = new QPainter(&imageCache);
        }
        paintFunc(p, option);
        if (doPixmapCache)
        {
            p->end();
            delete p;
            internalPixmapCache = QPixmap::fromImage(imageCache);
            painter->drawPixmap(option->rect.topLeft(), internalPixmapCache);
            QPixmapCache::insert(pixmapName, internalPixmapCache);
        }
    }
}

void
paintDialBase(QPainter *painter, const QStyleOption *option)
{
//	painter->fillRect(option->rect, Qt::red);
//  painter->save();
//  painter->setRenderHint(QPainter::Antialiasing, true);
    int d = qMin(option->rect.width(), option->rect.height());
    /*  if (d > 20 && option->notchTarget > 0) {
            d += -1;
        }
    */
    QRectF r((option->rect.width() - d) / 2.0, (option->rect.height() - d) / 2.0, d, d);
    const qreal angle = option->direction == Qt::LeftToRight ? 135.0 : 45.0;
//  const qreal angle = 90;

    painter->setPen(Qt::NoPen);
    painter->setRenderHint(QPainter::Antialiasing);
    QColor border_color = option->palette.color(QPalette::Window);
#if 0
    {
        QRadialGradient depth_gradient(r.center(), d / 2);
//      depth_gradient.setColorAt(0.0, QColor(0, 0, 0, 255));
        depth_gradient.setColorAt(0.5, QColor(0, 0, 0, 255));
        depth_gradient.setColorAt(1.0, QColor(0, 0, 0, 0));
        painter->setBrush(depth_gradient);
        painter->drawEllipse(r);
    }
#endif
#if 1
    if (option->state & QStyle::State_HasFocus && option->state & QStyle::State_KeyboardFocusChange)
    {
        painter->setBrush(option->palette.color(QPalette::Highlight).darker(180));
        r.adjust(1, 1, -1, -1);
        painter->drawEllipse(r);
        painter->setBrush(border_color);
        r.adjust(1, 1, -1, -1);
        painter->drawEllipse(r);
        r.adjust(1, 1, -1, -1);
    }
    else
    {
        painter->setBrush(border_color);
        r.adjust(1, 1, -1, -1);
        painter->drawEllipse(r);
        r.adjust(1, 1, -1, -1);
        QConicalGradient border_gradient(r.center(), angle);
        if (!(option->state & QStyle::State_Enabled))
        {
            border_color = border_color.lighter(120);
        }
        border_gradient.setColorAt(0.0, border_color.darker(180));
        border_gradient.setColorAt(0.3, border_color.darker(130));
        border_gradient.setColorAt(0.5, border_color.darker(170));
        border_gradient.setColorAt(0.7, border_color.darker(130));
        border_gradient.setColorAt(1.0, border_color.darker(180));
        painter->setBrush(border_gradient);
//      painter->setBrush(Qt::blue);
        painter->drawEllipse(r);
        r.adjust(1, 1, -1, -1);
    }
    d -= 6;

    QColor dial_color;
    if (option->state & QStyle::State_Enabled)
    {
        dial_color = option->palette.color(QPalette::Button).lighter(101);
        if (option->state & QStyle::State_MouseOver)
        {
            dial_color = dial_color.lighter(103);
        }
    }
    else
    {
        dial_color = option->palette.color(QPalette::Window);
    }
    qreal t = option->state & QStyle::State_Enabled ? 2.0 : 1.5;
    // ###: work around Qt 4.3.0 bug? (this works for 4.3.1)
    QConicalGradient border_gradient(r.center(), angle);
    border_gradient.setColorAt(0.0, dial_color.lighter(120));
    border_gradient.setColorAt(0.2, dial_color);
    border_gradient.setColorAt(0.5, dial_color.darker(130));
    border_gradient.setColorAt(0.8, dial_color);
    border_gradient.setColorAt(1.0, dial_color.lighter(120));
    painter->setPen(QPen(border_gradient, t));
#if 0
    QLinearGradient dial_gradient(r.topLeft(), r.bottomLeft());
    dial_gradient.setColorAt(0.0, dial_color.darker(105));
    dial_gradient.setColorAt(0.5, dial_color.lighter(102));
    dial_gradient.setColorAt(1.0, dial_color.lighter(105));
#elif 1
    QLinearGradient dial_gradient(option->direction == Qt::LeftToRight ? r.topLeft() : r.topRight(), option->direction == Qt::LeftToRight ? r.bottomRight() : r.bottomLeft());
//  QLinearGradient dial_gradient(r.topLeft(), r.bottomLeft());
    if (option->state & QStyle::State_Enabled)
    {
#if 1
        dial_gradient.setColorAt(0.0, dial_color.darker(106));
        dial_gradient.setColorAt(1.0, dial_color.lighter(104));
#else
        dial_gradient.setColorAt(0.0, dial_color.lighter(101));
        dial_gradient.setColorAt(0.5, dial_color.darker(103));
        dial_gradient.setColorAt(1.0, dial_color.lighter(104));
#endif
    }
    else
    {
        dial_gradient.setColorAt(0.0, dial_color);
        dial_gradient.setColorAt(1.0, dial_color);
    }
#elif 0
    QConicalGradient dial_gradient(r.center(), angle);
    dial_gradient.setColorAt(0.0, dial_color.lighter(102));
    dial_gradient.setColorAt(0.5, dial_color.darker(103));
    dial_gradient.setColorAt(1.0, dial_color.lighter(102));
#else
    QBrush dial_gradient(dial_color);
#endif
    painter->setBrush(dial_gradient);
    t = t / 2;
    painter->drawEllipse(r.adjusted(t, t, -t, -t));

//  painter->setPen(Qt::NoPen);
//  painter->setBrush(dial_color);
//  painter->drawEllipse(r.adjusted(d / 4, d / 4, - d / 4, - d / 4));

#if 0
    QLinearGradient border2_gradient(r.topLeft(), r.bottomRight());
    border2_gradient.setColorAt(1.0, dial_color.darker(425));
    border2_gradient.setColorAt(0.9, dial_color);
    border2_gradient.setColorAt(0.0, dial_color.darker(400));
    painter->setPen(QPen(border2_gradient, 1.3));
    painter->setBrush(Qt::NoBrush);
    painter->drawEllipse(r.adjusted(0.3, 0.3, -0.3, -0.3));
#endif
//  painter->restore();
#endif
}

void
paintCachedDialBase(QPainter *painter, const QStyleOptionSlider *option)
{
    bool useCache = UsePixmapCache;
    QString pixmapName;
    QRect r = option->rect;
    int d = qMin(r.width(), r.height());

    if (/* option->state & (QStyle::State_HasFocus | QStyle::State_MouseOver) ||*/ d > 128)
    {
        useCache = false;
    }
    if (useCache)
    {
        uint state = uint(option->state) & (QStyle::State_Enabled | QStyle::State_On | QStyle::State_MouseOver | QStyle::State_KeyboardFocusChange | QStyle::State_HasFocus);
        if (!(state & QStyle::State_Enabled))
        {
            state &= ~(QStyle::State_MouseOver | QStyle::State_HasFocus | QStyle::State_KeyboardFocusChange);
        }
        //  state &= ~(QStyle::State_HasFocus);
        pixmapName = QString("scp-qdb-%1-%2-%3-%4")
            .arg(state, 0, 16)
            .arg(option->direction, 0, 16)
            .arg(option->palette.cacheKey(), 0, 16)
            .arg(d, 0, 16);
    }
    paintIndicatorCached(painter, option, paintDialBase, useCache, pixmapName);
}

void
paintIndicatorDial(QPainter *painter, const QStyleOptionSlider *option)
{
    int d = qMin(option->rect.width(), option->rect.height());
    QRect rect(option->rect.center() - QPoint((d - 1) / 2, (d - 1) / 2), QSize(d, d));
    QStyleOptionSlider opt;
    opt.QStyleOption::operator=(*option);
    opt.rect = rect;
    paintCachedDialBase(painter, &opt);
}

QColor
shaded_color(const QColor &color, int shade)
{
#if 1
    const qreal contrast = 1.0;
    int r, g, b;
    color.getRgb(&r, &g, &b);
    int gray = qGray(r, g, b);
    gray = qMax(r, qMax(g, b));
    gray = (r + b + g + 3 * gray) / 6;
    if (shade < 0)
    {
        qreal k = 220.0 / 255.0 * shade;
        k *= contrast;
        int a = 255;
        if (gray > 0)
        {
            a = int(k * 255 / (0 - gray));
            if (a < 0) a = 0;
            if (a > 255) a = 255;
        }
        return QColor(0, 0, 0, a);
    }
    else
    {
        qreal k = (255 - 220.0) / (255.0) * shade;
        k *= contrast;
        int a = 255;
        if (gray < 255)
        {
            a = int(k * 255 / (255 - gray));
            if (a < 0) a = 0;
            if (a > 255) a = 255;
        }
        return QColor(255, 255, 255, a);
    }
#else
    if (shade < 0)
    {
        return QColor(0, 0, 0, -shade);
    }
    else
    {
        return QColor(255, 255, 255, shade);
    }
#endif
}

static void
paintGrip(QPainter *painter, const QStyleOption *option)
{
    //painter->fillRect(option->rect, Qt::red);
    int d = qMin(option->rect.width(), option->rect.height());
    // good values are 3 (very small), 4 (small), 5 (good), 7 (large), 9 (huge)
    // int d = 5;
    QRectF rect(QRectF(option->rect).center() - QPointF(d / 2.0, d / 2.0), QSizeF(d, d));
    const qreal angle = option->direction == Qt::LeftToRight ? 135.0 : 45.0;
//  const qreal angle = 90;
    QColor color;
    qreal opacity = 0.9;

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    if (option->state & QStyle::State_Enabled)
    {
        if (option->state & QStyle::State_Sunken)
        {
            color = option->palette.color(QPalette::Highlight).darker(110);
        }
        else
        {
            color = option->palette.color(QPalette::Button);
        }
    }
    else
    {
        color = option->palette.color(QPalette::Button);
        opacity = 0.5;
    }

    QConicalGradient gradient1(rect.center(), angle);
    gradient1.setColorAt(0.0, shaded_color(color, -110));
    gradient1.setColorAt(0.25, shaded_color(color, -30));
    gradient1.setColorAt(0.5, shaded_color(color, 180));
    gradient1.setColorAt(0.75, shaded_color(color, -30));
    gradient1.setColorAt(1.0, shaded_color(color, -110));
    painter->setBrush(color);
    painter->drawEllipse(rect);
    painter->setBrush(gradient1);
#if (QT_VERSION >= QT_VERSION_CHECK(4, 2, 0))
    // ### merge opacity into color
    painter->setOpacity(opacity);
#endif
    painter->drawEllipse(rect);
#if (QT_VERSION >= QT_VERSION_CHECK(4, 2, 0))
    painter->setOpacity(1.0);
    if (d > 2)
    {
        QConicalGradient gradient2(rect.center(), angle);
        gradient2.setColorAt(0.0, shaded_color(color, -40));
        gradient2.setColorAt(0.25, shaded_color(color, 0));
        gradient2.setColorAt(0.5, shaded_color(color, 210));
        gradient2.setColorAt(0.75, shaded_color(color, 0));
        gradient2.setColorAt(1.0, shaded_color(color, -40));
        rect.adjust(1, 1, -1, -1);
        painter->setBrush(color);
        painter->drawEllipse(rect);
        painter->setBrush(gradient2);
        painter->setOpacity(opacity);
        painter->drawEllipse(rect);
        painter->setOpacity(1.0);
        if (d > 8)
        {
            QConicalGradient gradient3(rect.center(), angle);
            gradient3.setColorAt(0.0, shaded_color(color, -10));
            gradient3.setColorAt(0.25, shaded_color(color, 0));
            gradient3.setColorAt(0.5, shaded_color(color, 180));
            gradient3.setColorAt(0.75, shaded_color(color, 0));
            gradient3.setColorAt(1.0, shaded_color(color, -10));
            rect.adjust(2, 2, -2, -2);
            painter->setBrush(color);
            painter->drawEllipse(rect);
            painter->setBrush(gradient3);
            painter->setOpacity(opacity);
            painter->drawEllipse(rect);
            painter->setOpacity(1.0);
        }
    }
#endif
    painter->restore();
}

void
paintCachedGrip(QPainter *painter, const QStyleOption *option)
{
    bool useCache = UsePixmapCache;
    QString pixmapName;

    if (/* option->state & (QStyle::State_HasFocus | QStyle::State_MouseOver) ||*/ option->rect.width() * option->rect.height() > 4096)
    {
        useCache = false;
    }
    if (useCache)
    {
        QStyle::State state = option->state & (QStyle::State_Enabled | QStyle::State_On | QStyle::State_MouseOver | QStyle::State_Sunken | QStyle::State_HasFocus);
        if (!(state & QStyle::State_Enabled))
        {
            state &= ~(QStyle::State_MouseOver | QStyle::State_HasFocus);
        }
        state &= ~(QStyle::State_HasFocus);
        QByteArray colorName = option->palette.color(QPalette::Button).name().toLatin1();
        pixmapName = QString("scp-isg-%1-%2-%3-%4-%5")
            .arg(state, 0, 16)
            .arg(option->direction, 0, 16)
            .arg(QString(colorName))
            .arg(option->rect.width(), 0, 16)
            .arg(option->rect.height(), 0, 16);
    }
    paintIndicatorCached(painter, option,
        [ = ](QPainter * painter, const QStyleOption * option)
    {
        QStyleOption opt(*option);
        opt.rect.moveTo(0, 0);
        paintGrip(painter, &opt);
    },
    useCache, pixmapName);
}

void
QDialSkulptureStyle::drawComplexControl(ComplexControl cc,
    const QStyleOptionComplex *optc,
    QPainter *painter,
    const QWidget *widget) const
{

    if (cc != QStyle::CC_Dial)
    {
        QCommonStyle::drawComplexControl(cc, optc, painter, widget);
        return;
    }

    const QStyleOptionSlider *option = qstyleoption_cast<const QStyleOptionSlider *>(optc);
    if (option == nullptr)
        return;

    int d = qMin(option->rect.width() & ~1, option->rect.height() & ~1);
    QStyleOptionSlider opt = *option;
    const QAbstractSlider *slider = nullptr;
    // always highlight knob if pressed (even if mouse is not over knob)
    if ((option->state & QStyle::State_HasFocus) && (slider = qobject_cast<const QAbstractSlider *>(widget)))
    {
        if (slider->isSliderDown())
        {
            opt.state |= QStyle::State_MouseOver;
        }
    }

    // tickmarks
    opt.palette.setColor(QPalette::Inactive, QPalette::WindowText, QColor(80, 80, 80, 255));
    opt.palette.setColor(QPalette::Active, QPalette::WindowText, QColor(80, 80, 80, 255));
    opt.state &= ~QStyle::State_HasFocus;
    opt.rect.setWidth(opt.rect.width() & ~1);
    opt.rect.setHeight(opt.rect.height() & ~1);
    opt.rect.moveCenter(option->rect.center());
    QCommonStyle::drawComplexControl(QStyle::CC_Dial, &opt, painter, widget);

    // focus rectangle
    if (option->state & QStyle::State_HasFocus)
    {
        QStyleOptionFocusRect focus;
        opt.state |= QStyle::State_HasFocus;
        focus.QStyleOption::operator=(opt);
        focus.rect.adjust(-1, -1, 1, 1);
        //drawPrimitive(QStyle::PE_FrameFocusRect, &focus, painter, widget);
    }
    opt.palette = option->palette;

    // dial base
    if (d <= 256)
    {
        QStyleOptionSlider topt(opt);
        topt.rect.adjust(2, 2, -2, -2);
        topt.rect.moveCenter(option->rect.center());
        paintIndicatorDial(painter, &topt);
    }
    else
    {
        // large dials are slow to render, do not render them
    }

    // dial knob
    d -= 6;
    int gripSize = (option->fontMetrics.height() / 4) * 2 - 1;
    opt.rect.setSize(QSize(gripSize, gripSize));
    opt.rect.moveCenter(option->rect.center());
    // angle calculation from qcommonstyle.cpp (c) Trolltech 1992-2007, ASA.
    qreal angle;
    int sliderPosition = option->upsideDown ? option->sliderPosition : (option->maximum - option->sliderPosition);
    int range = option->maximum - option->minimum;
    if (!range)
    {
        angle = M_PI / 2;
    }
    else if (option->dialWrapping)
    {
        angle = M_PI * 1.5 - (sliderPosition - option->minimum) * 2 * M_PI / range;
    }
    else
    {
        angle = (M_PI * 8 - (sliderPosition - option->minimum) * 10 * M_PI / range) / 6;
    }

    qreal rr = d / 2.0 - gripSize - 2;
    opt.rect.translate(int(0.5 + rr * cos(angle)), int(0.5 - rr * sin(angle)));
    paintCachedGrip(painter, &opt);
}
