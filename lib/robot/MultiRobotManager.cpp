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
#include "MultiRobotManager.h"
#include "Robot.h"

MultiRobotManager::MultiRobotManager(QObject* parent)
    : IRobotManager(parent)
{
}

void MultiRobotManager::add(IRobotManager& manager)
{
    _managers.append(&manager);
    connect(&manager, &IRobotManager::robotAdded, this, &IRobotManager::robotAdded);
    connect(&manager, &IRobotManager::robotRemoved, this, &IRobotManager::robotRemoved);
    connect(&manager, &IRobotManager::robotsChanged, this, &IRobotManager::robotsChanged);

    foreach (Robot* r, manager.robots()) {
        emit robotAdded(r);
    }
    emit robotsChanged();
}

Robot* MultiRobotManager::robot(QByteArray id) const
{
    foreach (auto manager, _managers) {
        if (auto robot = manager->robot(id)) {
            return robot;
        }
    }
    return nullptr;
}

QList<Robot*> MultiRobotManager::robots() const
{
    QList<Robot*> result;
    foreach (auto manager, _managers) {
        result += manager->robots();
    }
    return result;
}

int MultiRobotManager::count() const
{
    int result = 0;
    foreach (auto manager, _managers) {
        result += manager->count();
    }
    return result;
}
