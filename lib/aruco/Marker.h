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
#include <QVector3D>

class Marker {
public:
    explicit Marker(int id = -1,
        bool isDetected = false,
        QPointF screenPos = QPointF(),
        QVector3D pos = QVector3D(),
        float angle = 0,
        bool isDetectedFiltered = false,
        QVector3D filteredPos = QVector3D(),
        float filteredAngle = 0);

    int id() const { return _id; }
    bool isDetected() const { return _isDetected; }
    QPointF screenPos() const { return _screenPos; }
    QVector3D pos() const { return _pos; }
    float angle() const { return _angle; }
    bool isDetectedFiltered() const { return _isDetectedFiltered; }
    QVector3D filteredPos() const { return _filteredPos; }
    float filteredAngle() const { return _filteredAngle; }

private:
    int _id;
    bool _isDetected;
    QPointF _screenPos;
    QVector3D _pos;
    float _angle;
    bool _isDetectedFiltered;
    QVector3D _filteredPos;
    float _filteredAngle;
};
