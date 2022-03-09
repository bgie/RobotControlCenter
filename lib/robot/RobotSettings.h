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
#include <QList>
#include <QMap>
#include <QObject>

class Robot;
class IRobotManager;

class RobotSettings : public QObject {
    Q_OBJECT
public:
    explicit RobotSettings(IRobotManager& network, QObject* parent = nullptr);
    virtual ~RobotSettings();

    QMap<QByteArray, int> robot2Marker() const;
    void setRobot2Marker(QMap<QByteArray, int> values);

signals:
    void robot2MarkerChanged(QMap<QByteArray, int> values);

private:
    void onRobotAdded(Robot* r);
    void onRobotMarkerIdChanged(int newId);

private:
    struct Data;
    QScopedPointer<Data> _d;
};
