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
#pragma once
#include <QObject>
#include <QPolygonF>
#include <QVariantList>

// the edge of the world, defined on a 2d plane with a fixed z position in 3d
class WorldEdge : public QObject {
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY pointsChanged)
    Q_PROPERTY(float z READ z WRITE setZ NOTIFY zChanged)

public:
    explicit WorldEdge(QObject* parent = nullptr);
    virtual ~WorldEdge() override;

    int count() const;
    QPolygonF points() const;
    void setPoints(QPolygonF newPoints);

    void addPoint(float x, float y, float z);
    bool removeAt(int index);

    bool isInside(QPointF pos) const;
    QRectF bounds() const;
    QRectF boundsWithMargin(float ratio = 0.125) const;

    float z() const;
    void setZ(float newValue);

    Q_INVOKABLE void reset();

signals:
    void pointsChanged();
    void zChanged(float newValue);

private:
    void sortPoints(QPolygonF& points);

private:
    struct Data;
    QScopedPointer<Data> _d;
};
