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
#include "RobotSocket.h"
#include "RobotSocketManager.h"
#include "robot/IRobotManager.h"
#include "robot/Robot.h"
#include <QList>

struct RobotSocketManager::Data {
    QList<RobotSocket*> sockets;
};

RobotSocketManager::RobotSocketManager(IRobotManager& robots, QObject* parent)
    : QObject(parent)
    , _d(new Data())
{
    connect(&robots, &IRobotManager::robotAdded, this, &RobotSocketManager::onRobotAdded);
    connect(&robots, &IRobotManager::robotRemoved, this, &RobotSocketManager::onRobotRemoved);
    foreach (auto r, robots.robots()) {
        onRobotAdded(r);
    }
}

RobotSocketManager::~RobotSocketManager()
{
    while (!_d->sockets.empty()) {
        auto p = _d->sockets.takeLast();
        emit robotSocketRemoved(p);
        delete p;
    }
    emit robotSocketChanged();
}

bool RobotSocketManager::robotsConnected() const
{
    return !_d->sockets.empty();
}

QList<RobotSocket*> RobotSocketManager::robotSocket() const
{
    return _d->sockets;
}

QList<QObject*> RobotSocketManager::robotSocketQObject() const
{
    QList<QObject*> result;
    for (auto p : qAsConst(_d->sockets)) {
        result << p;
    }
    return result;
}

void RobotSocketManager::onRobotAdded(Robot* robot)
{
    RobotSocket* socket = new RobotSocket(*robot, this);
    _d->sockets.append(socket);
    emit robotSocketAdded(socket);
    emit robotSocketChanged();
}

void RobotSocketManager::onRobotRemoved(Robot* robot)
{
    bool found = false;
    QMutableListIterator it(_d->sockets);
    while (it.hasNext()) {
        auto socket = it.next();
        if (socket->robot() == robot) {
            it.remove();
            emit robotSocketRemoved(socket);
            delete socket;
            found = true;
        }
    }
    if (found) {
        emit robotSocketChanged();
    }
}
