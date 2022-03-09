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
#include "FakeRobotManager.h"
#include "Robot.h"
#include <QMap>

struct FakeRobotManager::Data {
    Data()
        : nextRobotIndex(1)
    {
    }

    QMap<QByteArray, Robot*> robots;
    int nextRobotIndex;
};

FakeRobotManager::FakeRobotManager(QObject* parent)
    : IRobotManager(parent)
    , _d(new Data())
{
}

FakeRobotManager::~FakeRobotManager()
{
}

Robot* FakeRobotManager::robot(QByteArray id) const
{
    return _d->robots.value(id, nullptr);
}

QList<Robot*> FakeRobotManager::robots() const
{
    return _d->robots.values();
}

int FakeRobotManager::count() const
{
    return _d->robots.size();
}

void FakeRobotManager::addFakeRobot()
{
    Robot* r = new Robot(QStringLiteral("{Fake%1}").arg(_d->nextRobotIndex++).toLatin1(), QHostAddress(), 0, this);
    _d->robots[r->id()] = r;
    emit robotAdded(r);
    emit robotsChanged();
}
