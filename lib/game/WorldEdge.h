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

class WorldEdge : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList points READ pointsVariantList NOTIFY pointsChanged)

public:
    explicit WorldEdge(QObject* parent = nullptr);
    virtual ~WorldEdge() override;

    QVariantList pointsVariantList() const;
    QPolygonF points() const;
    void setPoints(QPolygonF newPoints);

    void addPoint(QPointF pos);
    bool removeAt(int index);

    bool isInside(QPointF pos) const;
    QRectF bounds() const;
    QRectF boundsWithMargin(float ratio = 0.125) const;

signals:
    void pointsChanged();

private:
    void sortPoints(QPolygonF& points);

private:
    struct Data;
    QScopedPointer<Data> _d;
};
