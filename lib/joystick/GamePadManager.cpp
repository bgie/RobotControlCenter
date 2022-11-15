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
#include "GamePadManager.h"
#include "GamePad.h"
#include "IJoystick.h"
#include "JoystickManager.h"

GamePadManager::GamePadManager(JoystickManager& joystickManager, QObject* parent)
    : QObject(parent)
    , _joystickManager(joystickManager)
{
    connect(&joystickManager, &JoystickManager::added, this, &GamePadManager::onJoystickAdded);
    connect(&joystickManager, &JoystickManager::removed, this, &GamePadManager::onJoystickRemoved);

    foreach (IJoystick* j, joystickManager.joysticks()) {
        onJoystickAdded(j);
    }
}

QVector<GamePad*> GamePadManager::gamepads() const
{
    return _gamepads;
}

int GamePadManager::count() const
{
    return _gamepads.size();
}

QList<QObject*> GamePadManager::gamePadQObjects() const
{
    QList<QObject*> result;
    foreach (auto gamepad, _gamepads) {
        result << gamepad;
    }
    return result;
}

GamePad* GamePadManager::gamepadForRobot(QByteArray id) const
{
    int i = id.toInt() - 1;
    return _gamepads.value(i, nullptr);
}

void GamePadManager::onJoystickAdded(IJoystick* j)
{
    auto gamepad = new GamePad(j);
    _gamepads.append(gamepad);
    emit gamePadAdded(gamepad);
    emit gamePadsChanged();
}

void GamePadManager::onJoystickRemoved(IJoystick* j)
{
    QMutableVectorIterator<GamePad*> it(_gamepads);
    while (it.hasNext()) {
        auto gamepad = it.next();
        if (gamepad->joystick() == j) {
            it.remove();
            emit gamePadRemoved(gamepad);
            emit gamePadsChanged();
            break;
        }
    }
}
