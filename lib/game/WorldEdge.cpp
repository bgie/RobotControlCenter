/*  RobotControlCenter
    Copyright (C) 2021-2022 Kuppens Brecht

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "WorldEdge.h"
#include <QMap>
#include <QPolygonF>
#include <math.h>

struct WorldEdge::Data {
    Data()
        : z(0.0f)
    {
    }

    QPolygonF points;
    float z;
};

WorldEdge::WorldEdge(QObject* parent)
    : QObject(parent)
    , _d(new Data())
{
}

WorldEdge::~WorldEdge()
{
}

int WorldEdge::count() const
{
    return _d->points.size();
}

QPolygonF WorldEdge::points() const
{
    return _d->points;
}

void WorldEdge::setPoints(QPolygonF newPoints)
{
    sortPoints(newPoints);
    if (_d->points == newPoints)
        return;

    _d->points = newPoints;
    emit pointsChanged(_d->points);
}

void WorldEdge::addPoint(float x, float y, float z)
{
    if (_d->points.contains(QPointF(x, y)))
        return;

    _d->points.append(QPointF(x, y));
    sortPoints(_d->points);

    setZ(((_d->z * (_d->points.size() - 1)) + z) / _d->points.size()); // average all the z values

    emit pointsChanged(_d->points);
}

bool WorldEdge::removeAt(int index)
{
    if (index >= 0 && index < _d->points.size()) {
        _d->points.removeAt(index);
        emit pointsChanged(_d->points);
        return true;
    }
    return false;
}

bool WorldEdge::isInside(QPointF pos) const
{
    return _d->points.size() >= 3 && _d->points.containsPoint(pos, Qt::OddEvenFill);
}

QRectF WorldEdge::bounds() const
{
    return _d->points.boundingRect();
}

QRectF WorldEdge::boundsWithMargin(float ratio) const
{
    QRectF result = _d->points.boundingRect();
    QSizeF margin = result.size() * ratio;
    return result.adjusted(-margin.width(), -margin.height(), +margin.width(), +margin.height());
}

float WorldEdge::z() const
{
    return _d->z;
}

void WorldEdge::setZ(float newValue)
{
    if (qFuzzyIsNull(_d->z - newValue))
        return;

    _d->z = newValue;
    emit zChanged(newValue);
}

void WorldEdge::reset()
{
    _d->points.clear();
    setZ(0.0f);
    emit pointsChanged(_d->points);
}

void WorldEdge::sortPoints(QPolygonF& points)
{
    if (points.size() > 1) {
        QPointF center(0, 0);
        foreach (const QPointF& p, points) {
            center += p;
        }
        center /= points.size();

        QMap<float, QPointF> angle2point;
        foreach (const QPointF& p, points) {
            angle2point.insert(atan2(p.y() - center.y(), p.x() - center.x()), p);
        }

        points = QPolygonF(angle2point.values().toVector());
    }
}
