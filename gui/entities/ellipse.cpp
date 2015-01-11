/**
 * ellipse.cpp
 *
 * Copyright (C) 2014 GreatDevelopers
 *
 * eCAD is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * eCAD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ellipse.h"

Ellipse::Ellipse(QPointF point1, QPointF point2, QPointF point3)
{
    /** set values of three points
     * and calculate radii of ellipse
    */
    p1 = point1;
    p2 = point2;
    p3 = point3;

    calculate();
}

Ellipse::Ellipse(int i, QPointF point1, QPointF point2, QPointF point3)
{
    // assigns id
    id = i;

    /** set values of three points
     * and calculate radii of ellipse
    */
    p1 = point1;
    p2 = point2;
    p3 = point3;

    calculate();
}

Ellipse::Ellipse(int i, QPointF point1, qreal rad, qreal radM, qreal angle)
{
    // assigns id
    id = i;

    /**
     * set values of center point
     * and radii of ellipse
    */
    p1 = point1;
    minRadius = rad;
    majRadius = radM;
    theta = angle;
}

void Ellipse::calculate()
{
    d12 = qSqrt(qPow((p2.x() - p1.x()), 2) + qPow((p2.y() - p1.y()), 2));
    d13 = qSqrt(qPow((p3.x() - p1.x()), 2) + qPow((p3.y() - p1.y()), 2));

    if (d12 > d13)
    {
        majRadius = d12;
        minRadius = d13;
        theta = atan2((p2.y() - p1.y()), (p2.x() - p1.x())) * (180 / M_PI);
    }
    else
    {
        majRadius = d13;
        minRadius = d12;
        theta = atan2((p3.y() - p1.y()), (p3.x() - p1.x())) * (180 / M_PI);
    }

    topLeft.setX(p1.x() - majRadius);
    topLeft.setY(p1.y() - minRadius);
    bottomRight.setX(p1.x() + majRadius);
    bottomRight.setY(p1.y() + minRadius);

    setTransformOriginPoint(p1);
    setRotation(theta);
}

int Ellipse::type() const
{
    // Enable the use of qgraphicsitem_cast with ellipse item.
    return Type;
}

QPainterPath Ellipse::shape() const
{
    // sets the shape of the ellipse for selection
    QPainterPath path, path2;
    qreal adjust = 10;
    path.addEllipse(p1, majRadius + adjust, minRadius + adjust);
    path2.addEllipse(p1, majRadius - adjust, minRadius - adjust);
    return path - path2;
}

QRectF Ellipse::boundingRect() const
{
    // bounding rectangle for ellipse
    return QRectF(p1.x() - majRadius, p1.y() - majRadius,
                  majRadius * 2, majRadius * 2);
}

void Ellipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget)
{
    // draws/paints the path of ellipse
    QPen paintpen;
    paintpen.setCosmetic(true);
    paintpen.setWidth(1);
    painter->setRenderHint(QPainter::Antialiasing);

    if (option->state & QStyle::State_Selected)
    {
        // sets brush for center point
        painter->setBrush(Qt::SolidPattern);
        paintpen.setColor(Qt::red);
        painter->setPen(paintpen);
        painter->drawEllipse(p1, 2, 2);

        // sets pen for circumference
        paintpen.setStyle(Qt::DashLine);
        paintpen.setColor(Qt::black);
        painter->setBrush(Qt::NoBrush);
        painter->setPen(paintpen);
        painter->drawEllipse(p1, majRadius, minRadius);
    }

    else
    {
        painter->setBrush(Qt::SolidPattern);
        painter->drawEllipse(p1, 2, 2);

        painter->setBrush(Qt::NoBrush);
        painter->setPen(paintpen);
        painter->drawEllipse(p1, majRadius, minRadius);
    }
}

getEntity *Ellipse::clone(int i)
{
    Ellipse *e = new Ellipse;
    e->id = i;
    e->p1.x();
    e->p1.y();
    e->p2.x();
    e->p2.y();
    e->p3.x();
    e->p3.y();
    e->theta = theta;
    e->minRadius = minRadius;
    e->majRadius = majRadius;
    return e;
}

QPointF Ellipse::getCenter()
{
    // returns center of ellipse
    return p1;
}
