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
#include "JoystickManager.h"
#include "Joystick.h"
#include "SDL2EventLoop.h"
#include <QHash>
#include <SDL2/SDL.h>
#include <SDL2/SDL_joystick.h>

struct JoystickManager::Data {
    Data()
        : eventLoop(nullptr)
    {
    }

    SDL2EventLoop* eventLoop;
    QHash<SDL_JoystickID, Joystick*> joysticks;
};

JoystickManager::JoystickManager(QObject* parent)
    : QObject(parent)
    , _d(new Data())
{
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
}

JoystickManager::~JoystickManager()
{
    clearJoysticks();
    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
}

void JoystickManager::setEventLoop(SDL2EventLoop* eventLoop)
{
    if (_d->eventLoop == eventLoop)
        return;

    if (_d->eventLoop) {
        _d->eventLoop->disconnect(this);
    }
    clearJoysticks();

    _d->eventLoop = eventLoop;

    if (_d->eventLoop) {
        connect(_d->eventLoop, &SDL2EventLoop::joystickAdded, this, &JoystickManager::onJoystickAddedEvent);
        connect(_d->eventLoop, &SDL2EventLoop::joystickRemoved, this, &JoystickManager::onJoystickRemovedEvent);
        connect(_d->eventLoop, &SDL2EventLoop::joystickAxisChanged, this, &JoystickManager::onJoystickAxisChangedEvent);
        connect(_d->eventLoop, &SDL2EventLoop::joystickButtonChanged, this, &JoystickManager::onJoystickButtonChangedEvent);
    }
    initJoysticks();
}

QList<IJoystick*> JoystickManager::joysticks() const
{
    QList<IJoystick*> result;
    for (auto it = _d->joysticks.cbegin(); it != _d->joysticks.cend(); ++it) {
        result << it.value();
    }
    return result;
}

void JoystickManager::onJoystickAddedEvent(int32_t deviceIndex)
{
    createJoystick(deviceIndex);
}

void JoystickManager::onJoystickRemovedEvent(int32_t instanceId)
{
    auto j = _d->joysticks.take(instanceId);
    if (j) {
        emit removed(j);
        delete j;
    }
}

void JoystickManager::onJoystickButtonChangedEvent(int32_t instanceId, uint8_t button, bool pressed)
{
    auto j = _d->joysticks.value(instanceId);
    if (j) {
        j->setButtonChanged(button, pressed);
    }
}

void JoystickManager::onJoystickAxisChangedEvent(int32_t instanceId, uint8_t axis, int16_t position)
{
    auto j = _d->joysticks.value(instanceId);
    if (j) {
        j->setAxisChanged(axis, position);
    }
}

void JoystickManager::initJoysticks()
{
    int count = SDL_NumJoysticks();
    for (int i = 0; i < count; ++i) {
        createJoystick(i);
    }
}

void JoystickManager::createJoystick(int32_t deviceIndex)
{
    SDL_Joystick* joy = SDL_JoystickOpen(deviceIndex);
    if (joy) {
        int32_t id = SDL_JoystickInstanceID(joy);
        if (!_d->joysticks.contains(id)) {
            auto j = new Joystick(joy, this);
            _d->joysticks[id] = j;
            emit added(j);
        }
    }
}

void JoystickManager::clearJoysticks()
{
    auto joysticks = _d->joysticks.values();
    _d->joysticks.clear();

    foreach (auto j, joysticks) {
        emit removed(j);
        delete j;
    }
}
