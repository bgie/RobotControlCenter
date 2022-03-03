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
    QPolygonF points;
};

WorldEdge::WorldEdge(QObject* parent)
    : QObject(parent)
    , _d(new Data())
{
}

WorldEdge::~WorldEdge()
{
}

QVariantList WorldEdge::pointsVariantList() const
{
    QVariantList result;
    foreach (const QPointF& p, _d->points) {
        result << p;
    }
    return result;
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
    emit pointsChanged();
}

void WorldEdge::addPoint(QPointF pos)
{
    if (_d->points.contains(pos))
        return;

    _d->points.append(pos);
    sortPoints(_d->points);
    emit pointsChanged();
}

bool WorldEdge::removeAt(int index)
{
    if (index >= 0 && index < _d->points.size()) {
        _d->points.removeAt(index);
        emit pointsChanged();
        return true;
    }
    return false;
}

bool WorldEdge::isInside(QPointF pos) const
{
    return _d->points.containsPoint(pos, Qt::OddEvenFill);
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
