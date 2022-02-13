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
#include "Joystick.h"
#include <QHash>
#include <SDL2/SDL.h>
#include <SDL2/SDL_joystick.h>

struct Joystick::Data {
    Data(SDL_Joystick* joy)
        : joy(joy)
    {
    }

    SDL_Joystick* joy;
    QHash<uint8_t, bool> buttonState;
    QHash<uint8_t, int16_t> axisPosition;
};

Joystick::Joystick(SDL_Joystick* joy, QObject* parent)
    : IJoystick(parent)
    , _d(new Data(joy))
{
    // we are assuming the joystick has been opend (by the manager)
}

Joystick::~Joystick()
{
    if (SDL_JoystickGetAttached(_d->joy)) {
        SDL_JoystickClose(_d->joy);
    }
}

bool Joystick::isButtonPressed(uint8_t button) const
{
    return _d->buttonState.value(button);
}

int16_t Joystick::axisPosition(uint8_t axis) const
{
    return _d->axisPosition.value(axis);
}

void Joystick::setButtonChanged(uint8_t button, bool pressed)
{
    _d->buttonState[button] = pressed;
    emit buttonChanged(button, pressed);
    if (pressed) {
        emit buttonDown(button);
    } else {
        emit buttonUp(button);
    }
}

void Joystick::setAxisChanged(uint8_t axis, int16_t position)
{
    _d->axisPosition[axis] = position;
    emit axisChanged(axis, position);
}
