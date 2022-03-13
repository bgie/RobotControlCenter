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
#include <QString>

class MarkerSceneItem
{
public:
    MarkerSceneItem(
        QString id = QString(),
        bool isDetected = false,
        QPointF unfilteredPos = QPointF(),
        QPointF filteredPos = QPointF(),
        float filteredAngle = 0.0f,
        bool isRobot = false,
        bool isOutsideWorld = false)
        : _id(id)
        , _isDetected(isDetected)
        , _unfilteredPos(unfilteredPos)
        , _filteredPos(filteredPos)
        , _filteredAngle(filteredAngle)
        , _isRobot(isRobot)
        , _isOutsideWorld(isOutsideWorld)
    {
    }

    QString id() const { return _id; }
    bool isDetected() const { return _isDetected; }
    QPointF unfilteredPos() const { return _unfilteredPos; }
    QPointF filteredPos() const { return _filteredPos; }
    float filteredAngle() const { return _filteredAngle; }
    bool isRobot() const { return _isRobot; }
    bool isOutsideWorld() const { return _isOutsideWorld; }

private:
    QString _id;
    bool _isDetected;
    QPointF _unfilteredPos;
    QPointF _filteredPos;
    float _filteredAngle;
    bool _isRobot;
    bool _isOutsideWorld;
};

