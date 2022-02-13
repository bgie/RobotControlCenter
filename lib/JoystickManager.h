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
#include <QObject>
#include <QScopedPointer>

class SDL2EventLoop;
class IJoystick;

class JoystickManager : public QObject {
    Q_OBJECT

public:
    explicit JoystickManager(QObject* parent = nullptr);
    virtual ~JoystickManager();

    void setEventLoop(SDL2EventLoop* eventLoop);

    QList<IJoystick*> joysticks() const;

signals:
    void added(IJoystick* j);
    void removed(IJoystick* j);

private slots:
    void onJoystickAddedEvent(int32_t deviceIndex);
    void onJoystickRemovedEvent(int32_t instanceId);
    void onJoystickButtonChangedEvent(int32_t instanceId, uint8_t button, bool pressed);
    void onJoystickAxisChangedEvent(int32_t instanceId, uint8_t axis, int16_t position);

private:
    void initJoysticks();
    void createJoystick(int32_t deviceIndex);
    void clearJoysticks();

private:
    struct Data;
    QScopedPointer<Data> _d;
};
