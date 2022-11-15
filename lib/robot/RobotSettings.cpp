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
#include "RobotSettings.h"
#include "Robot.h"
#include "RobotNetwork.h"

struct RobotSettings::Data {
    Data(IRobotManager& network)
        : network(network)
    {
    }

    IRobotManager& network;
    QMap<QByteArray, int> robot2Marker;
};

RobotSettings::RobotSettings(IRobotManager& network, QObject* parent)
    : QObject(parent)
    , _d(new Data(network))
{
    connect(&network, &RobotNetwork::robotAdded, this, &RobotSettings::onRobotAdded);
}

RobotSettings::~RobotSettings()
{
}

QMap<QByteArray, int> RobotSettings::robot2Marker() const
{
    return _d->robot2Marker;
}

void RobotSettings::setRobot2Marker(QMap<QByteArray, int> values)
{
    if (_d->robot2Marker == values)
        return;

    _d->robot2Marker = values;

    foreach (Robot* r, _d->network.robots()) {
        r->setMarkerId(_d->robot2Marker.value(r->id(), -1));
    }
    emit robot2MarkerChanged(_d->robot2Marker);
}

void RobotSettings::onRobotAdded(Robot* r)
{
    QSet<int> gamepadIndices;
    for (Robot* robot : _d->network.robots()) {
        if (robot != r) {
            gamepadIndices << robot->gamepadIndex();
        }
    }
    for (int i = 1; i <= gamepadIndices.count() + 1; ++i) {
        if (!gamepadIndices.contains(i)) {
            r->setGamepadIndex(i);
            break;
        }
    }
    r->setMarkerId(_d->robot2Marker.value(r->id(), -1));
    connect(r, &Robot::markerIdChanged, this, &RobotSettings::onRobotMarkerIdChanged);
}

void RobotSettings::onRobotMarkerIdChanged(int newId)
{
    if (auto robot = qobject_cast<Robot*>(sender())) {
        int oldId = _d->robot2Marker.value(robot->id(), -1);
        if (oldId != newId) {
            _d->robot2Marker[robot->id()] = newId;
            emit robot2MarkerChanged(_d->robot2Marker);
        }
    }
}
