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
#include "Robot.h"
#include <QElapsedTimer>

#define TIMEOUT_INTERVAL_MSEC 2000

struct Robot::Data {
    Data(QByteArray id)
        : id(id)
    {
    }

    const QByteArray id;
    float batteryVoltage;
    QElapsedTimer lastDiscoveryTime;
};

Robot::Robot(QByteArray id, QObject* parent)
    : QObject(parent)
    , _d(new Data(id))
{
}

Robot::~Robot()
{
}

QByteArray Robot::id() const
{
    return _d->id;
}

float Robot::batteryVoltage() const
{
    return _d->batteryVoltage;
}

void Robot::discoveryMessageReceived(float batteryVoltage)
{
    _d->lastDiscoveryTime.start();
    _d->batteryVoltage = batteryVoltage;
}

bool Robot::connectionTimedOut() const
{
    return _d->lastDiscoveryTime.hasExpired(TIMEOUT_INTERVAL_MSEC);
}
