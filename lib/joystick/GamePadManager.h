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
#include <QObject>
#include <QVector>

class JoystickManager;
class IJoystick;
class GamePad;

class GamePadManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> gamePads READ gamePadQObjects NOTIFY gamePadsChanged)
    Q_PROPERTY(int count READ count NOTIFY gamePadsChanged)

public:
    explicit GamePadManager(JoystickManager& joystickManager, QObject* parent = nullptr);

    QVector<GamePad*> gamepads() const;
    int count() const;
    QList<QObject*> gamePadQObjects() const;

    GamePad* gamepadForRobot(QByteArray id) const;

signals:
    void gamePadAdded(GamePad* g);
    void gamePadRemoved(GamePad* g);
    void gamePadsChanged();

private:
    void onJoystickAdded(IJoystick* j);
    void onJoystickRemoved(IJoystick* j);

private:
    JoystickManager& _joystickManager;
    QVector<GamePad*> _gamepads;
};
