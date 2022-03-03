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
#include <QPointF>
#include <QScopedPointer>
#include <QVector3D>

class MarkerTracker
{
public:
    MarkerTracker();
    ~MarkerTracker();

    void setPositionRotation(const QPointF& screenPos, const QVector3D& newPos, float newAngle, float elapsedMsecs);
    void setNotDetected(float elapsedMsecs);

    bool isDetected() const;
    QPointF screenPos() const;
    QVector3D pos() const;
    float angle() const;

    bool isDetectedFiltered() const;
    QVector3D filteredPos() const;
    float filteredAngle() const;

private:
    struct Data;
    QScopedPointer<Data> _d;
};
